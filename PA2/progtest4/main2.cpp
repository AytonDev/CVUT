#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

class CMail
{
  public:
    CMail();
    CMail ( const char * from, const char * to, const char * body);
    ~CMail();
    bool operator == ( const CMail & x ) const {
      return (strcmp(m_Mail->m_From,x.m_Mail->m_From) ==0) && (strcmp(m_Mail->m_To, x.m_Mail->m_To)==0) && (strcmp(m_Mail->m_Body,x.m_Mail->m_Body)==0);
    }

   struct TMail {
       TMail(const char * from="",const char * to="",const char * body="");
       ~TMail();
       int m_RefCount;
       char *m_From;
       char *m_To;
       char *m_Body;


   };
   TMail *m_Mail;

    // todo
};
CMail::TMail::TMail(const char * from,const char * to,const char * body): m_RefCount(1){
  m_From = new char[strlen(from) +1];
  m_To = new char[strlen(to) +1];
  m_Body = new char[strlen(body) +1];
  strcpy(m_From, from);
  strcpy(m_To, to);
  strcpy(m_Body, body);
}
CMail::CMail ( const char * from, const char * to, const char * body ):m_Mail(new TMail(from, to, body)){

}
CMail::TMail::~TMail(){
    delete [] m_From;
    delete [] m_To;
    delete [] m_Body;
}
CMail::~CMail(){
     if ( -- m_Mail -> m_RefCount == 0 )
        delete m_Mail;
}

CMail::CMail():m_Mail(new TMail()) { }

struct TServer {
        struct TUser {
             //TUser();
             TUser(const char * email="");
             ~TUser();
             void resize(int **arr, int &size);
             void pushMail(int mailIndex, int *&arr, size_t &size, unsigned int &count);
             char * m_Email;
             size_t m_SizeInbox,  m_SizeOutBox;
             unsigned int m_CountInbox,m_CountOutbox;
             int *m_Inbox, *m_Outbox ;

         };
        TServer();
        TServer(TServer *src);
        ~TServer();
        int m_RefCount, m_CountMails, m_SizeMails, m_CountEMails, m_SizeEmails;
        CMail * m_Mails;
        TUser * m_Emails;

        void pushBackMail(const CMail &src);
        void pushBackEmail( const char  *email);
        void prinMails( ) {
            for(int i=0; i < m_CountMails; ++i){
                cout << m_Mails[i].m_Mail->m_From << " ";
                cout << m_Mails[i].m_Mail->m_To <<"  ";
                cout << m_Mails[i].m_Mail->m_Body <<endl;
            }
             for(int i=0; i < m_CountEMails; ++i){
                cout << m_Emails[i].m_Email << " ";
                cout << m_Emails[i].m_CountOutbox << " ";
                cout << m_Emails[i].m_CountInbox << endl;
            }
        }
        bool emailExist(const char *email, int &index) const;


    };


class CMailIterator
{
  public:
    CMailIterator(TServer *server, int *mails, int size );
    ~CMailIterator();
    operator bool( void ) const;
    bool operator !( void ) const;
    const CMail & operator * ( void ) const;
    CMailIterator & operator ++ ( void );

    // todo
    private:
        TServer *m_Server;
        int *m_Mailbox;
        int m_Size;
        int m_Possition;
};


class CMailServer
{
  public:
    CMailServer ( void );
    CMailServer ( const CMailServer & src );
    CMailServer & operator = ( const CMailServer & src );
    ~CMailServer ( void );
    void SendMail ( const CMail & m );
    void PrintMail(){
        m_Server->prinMails();
    }
    CMailIterator Outbox ( const char * email ) const;
    CMailIterator Inbox  ( const char * email ) const;

  private:

    TServer *m_Server;
    // todo
};


//TSERVER---------------------------------------
TServer::TServer() :m_RefCount(1),m_CountMails(0), m_SizeMails(10), m_CountEMails(0),
             m_SizeEmails(20), m_Mails(new CMail[m_SizeMails]),m_Emails(new TUser[m_SizeEmails]){

}

