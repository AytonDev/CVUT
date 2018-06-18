#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect 
{
  public:
  CRect ( double x, double y,double w, double h ) : m_X ( x ), m_Y ( y ), m_W ( w ), m_H ( h )
  {
  }
  friend ostream & operator << ( ostream & os, const CRect & x )
  {
    return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
  }
  
  double m_X;
  double m_Y;
  double m_W;
  double m_H;
};
#endif /* __PROGTEST__ */

/**
 * Base class of Window Elements
 */
class CWindowElement
{
  public:
  CWindowElement(int id, const CRect &relPos);

  virtual ~CWindowElement() 
  { }

  virtual void Print(ostream & os, bool last =false, bool item=false) const =0;

  virtual CWindowElement * Clone ( ) const = 0;

  friend std::ostream & operator<<(std::ostream & os, const CWindowElement & v)
  { v.Print(os,false, true); return os; }  
  
  int m_Id;
  CRect m_RelPos;

  // pointer to element's parent - CWindow absolute size and position.
  CRect * m_Parent;

};

class CButton : public CWindowElement
{
  public:
  CButton( int id, const CRect &relPos, const string& name );

  CWindowElement * Clone ( ) const { return new CButton ( *this ); }

  void Print(ostream & os, bool last, bool item) const;
  private: 
  string m_Name;
};

class CInput : public CWindowElement
{
  public:
  CInput ( int id, const CRect & relPos, const string& value );

  CWindowElement * Clone ( ) const { return new CInput ( *this ); }

  // SetValue 
  void SetValue(const string &value);
  // GetValue 
  string GetValue() const ;

  void Print(ostream & os, bool last, bool item) const;

  private:
  string m_Value; 
};
class CLabel : public CWindowElement
{
  public:
  CLabel( int id,const CRect& relPos,const string & label );

  CWindowElement * Clone ( ) const { return new CLabel ( *this ); }

  void Print(ostream & os, bool last, bool item) const;

  private:
  string m_Label;
};

class CComboBox : public CWindowElement
{
  public:
  CComboBox ( int id, const CRect& relPos );

  CComboBox * Clone ( ) const {  return new CComboBox ( *this ); }
  // Add   
  CComboBox & Add(string item);
  // SetSelected
  void SetSelected(int index =0);

  // GetSelected
  int GetSelected() const;

  void Print(ostream & os, bool last, bool item) const;

 
  private:
  vector<string> m_List;
  unsigned int m_Position;
  
};

class CWindow 
{
  public:
  CWindow( const string & title, const CRect & absPos );

  //copy constructor, makes deep copy
  CWindow(const CWindow &src);

  //deletes all items in the vector
  ~CWindow();

  ///overload operator '=' makes deep copy of src
  CWindow &operator = (const CWindow & src);

  // Add
  CWindow &Add (const CWindowElement & newElemnt);

  // Search
  CWindowElement *Search(int index);

  // SetPosition
  void SetPosition(const CRect &absPos);

  friend ostringstream & operator <<(ostringstream  &output, const CWindow &window) { 
    output << "Window \"" << window.m_Title <<"\" " << window.m_AbsPos << endl;
    
     for(unsigned int i = 0; i <window.m_WindowElements.size() -1; ++i){
      output << "+- " ; window.m_WindowElements[i]->Print(output);
      }
      output << "+- " ; window.m_WindowElements[window.m_WindowElements.size()-1]->Print(output, true);
     
    return output;
  }
  
  private:
    string m_Title;
    CRect m_AbsPos;
    vector< CWindowElement*> m_WindowElements;
    
};


//CWindowElement
CWindowElement::CWindowElement(int id, const CRect &relPos) : m_Id(id), m_RelPos(relPos){
}

//CWindow
CWindow::CWindow(const string &title, const CRect &absPos ) : m_Title(title), m_AbsPos(absPos){
}

CWindow &CWindow::operator=(const CWindow &src) {
  if(&src == this)
    return *this;
  m_Title = src.m_Title;
  m_AbsPos = src.m_AbsPos;
  for(unsigned int i=0; i < m_WindowElements.size(); ++i){
    delete m_WindowElements[i];
  }
  m_WindowElements.clear();
  for(unsigned int i = 0; i < src.m_WindowElements.size(); ++i){
    CWindowElement *element = src.m_WindowElements[i]->Clone();
    element->m_Parent = &m_AbsPos;
    m_WindowElements.push_back(element);
  }
  return *this;
}

CWindow::CWindow(const CWindow &src): m_Title(src.m_Title), m_AbsPos(src.m_AbsPos) {
  for(unsigned int i = 0; i < src.m_WindowElements.size(); ++i){
    CWindowElement *element = src.m_WindowElements[i]->Clone();
    element->m_Parent = &m_AbsPos;
    m_WindowElements.push_back(element);
  }
}

