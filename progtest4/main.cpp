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


class CStr {
  public:
  CStr(const char * str = "");
  friend bool operator == (const CStr & x1,const CStr & x2);
  private:
  struct TData{
    TData(const char * str );
    int m_Len;
    int m_Max;
    char *m_Str;
  };
  TData *m_Data;
  friend std::ostream    & operator <<         ( std::ostream & os, const CStr & x );
};

CStr::TData::TData ( const char * str ): m_Len ( strlen ( str ) ), m_Max ( m_Len + 1 ), m_Str ( new char[m_Max] )
{
   std::memcpy( m_Str, str, m_Len + 1 );
}

CStr::CStr( const char * str ): m_Data ( new TData ( str ) ){}

bool operator == (const CStr & x1, const CStr & x2) {
  if( strcmp(x1.m_Data->m_Str, x2.m_Data->m_Str) == 0) return true;
  else 
    return false;
}

std::ostream     & operator <<     ( std::ostream & os, const CStr & x )
{
  os << x . m_Data -> m_Str;
  return os;
}




struct TUser {
  CStr email;
  int inboxSize=1, inboxCount=0, outboxSize=1, outboxCount=0;
  int *inbox = new int[inboxSize], *outbox = new int[outboxSize];
};

class CMail
{
  public:
  CStr m_from, m_to, m_body;

  CMail() {}
  
  CMail( const char *from, const char * to, const char * body ):m_from(from), m_to(to),m_body(body){  }

  bool operator == ( const CMail & x ) const {
    return (m_from == x.m_from && m_to == x.m_to && m_body == x.m_body);
  }
  private:
    // todo
};

struct TServer {
    int m_refCount,
        m_SizeMails, 
        m_SizeUsers,
        m_CountMails,
        m_CountUsers; 
    CMail * m_Mails;
    TUser * m_Users;

    TServer():m_refCount(1),  m_SizeMails(1), m_SizeUsers(1),m_CountMails(0), m_CountUsers(0),
              m_Mails(new CMail[m_SizeMails]), m_Users(new TUser[m_SizeUsers]){

    }
    TServer(TServer &src):m_refCount(1),  m_SizeMails(src.m_SizeMails), m_SizeUsers(src.m_SizeUsers),
      m_CountMails(src.m_CountMails),m_CountUsers(src.m_CountUsers),m_Mails(new CMail[m_SizeMails]), m_Users(new TUser[m_SizeUsers])
       {
         for(int i=0; i < m_CountMails; ++i)
              m_Mails[i] = src.m_Mails[i];
          for(int i=0; i < m_CountUsers; ++i){
            m_Users[i] = src.m_Users[i];
            m_Users[i].inbox = new int[m_Users[i].inboxSize];
            m_Users[i].outbox = new int[m_Users[i].outboxSize];
          for(int j=0; j < m_Users[i].inboxCount; ++j)
            m_Users[i].inbox[j] = src.m_Users[i].inbox[j];
          for(int j=0; j < m_Users[i].outboxCount; ++j)
            m_Users[i].outbox[j] = src.m_Users[i].outbox[j];
      
    }

    }
    ~TServer() {
      delete [] m_Mails;
      delete [] m_Users;
      //TODO
    }

    int pushMail(const CMail &mail){
      if(m_CountMails < m_SizeMails) m_Mails[m_CountMails] = mail;
      else {
        int newSize = m_SizeMails*2;
        CMail* newArr = new CMail[newSize]; 
        memcpy( newArr, m_Mails, m_SizeMails * sizeof(CMail) );
        m_SizeMails = newSize;
        delete [] m_Mails;
        m_Mails = newArr;
        m_Mails[m_CountMails] = mail;
      }
      return m_CountMails++;
    }

    void pushEmailAddress(CStr email){
      if(m_CountUsers < m_SizeUsers)
        m_Users[m_CountUsers++].email = email;
      else {
        size_t newSize = m_SizeUsers * 2;
        TUser* newArr = new TUser[newSize];
        memcpy( newArr, m_Users, m_SizeUsers * sizeof(TUser) );
        m_SizeUsers = newSize;
        delete [] m_Users;
        m_Users = newArr;
        m_Users[m_CountUsers++].email = email;
      }
    }

    bool emailAddressExist(CStr email, int &index) const {
      for(int i =0; i < m_CountUsers; ++i){
        if(m_Users[i].email == email ){  
          index = i;
          return true;
        }   
      }
      return false;
    }

    void SaveMail(const CMail &mail){
      int emailIndex =0;
      pushMail(mail);
      if(emailAddressExist(mail.m_from, emailIndex))
         pushMailOutbox(m_CountMails-1, emailIndex);
      else {
        pushEmailAddress(mail.m_from);
        pushMailOutbox(m_CountMails-1, m_CountUsers-1);
      }

      if(emailAddressExist(mail.m_to, emailIndex))
        pushMailInbox(m_CountMails-1, emailIndex); 
      else {
        pushEmailAddress(mail.m_to);
        pushMailInbox(m_CountMails-1, m_CountUsers-1);
      }
    }

    void pushMailOutbox(int mailIndex, int userIndex){
     if(m_Users[userIndex].outboxCount < m_Users[userIndex].outboxSize )
        m_Users[userIndex].outbox[m_Users[userIndex].outboxCount++]= mailIndex;
     else {
       resize(m_Users[userIndex].outboxSize, &m_Users[userIndex].outbox);
        m_Users[userIndex].outbox[m_Users[userIndex].outboxCount++]= mailIndex;
     }
    }