TServer::TServer(TServer *src): m_RefCount(1),m_CountMails(src->m_CountMails), m_SizeMails(src->m_SizeMails),
        m_CountEMails(src->m_CountEMails),m_SizeEmails(src->m_SizeEmails){
            m_Emails = new TUser[m_SizeEmails];
            m_Mails = new CMail[m_SizeMails];
            for(int i=0; i < m_CountEMails;++i){
                m_Emails[i].m_Email = new char[strlen(src->m_Emails[i].m_Email)+1];
                strcpy( m_Emails[i].m_Email,src->m_Emails[i].m_Email);
                m_Emails[i].m_CountInbox = src->m_Emails[i].m_CountInbox;
                m_Emails[i].m_CountOutbox = src->m_Emails[i].m_CountOutbox;

                m_Emails[i].m_SizeInbox = src->m_Emails[i].m_SizeInbox;
                m_Emails[i].m_SizeOutBox = src->m_Emails[i].m_SizeOutBox;
                delete [] m_Emails[i].m_Inbox;
                m_Emails[i].m_Inbox = new int[m_Emails[i].m_SizeInbox];
                m_Emails[i].m_Outbox = new int[m_Emails[i].m_SizeOutBox];
                for(int j=0; j < src->m_Emails[i].m_CountInbox;++j){
                    m_Emails[i].m_Inbox[j] = src->m_Emails[i].m_Inbox[j];
                }
                for(int j=0; j < src->m_Emails[i].m_CountOutbox;++j){
                   m_Emails[i].m_Outbox[j] =  src->m_Emails[i].m_Outbox[j] ;
                }
               
            }
            for(int i=0; i < m_CountMails; ++i){
                m_Mails[i] = src->m_Mails[i];
                src->m_Mails[i].m_Mail->m_RefCount++;
            }

}

TServer::~TServer(){
    delete [] m_Emails;
    delete [] m_Mails;
     
}
void TServer::pushBackEmail(const char * email){
    if(m_CountEMails < m_SizeEmails){
        delete [] m_Emails[m_CountEMails].m_Email;
         m_Emails[m_CountEMails].m_Email = new char[strlen(email)+1];
         strcpy(m_Emails[m_CountEMails++].m_Email,email);
    }
       
    else {
        int size = m_SizeEmails*5;
        TUser * tmp = new TUser[size];
        for(int i=0; i < m_CountEMails; ++i){
                tmp[i].m_Email = new char[strlen(m_Emails[i].m_Email)+1];
                strcpy( tmp[i].m_Email,m_Emails[i].m_Email);
                tmp[i].m_CountInbox = m_Emails[i].m_CountInbox;
                tmp[i].m_CountOutbox = m_Emails[i].m_CountOutbox;

                tmp[i].m_SizeInbox = m_Emails[i].m_SizeInbox;
                tmp[i].m_SizeOutBox = m_Emails[i].m_SizeOutBox;

                tmp[i].m_Inbox = new int[tmp[i].m_SizeInbox];
                tmp[i].m_Outbox = new int[tmp[i].m_SizeOutBox];
                memcpy( tmp[i].m_Inbox,m_Emails[i].m_Inbox,m_Emails[i].m_SizeInbox);
                memcpy( tmp[i].m_Outbox,m_Emails[i].m_Outbox, m_Emails[i].m_SizeOutBox);
        }
        delete [] m_Emails;
        m_Emails = tmp;
        m_SizeEmails = size;
        delete [] m_Emails[m_CountEMails].m_Email;
        m_Emails[m_CountEMails].m_Email = new char[strlen(email)+1];
        strcpy(m_Emails[m_CountEMails++].m_Email,email);



    }

}

