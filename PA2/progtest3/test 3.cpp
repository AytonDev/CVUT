#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
class InvalidDateException
{
};
#endif /* __PROGTEST__ */

// uncomment if your code implements the overloaded suffix operators
// #define TEST_LITERALS

class YMDPicker {
  public:
  int m_year, m_month, m_days;
  YMDPicker(int year, int month, int days){
    m_year = year; m_month = month; m_days = days;
  }

  YMDPicker(int value, int option){
    m_year=0;
    m_month=0;
    m_days=0;
    if(option == 0){
      m_year = value;
    }
    else if(option == 1){
      m_month = value;
    }
    else if(option == 2){
      m_days = value;
    }
  }

  YMDPicker operator - ()const {
    if(m_year !=0) 
      return YMDPicker(m_year * -1, 0);
  
    else if(m_month !=0)
     return YMDPicker( m_month * -1,1);
    
    else 
      return YMDPicker( m_days * -1,2);
    
  }

  YMDPicker operator - ( const YMDPicker &input)const;
  YMDPicker operator + (const YMDPicker &input)const;
      
};
vector <YMDPicker> dates;


YMDPicker YMDPicker::operator+(const YMDPicker &input) const {
    dates.push_back(input);
    return YMDPicker(m_year, m_month, m_days);
}

YMDPicker YMDPicker::operator-(const YMDPicker &input) const {
  int year =input.m_year; int month = input.m_month; int days = input.m_days;
    year  *= -1;
    month  *= -1;
    days  *= -1;
    dates.push_back(YMDPicker(year,month, days));
    return YMDPicker(m_year, m_month, m_days);
}



