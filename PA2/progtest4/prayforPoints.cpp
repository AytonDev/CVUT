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
    CMail ( const char * from, const char * to, const char * body);
    ~CMail();
    bool operator == ( const CMail & x ) const {
      return (strcmp(m_Mail->m_From,x.m_Mail->m_From) ==0) && (strcmp(m_Mail->m_To, x.m_Mail->m_To)==0) && (strcmp(m_Mail->m_Body,x.m_Mail->m_Body)==0);
    }

   struct TMail {
       explicit TMail(const char * from="",const char * to="",const char * body="");
       ~TMail();
       int m_RefCount;
       char *m_From;
       char *m_To;
       char *m_Body;
       CMail *m_NextInbox, *m_NextOutbox, *m_NextMail;
   };
   TMail *m_Mail;

    // todo
};

//Mail Constructor
CMail::CMail( const char * from, const char * to, const char * body): m_Mail(new TMail(from, to, body)) {
  

}
CMail::CMail::TMail::TMail( const char * from, const char * to, const char * body): m_RefCount(1), m_NextInbox(nullptr), 
    m_NextOutbox(nullptr), m_NextMail(nullptr) {
  m_From = new char[strlen(from) +1];
  m_To = new char[strlen(to) +1];
  m_Body = new char[strlen(body) +1];
  strcpy(m_From, from);
  strcpy(m_To, to);
  strcpy(m_Body, body);
}
CMail::TMail::~TMail(){
    delete [] m_From;
    delete [] m_To;
    delete [] m_Body;
}
CMail::~CMail(){
   // cout << "info " << m_Mail->m_Body  << endl;
     if ( -- m_Mail -> m_RefCount == 0 )
        delete m_Mail;
}

struct TServer {
        struct TUser {
             //TUser();
             explicit TUser(const char * email="");
             ~TUser();
             //void pushMail(int mailIndex, int *&arr, size_t &size, unsigned int &count);
             char * m_Email;
             void pushMailOutbox(CMail *newMail,CMail **firstOne, CMail **lastOne, int &size);
             void pushMailInbox(CMail *newMail,CMail **firstInbox, CMail **lastInbox, int &size);

             int m_SizeInbox,  m_SizeOutbox;
             //pointers to the linkedList of in/outbox mails.
             CMail *m_FirstInbox, *m_LastInbox, *m_FirstOutbox, *m_LastOutbox;

         };
        TServer();
        explicit TServer(TServer *src);
        ~TServer();
        unsigned int m_RefCount, m_CountMails, m_CountEMails;
        size_t  m_SizeEmails;
        CMail * m_FirstMail, *m_LastMail;
        TUser * m_Emails;

        void pushBackMail(CMail **src);
        void pushBackEmail( const char  *email);
        bool emailExist(const char *email, int &index) const;

        void printMails(){
            
            CMail *tmp = m_FirstMail;
            while(tmp != nullptr){
               // cout << tmp->m_Mail->m_From << " " << tmp->m_Mail->m_To << " " << tmp->m_Mail->m_Body << endl;
                tmp = tmp->m_Mail->m_NextMail;
            }
        }


};

class CMailIterator
{
  public:
    CMailIterator(TServer *server, CMail *mailBox, int size, bool type );
    ~CMailIterator();
    operator bool( ) const;
    bool operator !(  ) const;
    const CMail & operator * (  ) const;
    CMailIterator & operator ++ (  );

    // todo
    private:
        TServer *m_Server;
        CMail *m_Mailbox;
        int m_Size, m_Position;
        bool m_Type;
};

class CMailServer
{
  public:
    CMailServer (  );
    CMailServer ( const CMailServer & src );
    CMailServer & operator = ( const CMailServer & src );
    ~CMailServer (  );
    void SendMail ( const CMail & m );
    void PrintMail(){
        CMail *mail = m_Server->m_FirstMail;

        while( mail != nullptr){
         
        //    cout << mail->m_Mail->m_From <<" " << mail->m_Mail->m_To <<" " << mail->m_Mail->m_Body  << " " << mail->m_Mail->m_RefCount<<endl;
            mail = mail->m_Mail->m_NextMail;
        }
       
    }
    CMailIterator Outbox ( const char * email ) const;
    CMailIterator Inbox  ( const char * email ) const;

  private:
    TServer *m_Server;
    // todo
};

//CMailServer
CMailServer::CMailServer(): m_Server(new TServer()) {
}

CMailServer::CMailServer ( const CMailServer & src ): m_Server(new TServer(src.m_Server)) {
}

CMailServer::~CMailServer(){
   // cout << m_Server->m_RefCount << " " << m_Server <<endl;
    if( -- m_Server->m_RefCount == 0)
        delete m_Server;
}