void TServer::pushBackMail(const CMail &src){
    if(m_CountMails < m_SizeMails){
        delete [] m_Mails[m_CountMails].m_Mail->m_From;
        delete [] m_Mails[m_CountMails].m_Mail->m_To;
        delete []m_Mails[m_CountMails].m_Mail->m_Body;

        m_Mails[m_CountMails].m_Mail->m_From = new char[strlen(src.m_Mail->m_From)+1];
        m_Mails[m_CountMails].m_Mail->m_To = new char[strlen(src.m_Mail->m_To)+1];
        m_Mails[m_CountMails].m_Mail->m_Body =new char[strlen(src.m_Mail->m_Body)+1];
        strcpy(m_Mails[m_CountMails].m_Mail->m_From, src.m_Mail->m_From);
        strcpy(m_Mails[m_CountMails].m_Mail->m_To, src.m_Mail->m_To);
        strcpy(m_Mails[m_CountMails].m_Mail->m_Body, src.m_Mail->m_Body);
        m_CountMails++;
    }else {
        int size = m_CountMails*2;
        CMail * arr = new CMail[size];
        for(int i=0; i < m_CountMails; ++i){
            arr[i] = m_Mails[i];
            arr[i].m_Mail->m_RefCount++;
        }
        delete [] m_Mails;
        m_Mails = arr;
        m_SizeMails = size;
        delete [] m_Mails[m_CountMails].m_Mail->m_From;
        delete [] m_Mails[m_CountMails].m_Mail->m_To;
        delete []m_Mails[m_CountMails].m_Mail->m_Body;
        m_Mails[m_CountMails].m_Mail->m_From = new char[strlen(src.m_Mail->m_From)+1];
        m_Mails[m_CountMails].m_Mail->m_To = new char[strlen(src.m_Mail->m_To)+1];
        m_Mails[m_CountMails].m_Mail->m_Body =new char[strlen(src.m_Mail->m_Body)+1];
        strcpy(m_Mails[m_CountMails].m_Mail->m_From, src.m_Mail->m_From);
        strcpy(m_Mails[m_CountMails].m_Mail->m_To, src.m_Mail->m_To);
        strcpy(m_Mails[m_CountMails].m_Mail->m_Body, src.m_Mail->m_Body);
        m_CountMails++;


    }
}

//TUSER---------------------------------------
/*TServer::TUser::TUser(): m_SizeInbox(20), m_CountInbox(0), m_SizeOutBox(20),
            m_CountOutbox(0),m_Inbox(new int[m_SizeInbox]) ,m_Outbox(new int[m_SizeOutBox]){
}*/

TServer::TUser::TUser(const char *email): m_SizeInbox(2),m_SizeOutBox(2), m_CountInbox(0), 
            m_CountOutbox(0),m_Inbox(new int[m_SizeInbox]) ,m_Outbox(new int[m_SizeOutBox]){
                m_Email = new char[strlen(email)+1];
                strcpy(m_Email, email);
}
void TServer::TUser::pushMail(int index, int *&arr, size_t &size, unsigned int &count) {
    if(count < size){
        arr[count++] = index;
      }else {
       size_t newSize = size*2;
       int *newArr = new int[newSize];
       memcpy( newArr, arr, size * sizeof(int) );
       size = newSize;
       //delete [] arr;
        arr = newArr;
       arr[count++] = index;
      }
}
int i=0;
TServer::TUser::~TUser(){
    delete [] m_Inbox;
    delete [] m_Outbox;
}

//SERVER---------------------------------------
CMailServer::CMailServer() :m_Server(new TServer()) {
}

CMailServer::CMailServer ( const CMailServer & src ) :m_Server(new TServer(src.m_Server)) {
}

CMailServer::~CMailServer ( void ) {
     if ( -- m_Server -> m_RefCount == 0 ){
         delete  m_Server;
     }
        
}

CMailServer & CMailServer::operator = ( const CMailServer & src ) {
    if(-- m_Server-> m_RefCount == 0)
        delete m_Server;
    if(&src == this)
         return  *this;

    m_Server = new TServer(src.m_Server);
    return *this;

}

bool TServer::emailExist(const char * email, int &index)const {
    for(int i =0; i < m_CountEMails; ++i){
        if(strcmp(m_Emails[i].m_Email, email) == 0 ){
              index = i;
              return true;
        }
      }
      return false;
}

void CMailServer::SendMail( const CMail & m ){
     int userIndex=m_Server->m_CountEMails, mailIndex= m_Server->m_CountMails;
     m_Server->pushBackMail(m);

    if(!m_Server->emailExist(m.m_Mail->m_From, userIndex)){
       m_Server->pushBackEmail(m.m_Mail->m_From);
    }
    m_Server->m_Emails[userIndex].pushMail(mailIndex, m_Server->m_Emails[userIndex].m_Outbox,  m_Server->m_Emails[userIndex].m_SizeOutBox,  m_Server->m_Emails[userIndex].m_CountOutbox);
    userIndex=m_Server->m_CountEMails;
    if(!m_Server->emailExist(m.m_Mail->m_To, userIndex))
      m_Server->pushBackEmail(m.m_Mail->m_To);
    m_Server->m_Emails[userIndex].pushMail(mailIndex, m_Server->m_Emails[userIndex].m_Inbox,  m_Server->m_Emails[userIndex].m_SizeInbox,  m_Server->m_Emails[userIndex].m_CountInbox);

}