class CDate
{
  public:
    // constructor
    CDate(int year, int month, int days){
       if(year < 1600 || month > 12 || month < 1 || days > 31 ||  days < 1){
            throw InvalidDateException();
        }
        m_leapYear =leapYearCalculator(year);
        
        if( m_leapYear== false &&month == 2 &&  days >= 29){
             throw InvalidDateException();
        }
        
        m_year = year;
        m_month = month;
        m_days = days;
        checkDate();
    }
    // operator(s) +
    CDate operator + (const YMDPicker &input)const  {
      if(input.m_year != 0){
        return (CDate(m_year+input.m_year, m_month, m_days));
      }

      else if(input.m_month !=0){
        int months =m_month + input.m_month;
        if(months <=12 && months > 0) return (CDate(m_year, months, m_days));
        if(months < 0) {
          months *= -1;

          int years = months/12+1;
          months = 12 - (months % 12);
           return (CDate(m_year-years, months, m_days));
        }
        else {
          int year = (months-1)/12;
          months = months %12;
          if(months==0) months=12;
          return (CDate(m_year+year, months, m_days));
        }
      }
      else if(input.m_days !=0){
        int days = DateToDays(m_year, m_month, m_days);
        int year=0, month=0, day =0; 
        DaysToDate(days+input.m_days,year, month, day);
        return  CDate(year, month, day);
      }else {
        
        return CDate(m_year, m_month, m_days);
      }

    }
    int friend operator + (const CDate &input1, const CDate &input2){
      int date1 = ToDays(input1.m_year, input1.m_month, input1.m_days, input1.m_leapYear);
      int date2 = ToDays(input2.m_year, input2.m_month, input2.m_days, input2.m_leapYear);
      return date1 + date2;
    }
    // operator(s) -
    CDate operator - (const YMDPicker &input) const {
      if(input.m_year != 0){
        return (CDate(m_year-input.m_year, m_month, m_days));
      }
      else if(input.m_month !=0){
        int value = input.m_month;
        value *= -1;
        int outMonth = m_month , outYear = m_year;
        if(outMonth - value > 0){
           if(value > 0){
            if(value + m_month <= 12) outMonth +=value;
            else{ 
              int years = (value+ outMonth -1)/12;
              outMonth = (value + outMonth) %12;
              if(outMonth == 0)
                outMonth = 12;
              outYear += years;
            }

          }else{
           
            if(outMonth + value > 0) outMonth +=value;
            else if(outMonth + value == 0) {
              outMonth =12;
              outYear--;
            }
            else {
              int years = 1;
              int moths = value + outMonth;
              outMonth = 12;
              years +=  moths / -12;
              moths =  moths % 12;
              outYear-=years;
              outMonth += moths;
            }
          }
          return (CDate(outYear, outMonth, m_days));
        }else{
            int months =m_month + value;
            int year=0;
            if(outMonth + value <= 12) return (CDate(outYear+year, months, m_days));
            else {
              
              year = (months-1)/12;
              months = months %12;
              if(months==0) months=12;
            }
            return (CDate(outYear+year, months, m_days));
          }
        
      }
      else if(input.m_days !=0){
        int days = DateToDays(m_year, m_month, m_days) - input.m_days;
        int year=0, month=0, day =0;
        DaysToDate(days,year, month, day);
        return  CDate(year, month, day);
      }
       return CDate(m_year, m_month, m_days);


    }
    int friend operator - (const CDate &input1, const CDate &input2){

      int date1 = ToDays(input1.m_year, input1.m_month, input1.m_days, 0);
      int date2 = ToDays(input2.m_year, input2.m_month, input2.m_days, 0);
      
      return date1 - date2;
    }
    // operator ==
    friend bool operator == (const CDate input1, const CDate input2 )  {
      if(input1.m_year != input2.m_year) return false;
      if(input1.m_month != input2.m_month) return false;
      if(input1.m_days != input2.m_days) return false;
      return true;
    }
    // operator !=
    friend bool operator != (const CDate input1, const CDate input2 )  {
     
      if(input1.m_year != input2.m_year) return true;
      if(input1.m_month != input2.m_month) return true;
      if(input1.m_days != input2.m_days) return true;
      return false;
    }
    // operator <
    friend bool operator < (const CDate input1, const CDate input2 )  {
      if(input1.m_year > input2.m_year) return false;
      if(input1.m_month > input2.m_month) return false;
      if(input1.m_month == input2.m_month){
         if(input1.m_days >=input2.m_days) return false;
         return true;
      }
      if(input1.m_days >=input2.m_days) return false;
      return true;
    }
    // operator(s) +=
    CDate operator += (const YMDPicker &input){
          if(input.m_year != 0){
            m_year +=input.m_year;
            checkDate();
          }

        else if(input.m_month !=0){
          if(input.m_month > 0){
            if(input.m_month + m_month <= 12) m_month +=input.m_month;
            else{ 
              int years = (input.m_month + m_month -1)/12;
              m_month = (input.m_month + m_month) %12;
              if(m_month == 0)
                m_month = 12;
              m_year += years;
            }

          }else{
            if(m_month + input.m_month > 0) m_month +=input.m_month;
            else if(m_month + input.m_month == 0) {
              m_month =12;
              m_year--;
            }
            else {
              int years = 1;
              int moths = input.m_month + m_month;
              m_month = 12;
              years +=  moths / -12;
              moths =  moths % 12;
            
              m_year-=years;
              m_month += moths;
            }
          }
          checkDate();
        }
        else if(input.m_days !=0){
          int days = DateToDays(m_year, m_month, m_days);
           m_year=0, m_month=0, m_days =0; 
          DaysToDate(days+input.m_days,m_year, m_month, m_days);
          checkDate();
        }
    
      for(unsigned int i=0; i < dates.size(); ++i){
            if(dates[i].m_year != 0){
              m_year += dates[i].m_year;
              checkDate();
            }
            else if(dates[i].m_month != 0){
          if(dates[i].m_month > 0){
            if(dates[i].m_month + m_month <= 12) m_month +=dates[i].m_month;
            else{ 
              int years = (dates[i].m_month+ m_month -1)/12;
              m_month = (dates[i].m_month+ m_month) %12;
              if(m_month == 0)
                m_month = 12;
              m_year += years;
            }

          }else{
            if(m_month + dates[i].m_month > 0) m_month +=dates[i].m_month;
            else if(m_month + dates[i].m_month == 0) {
              m_month =12;
              m_year--;
            }
            else {
              int years = 1;
              int moths = dates[i].m_month + m_month;
              m_month = 12;
              years +=  moths / -12;
              moths =  moths % 12;
            
              m_year-=years;
              m_month += moths;
             }
            }
              checkDate();
            }
            else if(dates[i].m_days !=0){
              int days = DateToDays(m_year, m_month, m_days);
              m_year=0, m_month=0, m_days =0; 
              DaysToDate(days+dates[i].m_days,m_year, m_month, m_days);
              checkDate();
            }
        }
        dates.clear();
        return CDate(m_year, m_month, m_days);
    }
    // operator <<
    friend ostringstream & operator << (ostringstream  &output, const CDate date) {
        output <<  date.m_year << "-"  << std::setfill ('0') << setw(2) <<  date.m_month << "-" << std::setfill ('0') << setw(2) << date.m_days;
        return output;
    }
    