CWindow::~CWindow() {
  for(unsigned int i = 0; i < m_WindowElements.size(); ++i){
    delete m_WindowElements[i];
  }
}

CWindow &CWindow::Add(const CWindowElement &element) {
  CWindowElement *newElement = element.Clone();
  newElement->m_Parent = &m_AbsPos;
  m_WindowElements.push_back(newElement);
  return *this;

}

CWindowElement *CWindow::Search(int index)  {
  for(unsigned int i = 0; i < m_WindowElements.size(); ++i){
    if(m_WindowElements[i]->m_Id == index){
       return m_WindowElements[i];
    }
  }
  return nullptr;
}

void CWindow::SetPosition(const CRect &absPos) {
  m_AbsPos = absPos;
}

//CButton
CButton::CButton( int id, const CRect &relPos, const string &name ) :CWindowElement(id, relPos), m_Name(name){
}

void CButton::Print(ostream & os, bool last, bool item) const {
  CRect pos = CRect(m_Parent->m_W * m_RelPos.m_X+ m_Parent->m_X, m_Parent->m_H * m_RelPos.m_Y + m_Parent->m_Y, m_Parent->m_W * m_RelPos.m_W, m_Parent->m_H * m_RelPos.m_H ); 
  os << "[" << m_Id <<"] Button \""<< m_Name << "\" " << pos << endl;
}

//CInput 
CInput::CInput(int id, const CRect &relPos, const string &value) :CWindowElement(id, relPos), m_Value(value) {
}

void CInput::Print(ostream & os, bool last, bool item) const {
  CRect pos = CRect(m_Parent->m_W * m_RelPos.m_X+ m_Parent->m_X, m_Parent->m_H * m_RelPos.m_Y + m_Parent->m_Y, m_Parent->m_W * m_RelPos.m_W, m_Parent->m_H * m_RelPos.m_H ); 
  os << "[" << m_Id <<"] Input \""<< m_Value << "\" " << pos << endl;
}

string CInput::GetValue() const {
  return m_Value;
}

void CInput::SetValue(const string &value)  {
  m_Value = value;
}

//CLabel
CLabel::CLabel(int id, const CRect &relPos, const string &label ) :CWindowElement(id, relPos), m_Label(label) {
}

void CLabel::Print(ostream & os, bool last, bool item) const {
    CRect pos = CRect(m_Parent->m_W * m_RelPos.m_X+ m_Parent->m_X, m_Parent->m_H * m_RelPos.m_Y + m_Parent->m_Y, m_Parent->m_W * m_RelPos.m_W, m_Parent->m_H * m_RelPos.m_H ); 
    os << "[" << m_Id <<"] Label \""<< m_Label << "\" " << pos << endl;
}

//CComboBox
CComboBox::CComboBox (int id, const CRect &relPos ) : CWindowElement(id, relPos), m_Position(0) {
}

CComboBox &CComboBox::Add(string item) {
  m_List.push_back(item);
  return *this;
}

int CComboBox::GetSelected( ) const {
  return m_Position;
}

void CComboBox::SetSelected(int index) {
  m_Position = index;
}

void CComboBox::Print(ostream & os, bool last, bool item) const {
  CRect pos = CRect(m_Parent->m_W * m_RelPos.m_X+ m_Parent->m_X, m_Parent->m_H * m_RelPos.m_Y + m_Parent->m_Y, m_Parent->m_W * m_RelPos.m_W, m_Parent->m_H * m_RelPos.m_H ); 
  os << "[" << m_Id <<"] ComboBox " << pos << endl;
  string start = "|  +-";
  if (last == true)
    start = "   +-";
  else if (item == true)
    start = "+-";
     
  for(unsigned int i=0; i < m_List.size();++i ){
    if(m_Position == i)
      os << start <<">" <<m_List[i] <<"<" << endl;
    else 
      os << start<<" " <<m_List[i]<< endl;  
  }
}

// output operators

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  
  CWindow a ( "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . Add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . Add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . Add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . Add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . Add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . Add ( "Karate" ) . Add ( "Judo" ) . Add ( "Box" ) . Add ( "Progtest" ) );
  assert ( toString ( a ) ==
    "Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  CWindow b = a;
  assert ( toString ( *b . Search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . Search ( 20 ) ) . GetSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . Search ( 20 ) ) . SetSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . Search ( 11 ) ) . GetValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . Search ( 11 ) ) . SetValue ( "chucknorris@fit.cvut.cz" );
  b . Add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . Add ( "PA2" ) . Add ( "OSY" ) . Add ( "Both" ) );
  assert ( toString ( b ) ==
    "Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (70,250,480,48)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  assert ( toString ( a ) ==
    "Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  b . SetPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
    a = b;
  return 0;
}
#endif /* __PROGTEST__ */