void CMailServer::SendMail(const CMail & m){
    auto *mail = new CMail(m.m_Mail->m_From, m.m_Mail->m_To, m.m_Mail->m_Body);
    m_Server->pushBackMail(&mail);
    
    int userIndex = m_Server->m_CountEMails;
    if(m_Server->emailExist(mail->m_Mail->m_From,userIndex))
        m_Server->m_Emails[userIndex].pushMailOutbox(m_Server->m_LastMail, &m_Server->m_Emails[userIndex].m_FirstOutbox, &m_Server->m_Emails[userIndex].m_LastOutbox, m_Server->m_Emails[userIndex].m_SizeOutbox);
    else {
        m_Server->pushBackEmail(mail->m_Mail->m_From);
        m_Server->m_Emails[userIndex].pushMailOutbox(m_Server->m_LastMail, &m_Server->m_Emails[userIndex].m_FirstOutbox, &m_Server->m_Emails[userIndex].m_LastOutbox, m_Server->m_Emails[userIndex].m_SizeOutbox);
    }
    
    //cout << userIndex << endl;
    userIndex = m_Server->m_CountEMails;

    if(m_Server->emailExist(mail->m_Mail->m_To,userIndex))
        m_Server->m_Emails[userIndex].pushMailInbox(m_Server->m_LastMail, &m_Server->m_Emails[userIndex].m_FirstInbox, &m_Server->m_Emails[userIndex].m_LastInbox, m_Server->m_Emails[userIndex].m_SizeInbox);
    else {

        m_Server->pushBackEmail(m.m_Mail->m_To);
        m_Server->m_Emails[userIndex].pushMailInbox(m_Server->m_LastMail, &m_Server->m_Emails[userIndex].m_FirstInbox, &m_Server->m_Emails[userIndex].m_LastInbox, m_Server->m_Emails[userIndex].m_SizeInbox);
    }
}

CMailIterator CMailServer::Inbox(const char *email) const{
    int index;
    m_Server->m_RefCount++;
    if(m_Server->emailExist(email,index)){
        return CMailIterator(m_Server,m_Server->m_Emails[index].m_FirstInbox,m_Server->m_Emails[index].m_SizeInbox,true);
    }else {
        return CMailIterator(m_Server,nullptr,0, true);
    }
}

CMailIterator CMailServer::Outbox(const char *email) const{
    int index;
    m_Server->m_RefCount++;
    if(m_Server->emailExist(email,index)){
        return CMailIterator(m_Server,m_Server->m_Emails[index].m_FirstOutbox,m_Server->m_Emails[index].m_SizeOutbox, false);
    }else {
        return CMailIterator(m_Server,nullptr,0, false);
    }
}

//TServer 
TServer::TServer(): m_RefCount(1), m_CountMails(0), m_CountEMails(0), m_SizeEmails(20), m_FirstMail(nullptr), 
    m_LastMail(nullptr), m_Emails(new TUser[m_SizeEmails])  {
}

TServer::TServer(TServer *src): m_RefCount(1), m_CountMails(src->m_CountMails), m_CountEMails(src->m_CountEMails),
 m_SizeEmails(src->m_SizeEmails), m_FirstMail(nullptr), m_LastMail(nullptr),m_Emails(new TUser[m_SizeEmails])  {
     CMail *tmp = src->m_FirstMail;
     CMail *mail;
        for( unsigned int i =0; i < m_CountEMails; ++i){
            delete []  m_Emails[i].m_Email;
            m_Emails[i].m_Email = new char[strlen(src->m_Emails[i].m_Email)+1];
            strcpy( m_Emails[i].m_Email,src->m_Emails[i].m_Email);
            m_Emails[i].m_SizeInbox = 0;
            m_Emails[i].m_SizeOutbox = 0;
        }

        while(tmp != nullptr){
                mail = new CMail(tmp->m_Mail->m_From,tmp->m_Mail->m_To, tmp->m_Mail->m_Body) ;
                pushBackMail(&mail);
                int userIndex;
                if(emailExist(mail->m_Mail->m_From,userIndex))
                    m_Emails[userIndex].pushMailOutbox(mail, &m_Emails[userIndex].m_FirstOutbox, &m_Emails[userIndex].m_LastOutbox, m_Emails[userIndex].m_SizeOutbox);

                if(emailExist(mail->m_Mail->m_To,userIndex))
                    m_Emails[userIndex].pushMailInbox(mail, &m_Emails[userIndex].m_FirstInbox, &m_Emails[userIndex].m_LastInbox, m_Emails[userIndex].m_SizeInbox);
                tmp = tmp->m_Mail->m_NextMail;
        }

       
        
}

CMailServer & CMailServer::operator = ( const CMailServer & src ){
    if(&src == this)
        return  *this;

    if(-- m_Server-> m_RefCount == 0)
        delete m_Server;

    m_Server = new TServer(src.m_Server);
    return *this;
}


TServer::~TServer() {
    delete [] m_Emails;
    CMail *tmp = m_FirstMail;
    while(m_FirstMail != nullptr){
        tmp = m_FirstMail->m_Mail->m_NextMail;
        delete m_FirstMail;
        m_FirstMail = tmp; 
    }
}