  private:
    // todo
    int m_year, m_month, m_days;
    bool m_leapYear;
    bool leapYearCalculator(int year)const  {
         if(((year % 4 == 0) && (year % 100 != 0)) ||
        ((year % 400 == 0) && (year % 4000 !=0))){
            return true;
        }else {
            return false;
        }
    }
    int DateToDays(int year, int month, int day) const {
        int days=0;
        while(true) {   
            if(year >= 4000){              
                days += 1460969 ;
                year -=4000;
            }
            else if(year >= 400){
                days += 146097 ;
                year -=400;
            }
            else if(year >=100) {               
                days +=36524;
                year-=100;
            }else if(year >= 4){              
                days += 1461;
                year-=4;
            }else if(year >=1){                
                 days+=365;
                 year -= 1;
            }else {                
                break;
            }
        }
        if(month == 1){      
            return days +=day;
        }
        else if(month == 2){
            return days += 31+day;
        }
        else if(month == 3){
            return days += 59+day + m_leapYear;
        }  
        else if(month == 4){  
            return  days += 90+day+m_leapYear;
        }  
        else if(month == 5){    
            return days += 120+day+m_leapYear;
        }  
        else if(month == 6){
            return days += 151+day+m_leapYear;
        }
        else if(month == 7){  
            return days += 181+day+m_leapYear;
        }  
        else if(month == 8){  
            return days += 212+day+m_leapYear;
        } 
        else if(month == 9){
            return  days += 243+day+m_leapYear;
        }  
        else if(month == 10){
            return  days += 273+day+m_leapYear;
        }   
        else if(month == 11){      
            return  days += 304+day+m_leapYear;
        }    
        else if(month == 12){
            return  days += 334+day+m_leapYear;
        } 
        return days;   
    }
    int static ToDays(int year, int month, int day, int leapYear)  {
        int days=0;
        while(true) {   
            if(year >= 4000){              
                days += 1460969 ;
                year -=4000;
            }
            else if(year >= 400){
                days += 146097 ;
                year -=400;
            }
            else if(year >=100) {               
                days +=36524;
                year-=100;
            }else if(year >= 4){              
                days += 1461;
                year-=4;
            }else if(year >=1){                
                 days+=365;
                 year -= 1;
            }else {                
                break;
            }
        }
        
        if(month == 1){      
            return days ;//+=day;
        }
        else if(month == 2){
            return days += 31+day;
        }
        else if(month == 3){
            return days += 59+day + leapYear;
        }  
        else if(month == 4){  
            return  days += 90+day+leapYear;
        }  
        else if(month == 5){    
            return days += 120+day+leapYear;
        }  
        else if(month == 6){
            return days += 151+day+leapYear;
        }
        else if(month == 7){  
            return days += 181+day+leapYear;
        }  
        else if(month == 8){  
            return days += 212+day+leapYear;
        } 
        else if(month == 9){
            return  days += 243+day+leapYear;
        }  
        else if(month == 10){
            return  days += 273+day+leapYear;
        }   
        else if(month == 11){      
            return  days += 304+day+leapYear;
        }    
        else if(month == 12){
            return  days += 334+day+leapYear;
        }  
        return days;  
    }
    void DaysToDate(int days, int &year, int &month, int &day) const{
        
        while(true) {   
            if(days >= 1460969){              
                days -= 1460969 ;
                year +=4000;
            }
            else if(days >= 146097){
                days -= 146097 ;
                year +=400;
            }
            else if(days >=36524) {               
                days -=36524;
                year+=100;
            }else if(days >= 1461){              
                days -= 1461;
                year+=4;
            }else if(days >=365){                
                 days-=365;
                 year+= 1;
            }else {                
                break;
            }
        }
      
        
        int leapYear = leapYearCalculator(year);
       
        if(days == 0){
            month=1;
            day=1;
            return;
        }
        else if(days <= 31){
            month =1;
        }
        else if(days <= 59+leapYear){
            month = 2;
            days -= 31;
        }  
        else if(days <=90+leapYear){
            month = 3;
            days -= 59+leapYear;
        } 
        else if(days<=120+leapYear){
            month = 4;
            days -= 90+leapYear;
        }   
        else if(days<=151+leapYear){
            month = 5;
            days -= 120+leapYear;
        } 
        else if(days<=181+leapYear){
            month = 6;
            days -= 151+leapYear;
        }  
        else if(days<=212+leapYear){
            month = 7;
            days -= 181+leapYear;
        }   
        else if(days<=243+leapYear){
            month = 8;
            days -= 212+leapYear;
        }   
        else if(days<=273+leapYear){
            month = 9;
            days -= 243+leapYear;
        }  
         else if(days<=304+leapYear){
            month = 10;
            days -= 273+leapYear;
         }   
        else if(days<=334+leapYear){
            month = 11;
            days -= 304+leapYear;
        } 
        else if(days<=365+leapYear){
            month = 12;
            days -= 334+leapYear;
        }
        day = days;
        
        
    }
    bool checkDate(){
      m_leapYear = leapYearCalculator(m_year);
      if(m_days <1 || m_month < 1 || m_year < 1600){
         throw InvalidDateException();
        return false;
      }
      if(m_leapYear ==false && m_month == 2 && m_days > 28){
        throw InvalidDateException();
        return false;
      }
      if(m_days > 31){
        throw InvalidDateException();
        return false;
      }

      
      if((m_month == 4 || m_month == 6 || m_month == 9 || m_month == 11) && m_days > 30) {
        throw InvalidDateException();
        return false;
      }
      else 
        return true;
    }
};

