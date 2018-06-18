#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

// enable only if your implementation supports Add ( ) with more than three parameters
#define MULTIPLE_STOPS


template <typename _T, typename _E>
class CAccess
{
 

  public:
   // default constructor
   CAccess(){}
   // destructor
   ~CAccess(){}
   
  CAccess & Add( const _E & train,const  _T from, const _T to){
    graph.insert( pair <_T, TLine> (from, TLine{to, train}));
    graph.insert( pair <_T, TLine> (to, TLine{from, train}));
    return *this;
  }

  CAccess & Add( const _E & train,const  _T from, const _T to,  _T args...){
    graph.insert( pair <_T, TLine> (from, TLine{to, train}));
    graph.insert( pair <_T, TLine> (to, TLine{from, train}));
    return *this;
  }
   // Find (with optional max and filter)

  map<_T,int> Find(_T from ) const {
    std::queue < std::pair < _T, int > > queue;
    std::set < _T > visited;
    std::map <_T, int> results;

    
    auto result = graph.equal_range(from);
    
    queue.push ( std::make_pair ( from , 0) );
    results.insert(std::make_pair ( from, 0));
    visited.insert (from);

   // cout << result.first->second.m_Destination << result.second->second.m_Destination<< from << endl;
    
    if(result.first == result.second){
      std::ostringstream s;
      s << "unknown " << from;
      throw std::invalid_argument(s.str());
    }
  
    while(! queue.empty()) {
      _T cnode = queue.front ( ).first;
      int cnumber = queue.front().second;
      result = graph.equal_range(cnode);
      queue.pop ( );   
      
      for ( auto it = result.first; it != result.second; it++) {
        //std::cout << it->second.m_Destination << std::endl;
        if(visited.count(it->second.m_Destination) == 0){
         // counter++;
            visited.insert(it->second.m_Destination);
            results.insert(std::make_pair ( it->second.m_Destination, cnumber+1 ));
            queue.push(std::make_pair ( it->second.m_Destination, cnumber+1 ));
        }
      }
    }
    return results;
  }

  map<_T,int> Find(_T from, int max ) const {
    std::queue < std::pair < _T, int > > queue;
    std::set < _T > visited;
    std::map <_T, int> results;

   
    auto result = graph.equal_range(from);
    
    if(max >= 0){
      queue.push ( std::make_pair ( from , 0) );
      results.insert(std::make_pair ( from, 0));
      visited.insert (from);
    }

   // cout << result.first->second.m_Destination << result.second->second.m_Destination<< from << endl;
    
    if(result.first == result.second){
      std::ostringstream s;
      s << "unknown " << from;
      throw std::invalid_argument(s.str());
    }

    while(! queue.empty()) {
      _T cnode = queue.front ( ).first;
      int cnumber = queue.front().second;
      result = graph.equal_range(cnode);
      queue.pop ( );   
      if(cnumber >= max){
        return results;
      }
      
      for ( auto it = result.first; it != result.second; it++) {
        //std::cout << it->second.m_Destination << std::endl;
        if(visited.count(it->second.m_Destination) == 0){
         // counter++;
            visited.insert(it->second.m_Destination);
            results.insert(std::make_pair ( it->second.m_Destination, cnumber+1 ));
            queue.push(std::make_pair ( it->second.m_Destination, cnumber+1 ));
        }
      }
    }
    return results;
  }

  map<_T,int> Find(_T from, int max, function<bool(_E)> const & func ) const  {
    std::queue < std::pair < _T, int > > queue;
    std::set < _T > visited;
    std::map <_T, int> results;
   
    auto result = graph.equal_range(from);
    
    if(max >= 0){
      queue.push ( std::make_pair ( from , 0) );
      results.insert(std::make_pair ( from, 0));
      visited.insert (from);
    }
   
   // cout << result.first->second.m_Destination << result.second->second.m_Destination<< from << endl;
    
    if(result.first == result.second){
      std::stringstream s;
      s << "unknown " << from;
      throw std::invalid_argument(s.str());
    }
   
    while(! queue.empty()) {
    
      _T cnode = queue.front ( ).first;
      int cnumber = queue.front().second;
      result = graph.equal_range(cnode);
      queue.pop ( );  
      if(cnumber >= max){
        return results;
      } 
      
      for ( auto it = result.first; it != result.second; it++) {
        //std::cout << it->second.m_Destination << std::endl;
        if(visited.count(it->second.m_Destination) == 0 && func(it->second.m_Train)){
         // counter++;
            visited.insert(it->second.m_Destination);
            results.insert(std::make_pair ( it->second.m_Destination, cnumber+1 ));
            queue.push(std::make_pair ( it->second.m_Destination, cnumber+1 ));
        }
      }
    }
    return results;
  }

