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
using namespace std;

const uint16_t ENDIAN_LITTLE = 0x4949;
const uint16_t ENDIAN_BIG    = 0x4d4d;

#endif /* __PROGTEST__ */


class Image {
private:
    int width;
    int length;
    char indien;
    const char *srcFileName;
    const char *dstFileName;
    int interLeaveTo;
    int interLeaveFrom;
    int **newPixels;
    int **pixels;
public:

    Image(const char * srcFile, const char *dstFile, int interLeave) {
        srcFileName = srcFile;
        dstFileName = dstFile;
        interLeaveTo = interLeave;

    }

    bool readData() {
        ifstream file ( srcFileName, ios::binary | ios::in );
        char c;
        char header[9] = "";
        if (file.fail()) {
            return false;
        }
        file.read(header, sizeof(header)-1);

        indien = (unsigned char)header[1];
        width =  (int)(unsigned char)header[3] + (int)(unsigned char)header[2];
        length = (int)(unsigned char)header[5] + (int)(unsigned char)header[4];

        pixels = new int*[length];
        newPixels = new int*[length];
        for(int i = 0; i <length; ++i){
            pixels[i] = new int[width];
            newPixels[i] = new int[width];
        }
        int xPosition=0, yPosition=0;

        while(file.get(c)) {
            if(xPosition >=width){
                yPosition++;
                xPosition=0;
            }

            pixels[yPosition][xPosition++]= (int)(unsigned char)c;
        }
        return true;
    }

    bool interleave1toN(){
        int xPosition=0, yPosition=0;
        for(int i =0; i < length; i +=interLeaveTo){
            for(int j=0; j < width; j+= interLeaveTo){
                if(xPosition >=width){
                    yPosition++;
                    xPosition=0;
                }
                newPixels[yPosition][xPosition++]=pixels[i][j];
            }
        }

        int last;
        while (interLeaveTo != 1){
            last = interLeaveTo;
            interLeaveTo = interLeaveTo >>1;
            for(int i =0; i < length; i +=interLeaveTo){
                for(int j=0; j < width; j+=interLeaveTo){
                    if(i % last != 0 || j % last != 0 ) {
                        if (xPosition >= width) {
                            yPosition++;
                            xPosition = 0;
                        }
                        cout  << i << " " << j << " " << yPosition<< " " << xPosition++ << endl;
                        newPixels[yPosition][xPosition++] = pixels[i][j];
                        cout << pixels[i][j] << endl;
                    }
                }
            }
        }

        return true;
    }

    bool interleaveNto1() {
        return true;
    }

    bool saveFile() {
        ofstream save(dstFileName, ios::binary | ios::trunc);
        char buffer[1];
        buffer[0] = indien;
       save.write(buffer , 1);
        for(int i=0; i < length; ++i){
            for(int j=0; j <width; ++j){
               // save << newPixels[i][j];
            }
        }
        return true;
    }

};

bool recodeImage ( const char  * srcFileName, const char  * dstFileName,  int interleave, uint16_t byteOrder )
{
  // todo
   /* Image image(srcFileName, dstFileName, interleave);
    image.readData();
    image.interleave1toN();
    image.saveFile();
    */
    int i =8;
    i = i >> 1;
    cout << i << endl;




    return true;

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
  assert ( recodeImage ( "input_00.img", "output_00.img", 1, ENDIAN_LITTLE )
           && identicalFiles ( "output_00.img", "ref_00.img" ) );
/*
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
  assert ( recodeImage ( "extra_input_01.img", "extra_out_01.img", 4, ENDIAN_BIG )
           && identicalFiles ( "extra_out_01.img", "extra_ref_01.img" ) );
  assert ( recodeImage ( "extra_input_02.img", "extra_out_02.img", 16, ENDIAN_BIG )
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
           && identicalFiles ( "extra_out_11.img", "extra_ref_11.img" ) );*/
  return 0;
}
#endif /* __PROGTEST__ */


/*ifstream file ( srcFileName, ios::binary | ios::in );
char c;
char header[9] = "";

if (file.fail()) {
    cout << "fail" << endl;
    return false;
}
file.read(header, sizeof(header)-1);
cout <<"indian: "<< hex << setw(2) << (int)(unsigned char)header[0];
cout << hex << setw(2) << (int)(unsigned char)header[1] << endl;
int width =  (int)(unsigned char)header[3] + (int)(unsigned char)header[2];
int length = (int)(unsigned char)header[5] + (int)(unsigned char)header[4];
int pixels[length][width];
int newPixels[length][width];
cout << width << "x" << length << endl;
int xposition=0, yposition=0;
int testarray[100];
while( file.get(c) ){

    if(yposition >= width){
        xposition++;
        yposition=0;
    }
    pixels[xposition][yposition] =  (int)(unsigned char)c;
    yposition++;

}

for(int i =0; i < length; ++i){
    for(int j=0; j < width; ++j){
       cout << ' ' << hex <<setfill('0') << setw(2) << pixels[i][j];
    }
    cout << endl;
}

cout << endl;

cout << endl;

for(int i =0; i < length; i +=interleave){
    for(int j=0; j < width; j+= interleave){
            cout << ' ' << hex <<setfill('0') << setw(2) << pixels[i][j];
    }
}
int last;
while (interleave != 1){
    last = interleave;
    interleave = interleave /2;
    for(int i =0; i < length; i +=interleave){
        for(int j=0; j < width; j+=interleave){
            if(i % last != 0 || j % last != 0 )
                cout << ' ' << hex <<setfill('0') << setw(2) << pixels[i][j];
        }
    }
}
interleave = 8;
cout << endl;*/

