YMDPicker Year(int year){
  return YMDPicker(year, 0);
}

YMDPicker Month(int month){
  return YMDPicker(month,1);
}

YMDPicker Day(int day){
  return YMDPicker(day,2);
}

#ifndef __PROGTEST__
string             toString                                ( const CDate     & x )
{
  ostringstream oss;
  oss << x;
  //cout << oss . str () << endl;
  return oss . str ();
}

int                main                                    ( void )
{
  CDate tmp ( 2000, 1, 1 );
  
  assert ( toString ( CDate ( 2000,2, 29 ) Month (-12 ) )  == "2018-04-15" );
 // tmp = CDate ( 2000, 5, 1 );
 // tmp +=    Month ( 18) ;
 // assert ( toString ( tmp ) == "2000-01-11" );


  assert ( toString ( CDate ( 2000, 1, 1 ) )  == "2000-01-01" );
  assert ( toString ( CDate ( 2500, 12, 21 ) )  == "2500-12-21" );
  assert ( toString ( CDate ( 1685, 7, 11 ) )  == "1685-07-11" );
  try
  {
    tmp = CDate ( 1900, 2, 29 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2000, 2, 29 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 2004, 2, 29 ) )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 4000, 2, 29 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 1 ) )  == "2019-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 7 ) )  == "2025-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 73 ) )  == "2091-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 3 ) )  == "2018-06-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 1 ) )  == "2018-04-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 285 ) )  == "2041-12-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 1 ) )  == "2018-03-16" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 12 ) )  == "2018-03-27" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 1234567 ) )  == "5398-05-02" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 3 ) + Year ( 2 ) + Month ( 3 ) + Day ( 5 ) + Month ( 11 ) )  == "2021-05-23" );
  try
  {
    tmp = CDate ( 2000, 2, 29 ) + Year ( 300 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 3, 30 ) + Month ( 11 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2001, 2, 20 ) + Day ( 9 ) )  == "2001-03-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + Day ( 28 ) + Month ( 1 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 1999, 1, 1 ) + Year ( 1 ) + Day ( 28 ) + Month ( 1 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 2001, 1, 1 ) + Day ( 1095 ) + Day ( 28 ) + Month ( 1 ) )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 2001, 1, 1 ) + Day ( 28 ) + Month ( 1 ) + Day ( 1095 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + Day ( 28 ) + Month ( 1 ) + Year ( 1 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  assert ( toString ( tmp   - Year ( 5 ) + Month ( 2 ) )  == "1995-03-01" );
  assert ( toString ( tmp ) == "2000-01-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 1 ) - Month ( 3 ) - Day ( 10 ) )  == "1998-09-21" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 2 ) - Month ( -3 ) + Day ( -10 ) )  == "1998-03-22" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + Day ( 59 ) - Month ( 1 ) - Year ( 2 ) )  == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + Day ( 59 ) - Year ( 2 ) - Month ( 1 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
 // tmp +=    Day ( 10 ) ;
  //assert ( toString ( tmp ) == "2000-01-11" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 2 ) - Month ( -3 ) + Day ( 10 )) == "1998-04-11" );
  tmp +=  - Year ( 2 ) - Month ( -3 ) + Day ( -10);
  assert ( toString ( tmp ) == "1998-03-22" );
  tmp = CDate ( 2000, 1, 1 );
  tmp += Day ( 59 ) - Month ( 1 ) - Year ( 2 );
  assert ( toString ( tmp ) == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 );
    tmp += Day ( 59 ) - Year ( 2 ) - Month ( 1 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2018, 3, 15 ) + Day ( -3 );
  assert ( toString ( tmp ) == "2018-03-12" );
  assert ( !( CDate ( 2018, 3, 15 ) == CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) != CDate ( 2000, 1, 1 ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) == CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) != CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + Day ( 1 ) == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + Day ( 1 ) != CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) + Day ( 1 ) < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + Day ( -1 ) == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + Day ( -1 ) != CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) + Day ( -1 ) < CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) - CDate ( 2000, 1, 1 ) == 6648 );
  assert ( CDate ( 2000, 1, 1 ) - CDate ( 2018, 3, 15 ) == -6648 );
  assert ( CDate ( 2017, 9, 15 ) + Year ( 2 )  - CDate ( 2000, 1, 1 ) == 7197 );
  assert ( CDate ( 5398, 5, 2 ) - CDate ( 2018, 3, 15 ) == 1234567 );