CMailIterator CMailServer::Inbox(const char * email) const {
    int userIndex=0;
     m_Server->m_RefCount++;
    if(m_Server->emailExist(email, userIndex)){
       
       return CMailIterator(m_Server,m_Server->m_Emails[userIndex].m_Inbox, m_Server->m_Emails[userIndex].m_CountInbox);
    }
     return CMailIterator(m_Server, nullptr,0);
}

CMailIterator CMailServer::Outbox(const char * email) const {
    int userIndex=0;
      m_Server->m_RefCount++;
    if(m_Server->emailExist(email, userIndex)){
       return CMailIterator(m_Server,m_Server->m_Emails[userIndex].m_Outbox, m_Server->m_Emails[userIndex].m_CountOutbox);
    }
    return CMailIterator(m_Server, nullptr,0);
}

//CMailIterator---------------------------------------
CMailIterator::CMailIterator(TServer *server, int *mails, int size) : m_Server(server), m_Mailbox(mails), m_Size(size), m_Possition(0) {

}

CMailIterator::~CMailIterator() {
    if( -- m_Server->m_RefCount == 0){  
        delete m_Server;
    }
    
       
}

CMailIterator::operator bool( void ) const {
    return m_Possition < m_Size;
}

bool CMailIterator::operator!(void ) const {
   return m_Possition >= m_Size;
}

const CMail & CMailIterator::operator * ( void ) const {
     return m_Server->m_Mails[m_Mailbox[m_Possition]];
}

 CMailIterator &CMailIterator::operator ++ ( void ) {
     m_Possition++;
     return *this;
 }




#ifndef __PROGTEST__
int main ( void )
{
  char from[100], to[100], body[1024];

  

  assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
  CMailServer s0;
  s0 . SendMail ( CMail ( "john", "peter", "some important mail" ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "thomas", sizeof ( to ) );
  strncpy ( body, "another important mail", sizeof ( body ) );
  s0 . SendMail ( CMail ( from, to, body ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "alice", sizeof ( to ) );
  strncpy ( body, "deadline notice", sizeof ( body ) );
  s0 . SendMail ( CMail ( from, to, body ) );
  s0 . SendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
  s0 . SendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
  CMailIterator i0 = s0 . Inbox ( "alice" );
  assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ! ++i0 );

  CMailIterator i1 = s0 . Inbox ( "john" );
  assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
  assert ( ! ++i1 );

  CMailIterator i2 = s0 . Outbox ( "john" );
  assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ! ++i2 );

  CMailIterator i3 = s0 . Outbox ( "thomas" );
  assert ( ! i3 );

  CMailIterator i4 = s0 . Outbox ( "steve" );
  assert ( ! i4 );

  CMailIterator i5 = s0 . Outbox ( "thomas" );
  s0 . SendMail ( CMail ( "thomas", "boss", "daily report" ) );
  assert ( ! i5 );

  CMailIterator i6 = s0 . Outbox ( "thomas" );
  assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
  assert ( ! ++i6 );

  CMailIterator i7 = s0 . Inbox ( "alice" );
  s0 . SendMail ( CMail ( "thomas", "alice", "meeting details" ) );
  assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ! ++i7 );

  CMailIterator i8 = s0 . Inbox ( "alice" );
  assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( ! ++i8 );

  CMailServer s1 ( s0 );
  s0 . SendMail ( CMail ( "joe", "alice", "delivery details" ) );
  s1 . SendMail ( CMail ( "sam", "alice", "order confirmation" ) );
  CMailIterator i9 = s0 . Inbox ( "alice" );
  assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( ! ++i9 );

  CMailIterator i10 = s1 . Inbox ( "alice" );
  assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
  assert ( ! ++i10 );

  CMailServer s2;
  s2 . SendMail ( CMail ( "alice", "alice", "mailbox test" ) );
  CMailIterator i11 = s2 . Inbox ( "alice" );
  assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
  assert ( ! ++i11 );

  s2 = s0;
  s0 . SendMail ( CMail ( "steve", "alice", "newsletter" ) );
  s2 . SendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
  CMailIterator i12 = s0 . Inbox ( "alice" );
  assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
  assert ( ! ++i12 );

  CMailIterator i13 = s2 . Inbox ( "alice" );
  assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
  assert ( ! ++i13 );

  return 0;
}
#endif /* __PROGTEST__ */
