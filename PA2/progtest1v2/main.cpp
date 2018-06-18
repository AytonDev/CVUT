#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cctype>
#include <climits>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
#include <string>

using namespace std;

const uint16_t ENDIAN_LITTLE = 0x4949;
const uint16_t ENDIAN_BIG    = 0x4d4d;

#endif /* __PROGTEST__ */

class Image {
private:
    const char * m_srcFileName;
    const char * m_dstFileName;
    char header[9];
    int m_width;
    int m_length;
    int m_size;
    int m_interleave;
    int m_interleaveFrom;
    int m_endian;
    int m_endianTo;
    int m_format;
    int m_channel;
    int m_bitsPerCh;
    long int **pixels;
    long int ** newPixels;
    int m_newFormat;
    int shift;


public:
    Image(int interleave,  const char * srcFileName , const char * dstFileName, uint16_t endian) {
        m_interleave = interleave;
        m_srcFileName =srcFileName;
        m_dstFileName = dstFileName;
        m_endianTo = endian;

    }
    ~Image(){

    }

    bool loadImage() {
        //char buffer[100];

        if((m_interleave %2 != 0 || m_interleave/2 > 33 || m_interleave == 0) && m_interleave != 1){
            return  false;
        }


        ifstream file ( m_srcFileName, ios::binary | ios::in );
        if(file.good()) {
           if(! file.read(header, sizeof(header)-1))
               return false;

            m_endian = ((int)(unsigned char)header[1] << 8) + (int)(unsigned char)header[0];


            if(m_endian == ENDIAN_LITTLE) {
                m_width = ((int)(unsigned char)header[3] << 8 )+ (int)(unsigned char)header[2];
                m_length = ((int)(unsigned char)header[5] << 8 )+ (int)(unsigned char)header[4];
                m_format = (int)(unsigned char)header[6];
                if( (int)(unsigned char)header[7] != 0){
                    return false;
                }
            }
            else if (m_endian == ENDIAN_BIG) {
                m_width = ((int)(unsigned char)header[2] << 8 )+ (int)(unsigned char)header[3];
                m_length = ((int)(unsigned char)header[4] << 8 )+ (int)(unsigned char)header[5];
                m_format = (int)(unsigned char)header[7];
                if( (int)(unsigned char)header[6] != 0){
                    return false;
                }

            }else {
                return  false;
            }



            cout << m_width << "x" << m_length << endl;


            m_size = m_length* m_width;
            m_interleaveFrom = m_format >> 5;
            if(m_interleaveFrom >= 7 || m_width == 0 || m_length ==0 ){
                return false;
            }
            m_bitsPerCh = (m_format >> 2) - (m_interleaveFrom << 3);
            m_channel =  m_format  - ((m_format >> 2) << 2);
            m_interleaveFrom = (int)pow(2,m_format >> 5);

           // cout << m_interleaveFrom << endl;
            if(m_bitsPerCh == 1 || m_bitsPerCh == 2 || m_bitsPerCh >= 5){
                return false;
            }


             pixels= new long int*[m_length];
            newPixels = new long int*[m_length];
            for(int i=0; i <m_length; ++i){
                pixels[i] = new long int[m_width]();
                newPixels[i] = new long int[m_width]();
            }

            char buffer[10];
            int row = 0, coll = 0;
            shift = (int)pow(2, m_channel);
            int length=0;
            int count=0;

            int channel = m_channel;
            cout << "chanel " << channel << endl;
            if(m_bitsPerCh == 3){
                if(m_channel == 0){
                    length = -9;
                    channel = 1;
                }
                else if (m_channel == 1)
                    return  false;
                else if(m_channel == 2){
                    length =-7;
                    channel= 3;
                }
                else {
                    length=-6;
                    channel=4;

                }


            }
            else if(m_bitsPerCh == 4){
                if(channel== 0){
                    channel=2;
                    length = -8;
                }else if (channel == 1)
                    return  false;
                else if(channel == 2){
                    channel=  6;
                    length = -4;
                }
                else {
                    channel=8;
                    length = -2;
                }
            }
            cout << "chanel :2 " << channel << endl;
            while(file.read(buffer, sizeof(buffer)+ length)){
               // cout << "chanel " << channel << endl;
                count++;
                if(count > m_size){
                    cout << "size fail" << endl ;
                    return false;
                }
                for(int i=0; i < channel; ++i){
                    pixels[row][coll] = pixels[row][coll] <<8;
                    pixels[row][coll] +=  (int )(unsigned char)buffer[i];

                }
                //cout<<  pixels[row][coll] << hex <<setw(4) << setfill('0')<< endl;
                coll++;

                if(coll == m_width){
                    row++;
                    coll=0;
                }
            }
            file.close();

            for(int i=0; i < m_length; ++i){
                for(int j=0; j<m_width; ++j){
                    cout<< hex <<  setfill('0') << setw(12) << pixels[i][j] << " ";

                }
                cout << endl;

            }

            if(coll != 0 || row != m_length){
                return false;

            }



        }else {
            return  false;
        }




        if(m_interleaveFrom == 1){

            oneToN(pixels, newPixels);
              if(!saveImage(newPixels))
                return false;
        }else {
            NtoOne(pixels, newPixels);
            oneToN(newPixels, pixels);
            if(!saveImage(pixels))
                return  false;
        }


        return  true;

    }