    void pushMailInbox(int mailIndex, int userIndex){
      cout << userIndex << endl;
      if(m_Users[userIndex].inboxCount < m_Users[userIndex].inboxSize ){
        m_Users[userIndex].inbox[m_Users[userIndex].inboxCount++]= mailIndex;
      }else {
       resize(m_Users[userIndex].inboxSize, &m_Users[userIndex].inbox);
       m_Users[userIndex].inbox[m_Users[userIndex].inboxCount++]= mailIndex;
      }
    }

    void resize(int &size, int **arr) {
      size_t newSize = size * 2;
      int* newArr = new int[newSize];
      memcpy( newArr, *arr, size * sizeof(int) );
      size = newSize;
      delete [] *arr;
      *arr = newArr;
    }

  };



class CMailIterator
{

  public:
  CMailIterator (TServer *server, int *mails, int count){
      m_Server = server;
      m_MailIndex = mails;
      m_Count = count;
      m_Possition = 0;
  }

  ~CMailIterator(){
    if(--m_Server->m_refCount ==0)
      delete m_Server;
  }

  operator bool( void ) const {
    return m_Possition < m_Count;
  }

  bool operator ! ( void ) const {
     return m_Possition >= m_Count;
  }

  const CMail & operator * ( void ) const {
    return m_Server->m_Mails[*(m_MailIndex+m_Possition)];
  }
 

  CMailIterator & operator ++ ( void ) {
     m_Possition++;
     return *this;
  }
  private:
    // todo
    int *m_MailIndex, m_Count, m_Possition;
    TServer *m_Server;

};


class CMailServer
{
  public:
  CMailServer ( void ): m_Server( new TServer()){
  
  }

  CMailServer ( const CMailServer & src){
    m_Server = new TServer(*src.m_Server);
 
  }

  CMailServer & operator = ( const CMailServer & src ) {
    if(--m_Server->m_refCount ==0)
    delete m_Server;

    m_Server = new TServer(*src.m_Server);
    return *this;
 
  }
  ~CMailServer ( void ){
   if(--m_Server->m_refCount ==0)
    delete m_Server;
  };
  void SendMail ( const CMail & m ) {
      m_Server->SaveMail(m);
  }
  
    

  CMailIterator Outbox ( const char * email ) const {
    m_Server->m_refCount++;
    int index = 0;
    if(m_Server->emailAddressExist(email, index))
      return CMailIterator(m_Server,m_Server->m_Users[index].outbox,m_Server->m_Users[index].outboxCount);
    else return CMailIterator(m_Server,nullptr,0);
  }


  CMailIterator Inbox ( const char * email ) const {
    m_Server->m_refCount++;
    cout << m_Server->m_Users->inboxCount <<" " << m_Server->m_Users->inboxSize <<  endl;
    int index = 0;
    if(m_Server->emailAddressExist(email, index))
      return  CMailIterator(m_Server,m_Server->m_Users[index].inbox,m_Server->m_Users[index].inboxCount);
    else  return CMailIterator(m_Server,nullptr,0);
    
  }
  /*
  void printMails() {
    for(int i =0; i < m_CountMails; ++i){
      cout << m_Mails[i].m_from << " ";
      cout << m_Mails[i].m_to << " ";
      cout << m_Mails[i].m_body << endl;
    }
  }
  void printPersonInbox() {
    cout << m_Users[3].inboxCount <<endl;
    
    for(int i=0; i < m_Users[3].inboxCount; ++i){
      cout << m_Users[3].inbox[i] << " ";
    }
  }
  void printPersonOutbox() {
    cout << m_Users[0].outboxCount <<endl;
    
    for(int i=0; i < m_Users[0].outboxCount; ++i){
      cout << m_Users[0].outbox[i] << " ";
    }
    cout << endl;
  }*/

  private:
  // todo
  TServer *m_Server;

  
};

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
  assert ( ++i7 );
  CMail mail = *i7;
  cout << mail.m_body << mail.m_to << mail.m_from <<endl;
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
    cout << "Test mazani mailserveru pred pouzitim iteratoru" << endl;
    timeStart = getTimeStamp();
   
    CMailServer * test;
    test = new CMailServer();
    test->SendMail(CMail("haranti", "elisa", "Lol plesatec xD tahni"));
    test->SendMail(CMail("elisa", "buh", "oni se mi smeji :("));
    test->SendMail(CMail("buh", "medvedice", "plz kill, alespon 42 z nich"));
    test->SendMail(CMail("buh", "elisa", "vyrizeno xD"));
   
    // ulozit
    CMailIterator elisa_odeslane = test->Outbox("elisa");
    CMailIterator elisa_prijate = test->Inbox("elisa");
   
    // a znicit
    delete test;
   
    assert( elisa_odeslane && *elisa_odeslane == CMail("elisa", "buh", "oni se mi smeji :(") );
    assert(! ++elisa_odeslane );
   
    assert( elisa_prijate && *elisa_prijate == CMail("haranti", "elisa", "Lol plesatec xD tahni") );
    assert( ++elisa_prijate && *elisa_prijate == CMail("buh", "elisa", "vyrizeno xD") );
    assert(! ++elisa_prijate );
   
    cout << "test OK (" << ((getTimeStamp() - timeStart)*1000) << " ms)" << endl << endl;