#ifdef TEST_LITERALS
  assert ( toString ( CDate ( 2000, 1, 1 ) )  == "2000-01-01" );
  assert ( toString ( CDate ( 2500, 12, 21 ) )  == "2500-12-21" );
  assert ( toString ( CDate ( 1685, 7, 11 ) )  == "1685-07-11" );
  try
  {
    tmp = CDate ( 1900, 2, 29 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2000, 2, 29 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 2004, 2, 29 ) )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 4000, 2, 29 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_year )  == "2019-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 7_years )  == "2025-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 73_years )  == "2091-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 3_months )  == "2018-06-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_month )  == "2018-04-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 285_months )  == "2041-12-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_day )  == "2018-03-16" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 12_days )  == "2018-03-27" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1234567_days )  == "5398-05-02" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 3_days + 2_years + 3_months + 5_days + 11_months )  == "2021-05-23" );
  try
  {
    tmp = CDate ( 2000, 2, 29 ) + 300_years;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 3, 30 ) + 11_months;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2001, 2, 20 ) + 9_days )  == "2001-03-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + 28_days + 1_month )  == "2000-02-29" );
  assert ( toString ( CDate ( 1999, 1, 1 ) + 1_year + 28_days + 1_month )  == "2000-02-29" );
  assert ( toString ( CDate ( 2001, 1, 1 ) + 1095_days + 28_days + 1_month )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 2001, 1, 1 ) + 28_days + 1_month + 1095_days;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + 28_days + 1_month + 1_year;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  assert ( toString ( tmp - 5_years + 2_months )  == "1995-03-01" );
  assert ( toString ( tmp ) == "2000-01-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - 1_year - 3_months - 10_days )  == "1998-09-21" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - 2_years - ( -3_months ) + ( -10_days ) )  == "1998-03-22" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + 59_days - 1_month - 2_years )  == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + 59_days - 2_years - 1_month;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  tmp +=  - 2_years - ( -3_months ) + ( -10_days );
  assert ( toString ( tmp ) == "1998-03-22" );
  tmp = CDate ( 2000, 1, 1 );
  tmp += 59_days - 1_month - 2_years;
  assert ( toString ( tmp ) == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 );
    tmp += 59_days - 2_years - 1_month;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2018, 3, 15 ) + ( -3_days );
  assert ( toString ( tmp ) == "2018-03-12" );
  assert ( !( CDate ( 2018, 3, 15 ) == CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) != CDate ( 2000, 1, 1 ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) == CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) != CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + 1_day == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + 1_day != CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) + 1_day < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + ( -1_day ) == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + ( -1_day ) != CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) + ( -1_day ) < CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) - CDate ( 2000, 1, 1 ) == 6648 );
  assert ( CDate ( 2000, 1, 1 ) - CDate ( 2018, 3, 15 ) == -6648 );
  assert ( CDate ( 2018, 3, 15 ) + 3_years + ( -18_months ) - CDate ( 2000, 1, 1 ) == 7197 );
  assert ( CDate ( 5398, 5, 2 ) - CDate ( 2018, 3, 15 ) == 1234567 );
  ostringstream oss;
  oss << setfill ( 'x' ) << left << hex << CDate ( 2000, 1, 1 ) << ' ' << setw ( 10 ) << 65536;
  assert ( oss . str () == "2000-01-01 10000xxxxx" );
#endif /* TEST_LITERALS */
  return 0;
}
#endif /* __PROGTEST__ */