    void oneToN(long int **pixels, long int **outPixels){
      /*  for(int i = 0; i < m_length; ++i){
            for(int j=0; j < m_width; ++j)
                cout << " " << hex << setfill('0') << setw(2)  <<pixels[i][j];
            cout<< endl;
        }
        */
        //cout <<  "ok " << m_interleave << endl;
        int xPosition=0, yPosition=0, interLeaveTo = m_interleave;
        for(int i =0; i < m_length; i +=interLeaveTo){
            for(int j=0; j < m_width; j+= interLeaveTo){
                if(xPosition >=m_width){
                    yPosition++;
                    xPosition=0;
                }
                outPixels[yPosition][xPosition++]=pixels[i][j];
            }
        }

        int last;
        while (interLeaveTo != 1){
            last = interLeaveTo;
            interLeaveTo = interLeaveTo >>1;
            for(int i =0; i < m_length; i +=interLeaveTo){
                for(int j=0; j < m_width; j+=interLeaveTo){
                    if(i % last != 0 || j % last != 0 ) {
                        if (xPosition >= m_width) {
                            yPosition++;
                            xPosition = 0;
                        }
                        outPixels[yPosition][xPosition++] = pixels[i][j];
                    }
                }
            }
        }

        cout << endl;
        /*for(int i=0; i < m_length; ++i){
            for(int j=0; j<m_width; ++j){
                prefix++;
                cout<< hex <<  setfill('0') << setw(2) << outPixels[i][j] << " ";
                if(prefix %16 == 15)
                    cout << endl;
            }

        }*/


    }


    void NtoOne(long int **pixels, long int **outPixels) {
        int xPoss =0, yPoss=0, inter= m_interleaveFrom, last;

        for(int i =0; i < m_length; i +=inter){
            for(int j=0; j < m_width; j+= inter){
                if(xPoss >=m_width){
                    yPoss++;
                    xPoss=0;
                }
                outPixels[i][j]=pixels[yPoss][xPoss++];
            }
        }

        while (inter != 1){
            last = inter;
            inter = inter >>1;
            for(int i =0; i < m_length; i +=inter){
                for(int j=0; j < m_width; j+=inter){
                    if(i % last != 0 || j % last != 0 ) {
                        if (xPoss >= m_width) {
                            yPoss++;
                            xPoss = 0;
                        }
                        outPixels[i][j] = pixels[yPoss][xPoss++];
                    }
                }
            }
        }



    }