  private:
  struct TLine {
    _T m_Destination;
    _E m_Train;
    bool operator < (const TLine& pt) const 
    {
      return  m_Destination < pt.m_Destination;
    }
  };
  
  multimap <_T, TLine> graph;

  
};

#ifndef __PROGTEST__
//=================================================================================================
class CTrain
{
  public:
  CTrain ( const string & company, int speed ): m_Company ( company ),m_Speed ( speed ) 
  { 
  }
  //---------------------------------------------------------------------------------------------
  string m_Company;
  int m_Speed; 
};
//=================================================================================================
class TrainFilterCompany
{
  public:
  TrainFilterCompany( const set<string> & companies ) : m_Companies ( companies ) 
  { 
  }
  //---------------------------------------------------------------------------------------------
  bool operator () ( const CTrain & train ) const
  { 
    return m_Companies . find ( train . m_Company ) != m_Companies . end (); 
  }

  //---------------------------------------------------------------------------------------------
  private:  
  set <string> m_Companies;    
};
//=================================================================================================
class TrainFilterSpeed
{
  public:
  TrainFilterSpeed ( int min, int max ) : m_Min ( min ), m_Max ( max ) 
  { 

  }
  //---------------------------------------------------------------------------------------------
  bool operator () ( const CTrain    & train ) const
  { 
    return train . m_Speed >= m_Min && train . m_Speed <= m_Max; 
  }
  //---------------------------------------------------------------------------------------------
  private:  
  int m_Min;
  int m_Max; 
};
//=================================================================================================
bool NurSchnellzug ( const CTrain& zug )
{
  return ( zug . m_Company == "OBB" || zug . m_Company == "DB" ) && zug . m_Speed > 100;
}
//=================================================================================================
int                          main                          ( void )
{
  CAccess<string,CTrain> lines;
  lines . Add ( CTrain (   "DB", 120 ), "Berlin", "Prague"   )
        . Add ( CTrain (   "CD",  80 ), "Berlin", "Prague"   )
        . Add ( CTrain (   "DB", 160 ), "Berlin", "Dresden"  )
        . Add ( CTrain (   "DB", 160 ), "Dresden", "Munchen" )
        . Add ( CTrain (   "CD",  90 ), "Munchen", "Prague"  )
        . Add ( CTrain (   "DB", 200 ), "Munchen", "Linz"    )
        . Add ( CTrain (  "OBB",  90 ), "Munchen", "Linz"    )
        . Add ( CTrain (   "CD",  50 ), "Linz", "Prague"     )
        . Add ( CTrain (   "CD", 100 ), "Prague", "Wien"     )
        . Add ( CTrain (  "OBB", 160 ), "Linz", "Wien"       )
        . Add ( CTrain ( "SNCF", 300 ), "Paris", "Marseille" )
        . Add ( CTrain ( "SNCF", 250 ), "Paris", "Dresden"   )
        . Add ( CTrain ( "SNCF", 200 ), "London", "Calais"   );
  assert ( lines . Find ( "Berlin" ) == (map<string,int>
  {
    make_pair ( "Berlin", 0 ),
    make_pair ( "Dresden", 1 ),
    make_pair ( "Linz", 2 ),
    make_pair ( "Marseille", 3 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Paris", 2 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Wien", 2 )
  }) );
  assert ( lines . Find ( "London" ) == (map<string,int>
  {
    make_pair ( "Calais", 1 ),
    make_pair ( "London", 0 )
  }) );
  assert ( lines . Find ( "Wien" ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Linz", 1 ),
    make_pair ( "Marseille", 5 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Paris", 4 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Wien", 0 )
  }) );
  assert ( lines . Find ( "Wien", 3 ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Linz", 1 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Wien", 0 )
  }) );
  assert ( lines . Find ( "Wien", 5, NurSchnellzug ) == (map<string,int>
  {
    make_pair ( "Berlin", 4 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Linz", 1 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Prague", 5 ),
    make_pair ( "Wien", 0 )
  }) );
  assert ( lines . Find ( "Prague", 3,  TrainFilterCompany ( { "CD", "DB" } ) ) == (map<string,int>
  {
    make_pair ( "Berlin", 1 ),
    make_pair ( "Dresden", 2 ),
    make_pair ( "Linz", 1 ),
    make_pair ( "Munchen", 1 ),
    make_pair ( "Prague", 0 ),
    make_pair ( "Wien", 1 )
  }) );
  assert ( lines . Find ( "Munchen", 4, TrainFilterSpeed ( 160, 250 ) ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 1 ),
    make_pair ( "Linz", 1 ),
    make_pair ( "Munchen", 0 ),
    make_pair ( "Paris", 2 ),
    make_pair ( "Wien", 2 )
  }) );
  assert ( lines . Find ( "Munchen", 4, [] ( const CTrain & x ) { return x . m_Company == "CD"; } ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Linz", 2 ),
    make_pair ( "Munchen", 0 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Wien", 2 )
  }) );
  assert ( lines . Find ( "London", 20, [] ( const CTrain & x ) { return x . m_Company == "CD"; } ) == (map<string,int>
  {
    make_pair ( "London", 0 )
  }) );
  try
  {
     auto res = lines . Find ( "Salzburg" );
     assert ( "No exception thrown" == NULL );
  }
  catch ( const invalid_argument & e )
  {
    assert ( string ( e . what () ) == "unknown Salzburg" );
  }