//Push the mail to the end of Linked list
void TServer::pushBackMail(CMail **src ){
    //check if the mail is first
    if(m_FirstMail == nullptr){ 
        m_FirstMail = *src;
        m_LastMail = *src;
    }
    else{
        if(m_FirstMail->m_Mail->m_NextMail == nullptr )
            m_FirstMail->m_Mail->m_NextMail= *src;
        m_LastMail->m_Mail->m_NextMail = *src;
        m_LastMail = *src;
        
    }
}

//check if a user already exist
 bool TServer::emailExist(const char *email, int &index) const{
      for(unsigned int i =0; i < m_CountEMails; ++i){
        if(strcmp(m_Emails[i].m_Email, email) == 0 ){
              index = i;
              return true;
        }
    }
    return false;
 }


void TServer::pushBackEmail( const char  *email) {
    if(m_CountEMails < m_SizeEmails){
        delete [] m_Emails[m_CountEMails].m_Email;
        m_Emails[m_CountEMails].m_Email = new char[strlen(email)+1];
        strcpy(m_Emails[m_CountEMails++].m_Email,email);
    }
    else {
        size_t size = m_SizeEmails*2;
        auto * tmp = new TUser[size];
        for(unsigned int i=0; i < m_CountEMails; ++i){
                delete [] tmp[i].m_Email;
                tmp[i].m_Email = new char[strlen(m_Emails[i].m_Email)+1];
                strcpy( tmp[i].m_Email,m_Emails[i].m_Email);
                tmp[i].m_SizeInbox = m_Emails[i].m_SizeInbox;
                tmp[i].m_SizeOutbox = m_Emails[i].m_SizeOutbox;

                tmp[i].m_FirstInbox = m_Emails[i].m_FirstInbox;
                tmp[i].m_FirstOutbox = m_Emails[i].m_FirstOutbox;

                tmp[i].m_LastInbox = m_Emails[i].m_LastInbox;
                tmp[i].m_LastOutbox = m_Emails[i].m_LastOutbox;
        }
        delete [] m_Emails;
        m_Emails = tmp;
        m_SizeEmails = size;
        delete [] m_Emails[m_CountEMails].m_Email;
        m_Emails[m_CountEMails].m_Email = new char[strlen(email)+1];
        strcpy(m_Emails[m_CountEMails++].m_Email,email);
    }

}

//Tuser
TServer::TUser::TUser(const char * email): m_Email(new char[strlen(email)+1]), m_SizeInbox(0), m_SizeOutbox(0), m_FirstInbox(nullptr),
        m_LastInbox(nullptr), m_FirstOutbox(nullptr), m_LastOutbox(nullptr) {
            strcpy(m_Email,email);
}

TServer::TUser::~TUser(){
    delete [] m_Email; 
}

void TServer::TUser::pushMailOutbox(CMail *newMail,CMail **firstOutbox, CMail **lastOutbox, int &size) {
     if(*firstOutbox == nullptr){
         *firstOutbox = newMail;
         *lastOutbox = newMail;
     }else {
         if((*firstOutbox)->m_Mail->m_NextOutbox == nullptr )
             (*firstOutbox)->m_Mail->m_NextOutbox= newMail;
         (*lastOutbox)->m_Mail->m_NextOutbox = newMail;
         *lastOutbox = newMail;

     }
     size++;
}

void TServer::TUser::pushMailInbox(CMail *newMail,CMail **firstInbox, CMail **lastInbox, int &size) {
    if(*firstInbox == nullptr){
        *firstInbox = newMail;
        *lastInbox = newMail;
    }else {
        if((*firstInbox)->m_Mail->m_NextInbox == nullptr )
            (*firstInbox)->m_Mail->m_NextInbox= newMail;
        (*lastInbox)->m_Mail->m_NextInbox = newMail;
        *lastInbox = newMail;

    }
    size++;
}

//CMailIterator ---- 
CMailIterator::CMailIterator(TServer *server, CMail *mailBox, int size, bool type): m_Server(server), m_Mailbox(mailBox),
    m_Size(size), m_Position(0), m_Type(type) {

}

CMailIterator::~CMailIterator(){
   // cout << m_Server->m_RefCount << " " << m_Server <<endl;
    if(--m_Server->m_RefCount == 0)
        delete m_Server;
}

CMailIterator::operator bool( ) const {
    return m_Position < m_Size;
}

bool CMailIterator::operator!( ) const {
   return m_Position >= m_Size;
}

const CMail & CMailIterator::operator * (  ) const {
     return *m_Mailbox;
}

CMailIterator &CMailIterator::operator ++ ( ) {
     m_Position++;
     if(m_Type)
         m_Mailbox = m_Mailbox->m_Mail->m_NextInbox;
     else
         m_Mailbox = m_Mailbox->m_Mail->m_NextOutbox;
     return *this;
}
bool funkce() {
return nullptr;
}


#ifndef __PROGTEST__

int main (  )
{
    funkce();
    /*

  //CMail("mail", "to", "me");
  
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

  //s0.PrintMail();
 
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
  //s0.PrintMail();
  //cout <<endl;
  //s2.PrintMail();
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

*/

   
   
  return 0;
}
#endif /* __PROGTEST__ */