    bool saveImage(long int **pixels) {

        cout << endl; cout << endl;
        for(int i=0; i < m_length; ++i){
            for(int j=0; j<m_width; ++j){
                cout<< hex <<  setfill('0') << setw(12) << pixels[i][j] << " ";

            }
            cout << endl;

        }

      //  cout << "hovno"<<endl;
        int inter = (int)log2((double)m_interleave);
        m_newFormat = inter;
        m_newFormat= m_newFormat << 5 ;

        m_newFormat += m_bitsPerCh << 2;
        m_newFormat += m_channel;



        ofstream save(m_dstFileName, ios::binary | ios::trunc);
        int length = 0;
        char buffer[10];
        char header[9];
        int channel = m_channel;

        if(m_bitsPerCh == 3){
            if(m_channel ==0){
                length = -9;
                channel=1;
            }
            else if(m_channel == 2){
                length =-7;
                channel= 3;
            }else {
                channel=4;
                length= -6;
            }
        }
        else if(m_bitsPerCh == 4){
            if(channel== 0){
                channel=2;
                length = -8;
            }
            else if(channel == 2){
                cout<< m_endianTo << endl;
                channel=  6;
                length = -4;
            }
            else {
                channel=8;
                length = -2;
            }

        }


       // buffer[0] = m_endian;

        header[0] = m_endianTo;
        header[1] = m_endianTo >>8;
        if(m_endianTo == ENDIAN_LITTLE){
            header[2] = m_width;
            header[3] = m_width >>8;
            header[4] = m_length;
            header[5] = m_length >>8;
            header[6] = m_newFormat;
            header[7] =0;

        }else {
            header[3] = m_width;
            header[2] = m_width>>8 ;
            header[5] = m_length;
            header[4] = m_length >>8;
            header[7] = m_newFormat;
            header[6] = 0;
        }

        if (save.fail()){
            return  false;
        }

        if(!save.write(header, sizeof(header)-1))
            return false;

        for(int i=0; i < m_length; ++i){
            for(int j=0; j < m_width; ++j){

                if(m_endianTo == ENDIAN_LITTLE || m_endian == ENDIAN_BIG){
                    for(int k=channel-1; k >= 0; --k){
                        buffer[k] = (pixels[i][j]);
                        pixels[i][j] =  pixels[i][j] >>8;
                    }
                    if(!save.write(buffer, sizeof(buffer) + length))
                        return false;
                }else if(m_endianTo == ENDIAN_BIG){

                    for(int k=0; k < channel; ++k){
                        buffer[k] = (pixels[i][j]);
                        pixels[i][j] =  pixels[i][j] << 8;
                    }
                    save.write(buffer, sizeof(buffer) + length);
                }

            }
        }
        save.close();
        return true;


    }



};

bool recodeImage ( const char  * srcFileName,
                   const char  * dstFileName,
                   int           interleave,
                   uint16_t      byteOrder )
{
  // todo
    Image image(interleave,srcFileName, dstFileName, byteOrder);
    return  image.loadImage();


}

#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1,
                      const char * fileName2 )
{
  // DIY
    return  true;
}