#ifdef MULTIPLE_STOPS
  lines . Add ( CTrain ( "RZD",  80 ), "Prague", "Kiev", "Moscow", "Omsk", "Irkutsk", "Vladivostok" );
  lines . Add ( CTrain ( "LAV", 160 ), "Malaga", "Cordoba", "Toledo", "Madrid", "Zaragoza", "Tarragona", "Barcelona", "Montpellier", "Marseille" );
  assert ( lines . Find ( "Madrid" ) == (map<string,int>
  {
    make_pair ( "Barcelona", 1 ),
    make_pair ( "Berlin", 4 ),
    make_pair ( "Cordoba", 1 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 6 ),
    make_pair ( "Kiev", 6 ),
    make_pair ( "Linz", 5 ),
    make_pair ( "Madrid", 0 ),
    make_pair ( "Malaga", 1 ),
    make_pair ( "Marseille", 1 ),
    make_pair ( "Montpellier", 1 ),
    make_pair ( "Moscow", 6 ),
    make_pair ( "Munchen", 4 ),
    make_pair ( "Omsk", 6 ),
    make_pair ( "Paris", 2 ),
    make_pair ( "Prague", 5 ),
    make_pair ( "Tarragona", 1 ),
    make_pair ( "Toledo", 1 ),
    make_pair ( "Vladivostok", 6 ),
    make_pair ( "Wien", 6 ),
    make_pair ( "Zaragoza", 1 )
  }) );
  assert ( lines . Find ( "Omsk", 4 ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 1 ),
    make_pair ( "Kiev", 1 ),
    make_pair ( "Linz", 2 ),
    make_pair ( "Moscow", 1 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Omsk", 0 ),
    make_pair ( "Paris", 4 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Vladivostok", 1 ),
    make_pair ( "Wien", 2 )
  }) );
  assert ( lines . Find ( "Moscow", 10, TrainFilterCompany ( { "RZD", "DB" } ) ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 1 ),
    make_pair ( "Kiev", 1 ),
    make_pair ( "Linz", 5 ),
    make_pair ( "Moscow", 0 ),
    make_pair ( "Munchen", 4 ),
    make_pair ( "Omsk", 1 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Vladivostok", 1 )
  }) );
#endif /* MULTIPLE_STOPS */
  return 0;
}
#endif  /* __PROGTEST__ */