int main ( void )
{
    cout << sizeof(unsigned int) << endl;

 /* assert ( recodeImage ( "input_00.img", "output_00.img", 1, ENDIAN_LITTLE )
           && identicalFiles ( "output_00.img", "ref_00.img" ) );

  assert ( recodeImage ( "input_01.img", "output_01.img", 8, ENDIAN_LITTLE )
           && identicalFiles ( "output_01.img", "ref_01.img" ) );

  assert ( recodeImage ( "input_02.img", "output_02.img", 8, ENDIAN_LITTLE )
           && identicalFiles ( "output_02.img", "ref_02.img" ) );

  assert ( recodeImage ( "input_03.img", "output_03.img", 2, ENDIAN_LITTLE )
           && identicalFiles ( "output_03.img", "ref_03.img" ) );

  assert ( recodeImage ( "input_04.img", "output_04.img", 1, ENDIAN_LITTLE )
           && identicalFiles ( "output_04.img", "ref_04.img" ) );

  assert ( recodeImage ( "input_05.img", "output_05.img", 1, ENDIAN_LITTLE )
           && identicalFiles ( "output_05.img", "ref_05.img" ) );

  assert ( recodeImage ( "input_06.img", "output_06.img", 8, ENDIAN_LITTLE )
           && identicalFiles ( "output_06.img", "ref_06.img" ) );

  assert ( recodeImage ( "input_07.img", "output_07.img", 4, ENDIAN_LITTLE )
           && identicalFiles ( "output_07.img", "ref_07.img" ) );

  assert ( recodeImage ( "input_08.img", "output_08.img", 8, ENDIAN_LITTLE )
           && identicalFiles ( "output_08.img", "ref_08.img" ) );

  assert ( ! recodeImage ( "input_09.img", "output_09.img", 1, ENDIAN_LITTLE ) );

  assert ( ! recodeImage ( "input_10.img", "output_10.img", 5, ENDIAN_LITTLE ) );

 // extra inputs (optional & bonus tests)
 assert ( recodeImage ( "extra_input_00.img", "extra_out_00.img", 8, ENDIAN_LITTLE )
           && identicalFiles ( "extra_out_00.img", "extra_ref_00.img" ) );
  */ assert ( recodeImage ( "extra_input_01.img", "extra_out_01.img", 4, ENDIAN_BIG )
           && identicalFiles ( "extra_out_01.img", "extra_ref_01.img" ) );
 /* assert ( recodeImage ( "extra_input_02.img", "extra_out_02.img", 16, ENDIAN_BIG )
           && identicalFiles ( "extra_out_02.img", "extra_ref_02.img" ) );
  assert ( recodeImage ( "extra_input_03.img", "extra_out_03.img", 1, ENDIAN_LITTLE )
           && identicalFiles ( "extra_out_03.img", "extra_ref_03.img" ) );
  assert ( recodeImage ( "extra_input_04.img", "extra_out_04.img", 8, ENDIAN_LITTLE )
           && identicalFiles ( "extra_out_04.img", "extra_ref_04.img" ) );
  assert ( recodeImage ( "extra_input_05.img", "extra_out_05.img", 4, ENDIAN_LITTLE )
           && identicalFiles ( "extra_out_05.img", "extra_ref_05.img" ) );
  assert ( recodeImage ( "extra_input_06.img", "extra_out_06.img", 16, ENDIAN_BIG )
           && identicalFiles ( "extra_out_06.img", "extra_ref_06.img" ) );
  assert ( recodeImage ( "extra_input_07.img", "extra_out_07.img", 1, ENDIAN_BIG )
           && identicalFiles ( "extra_out_07.img", "extra_ref_07.img" ) );
  assert ( recodeImage ( "extra_input_08.img", "extra_out_08.img", 8, ENDIAN_LITTLE )
           && identicalFiles ( "extra_out_08.img", "extra_ref_08.img" ) );
  assert ( recodeImage ( "extra_input_09.img", "extra_out_09.img", 4, ENDIAN_LITTLE )
           && identicalFiles ( "extra_out_09.img", "extra_ref_09.img" ) );
  assert ( recodeImage ( "extra_input_10.img", "extra_out_10.img", 16, ENDIAN_BIG )
           && identicalFiles ( "extra_out_10.img", "extra_ref_10.img" ) );
  assert ( recodeImage ( "extra_input_11.img", "extra_out_11.img", 1, ENDIAN_BIG )
           && identicalFiles ( "extra_out_11.img", "extra_ref_11.img" ) );
*/
  return 0;

}
#endif /* __PROGTEST__ */
