#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

struct  Employee {
    unsigned int salary;
    string email;
    string name;
    string surname;

};

class CPersonalAgenda
{
private:
     int count=0;
    vector<Employee> employees;
    vector< unsigned int> sortedSalary;
    vector<int> sortedEmail;
    vector<int> sortedName;
   

    bool compareBySalary(unsigned const int i1,unsigned const int i2) const
    {
        return  employees.at(i1).salary < employees.at(i2).salary;
    }

    bool compareByMail(unsigned const int i1,unsigned const int i2)
    {
        return employees.at(i1).email < employees.at(i2).email;

    }

    bool compareByName(unsigned const int i1, unsigned const int i2) {
        if(employees.at(i1).surname == employees.at(i2).surname){
            return employees.at(i1).name < employees.at(i2).name;
        }else
            return  employees.at(i1).surname < employees.at(i2).surname;
    }

    int binarySearchSalary(const vector<int>  &arr,const vector<Employee> &employees, int high, int low, unsigned const int x) const{
        if (high >=low) {

            int mid = low + (high - low)/2;  /*low + (high - low)/2;*/
            if (x == employees[arr[mid]].salary)
                return mid;
            if (x < employees[arr[mid]].salary)
                return binarySearchSalary(arr, employees, (mid - 1), low, x);
            return binarySearchSalary(arr, employees, high,(mid +1), x);
        }
        return -1;
    }

    int binarySearchSalaryRankLow(const vector< unsigned int>  &arr, int high, int low, unsigned const int x) const{
        if (high >=low) {

            int mid = low + (high - low)/2;  /*low + (high - low)/2;*/
            if (x == arr[mid]){
                if(mid == 0 || x != arr[mid-1])
                    return mid;
                else
                    return binarySearchSalaryRankLow(arr, (mid - 1), low, x);
            }

            if (x < arr[mid])
                return binarySearchSalaryRankLow(arr,  (mid - 1), low, x);
            return binarySearchSalaryRankLow(arr, high,(mid +1), x);
        }
        return -1;
    }
    int binarySearchSalaryRankHigh(const vector<unsigned int>  &arr, int high, int low, unsigned const int x) const{
        if (high >=low) {

            int mid = low + (high - low)/2;  /*low + (high - low)/2;*/
            unsigned int i= arr[mid];
            if(mid == count-1)
                return mid;
            if (x == i){
                if(x != i ){
                    return mid;
                }
                else
                    return binarySearchSalaryRankHigh(arr,high, (mid + 1), x);
            }

            if (x < i)
                return binarySearchSalaryRankHigh(arr, (mid - 1), low, x);
            return binarySearchSalaryRankHigh(arr, high,(mid +1), x);
        }
        return -1;
    }



    int binarySearchEmail(const vector <int> &arr,const  vector<Employee> &employees,  int high, int low , const string &x) const{
        if (high >=low){
            int mid = low + (high - low)/2;  /*low + (high - low)/2;*/
            // cout <<  employees[arr[mid]].email <<endl;
            if (x == employees[arr[mid]].email)
                return mid;
            if (x < employees[arr[mid]].email)
                return binarySearchEmail(arr, employees,(mid - 1), low, x);
            return binarySearchEmail(arr, employees,high,(mid +1), x);
        }
        return -1;

    }

    int binarySearchName(const vector <int> &arr,const vector<Employee> &employees,  int high, int low , const string &name, const string &surName) const{
        if (high >= low){
            int mid = low + (high - low)/2;  /*low + (high - low)/2;*/

            // cout <<  employees[arr[mid]].email <<endl;
            string s= employees[arr[mid]].surname;
            if (surName ==s){
                if(name == employees[arr[mid]].name)
                    return mid;
                if(name < employees[arr[mid]].name)
                    return binarySearchName(arr, employees,(mid - 1),low,name, surName);
                return binarySearchName(arr, employees,high,(mid +1),name ,surName);
            }
            if (surName < employees[arr[mid]].surname)
                return binarySearchName(arr, employees,(mid - 1), low,name, surName);
            return binarySearchName(arr, employees,high,(mid +1),name, surName);
        }
        return -1;

    }
public:
    CPersonalAgenda  ( void ){
        count=0;
    }
    ~CPersonalAgenda ( void ) {

    }


    bool Add ( const string    & name, const string    & surname, const string    & email, unsigned int      salary ){
        if( binarySearchEmail(sortedEmail,employees,count-1, 0, email) ==-1 &&
            binarySearchName(sortedName, employees, count-1,0, name,surname) == -1){

            Employee employee = { salary, email ,name, surname };
            employees.push_back(employee);


        }else {
            return false;
        }
        if(count ==0){
            sortedSalary.push_back(salary);
            sortedEmail.push_back(count);
            sortedName.push_back(count);
        }
        else {
            bool ename=true, eemail=true, esalary=true;
            for (int i = 0; i < count; ++i) {
              if (sortedSalary[i] > salary && esalary) {
                    sortedSalary.insert(sortedSalary.begin() + i, salary);
                    esalary= false;
                }else if(i == count-1 && esalary){
                    sortedSalary.push_back(salary);
                }

                if (employees[sortedEmail[i]].email > email && eemail) {
                    sortedEmail.insert(sortedEmail.begin() + i, count);
                    eemail=false;
                }else if(i == count-1  && eemail){
                    sortedEmail.push_back(count);
                }


                if (employees[sortedName[i]].surname == surname && ename) {
                    if (employees[sortedName[i]].name > name) {
                        sortedName.insert(sortedName.begin() + i, count);
                        ename=false;
                    }
                    else if(i == count-1 && ename){
                        sortedName.push_back(count);
                    }
                }else if (employees[sortedName[i]].surname > surname && ename){
                    sortedName.insert(sortedName.begin() + i, count);
                    ename=false;
                }
                else if(i == count-1 && ename){
                    sortedName.push_back(count);
                }

                if(!eemail  && !ename)
                    break;
            }
        }


        count++;
       // std::sort(sortedSalary.begin(), sortedSalary.end(), [this](unsigned int l, unsigned int r) {return compareBySalary(l, r); } );
      //  std::sort(sortedEmail.begin(), sortedEmail.end(), [this](unsigned int l, unsigned int r) {return compareByMail(l, r); } );
      //  std::sort(sortedName.begin(), sortedName.end(), [this](unsigned int l, unsigned int r) {return compareByName(l, r); } );

        return true;
    }

    bool Del ( const string    & name, const string    & surname ){
        int index=binarySearchName(sortedName, employees, count-1,0,name, surname), value;
        if(index  ==-1)
            return  false;
        employees.erase(employees.begin()+sortedName[index]);
        value= sortedName[index];

        bool sEnable = true;
        int eIndex, nIndex,sIndex;
        for(int i=0; i < count; ++i){

            if(sortedEmail[i] == value ){
               eIndex=i;

            }else if(sortedEmail[i] > value){
                sortedEmail[i]--;
            }

            if(sortedSalary[i] == employees[sortedName[index]].salary && sEnable){
                sIndex= i;
                sEnable= false;
            }

            if(sortedName[i] == value){
                nIndex=i;
            }else if(sortedName[i] > value)
                sortedName[i]--;

        }

        sortedName.erase(sortedName.begin()+nIndex);
        sortedEmail.erase(sortedEmail.begin()+eIndex);
        sortedSalary.erase(sortedSalary.begin()+sIndex);

        count--;
        return true;
    }

    void printN() {
        for (auto v : sortedName)
            cout << employees[v].name  << ' ' << employees[v].surname << endl;

    }
    void printS() {
        for (auto v : sortedSalary)
            cout << employees[v].name  << ' ' << employees[v].surname << " " << employees[v].salary << endl;

    }
    void printE() {
        for (auto v : sortedEmail)
            cout << employees[v].name  << ' ' << employees[v].surname << " " << employees[v].email << endl;

    }

    bool Del ( const string    & email ) {
        int index=binarySearchEmail(sortedEmail, employees, count-1, 0, email), value;
        if(index  == -1)
            return false;
        employees.erase(employees.begin()+sortedEmail[index]);
        value= sortedEmail[index];


        bool sEnable = true;
        int eIndex, nIndex,sIndex;
        for(int i=0; i < count; ++i){
            if(sortedName[i] == value ){
                nIndex=i;

            } else if(sortedName[i] > value)
                    sortedName[i]--;

            if(sortedSalary[i] == employees[sortedEmail[index]].salary && sEnable){
                sIndex= i;
                sEnable= false;
            }

            if(sortedEmail[i] == value){
                eIndex=i;
            }else if(sortedEmail[i] > value)
                sortedEmail[i]--;
        }

        sortedName.erase(sortedName.begin()+nIndex);
        sortedEmail.erase(sortedEmail.begin()+eIndex);
        sortedSalary.erase(sortedSalary.begin()+sIndex);
        count--;
        return  true;
    }

    bool ChangeName ( const string    & email, const string    & newName, const string    & newSurname ){
        int index=binarySearchEmail(sortedEmail, employees, count-1, 0, email);
        if(index  == -1 ||
           binarySearchName(sortedName, employees, count-1,0,newName, newSurname) !=-1)
            return false;
        employees[sortedEmail[index]].name = newName;
        employees[sortedEmail[index]].surname = newSurname;
        std::sort(sortedName.begin(), sortedName.end(), [this](unsigned int l, unsigned int r) {return compareByName(l, r); } );
        return true;
    }

    bool ChangeEmail ( const string    & name, const string    & surname, const string    & newEmail ) {
        int index = binarySearchName(sortedName,employees,count-1, 0, name, surname);
        if(index  ==-1 ||
           binarySearchEmail(sortedEmail, employees,count -1,0, newEmail) != -1 )
            return false;
        employees[sortedName[index]].email = newEmail;
        std::sort(sortedEmail.begin(), sortedEmail.end(), [this](unsigned int l, unsigned int r) {return compareByMail(l, r); } );
        return true;
    }

    bool SetSalary ( const string    & name, const string    & surname, unsigned int      salary ){
        int index =binarySearchName(sortedName,employees,count-1, 0, name, surname);
        if( index==-1 )
            return false;
        int oldSalary = employees[sortedName[index]].salary;
        employees[sortedName[index]].salary = salary;

        vector<unsigned int>::iterator lower=  lower_bound (sortedSalary.begin(), sortedSalary.end(), oldSalary);
        sortedSalary.at(lower- sortedSalary.begin()) = salary;
        sort (sortedSalary.begin(), sortedSalary.end());
        return true;
    }

    bool SetSalary ( const string   & email, unsigned int salary ) {
        int index=binarySearchEmail(sortedEmail, employees,count-1, 0, email);
        if(index  == -1)
            return false;
        employees[sortedEmail[index]].salary = salary;
        int oldSalary = employees[sortedEmail[index]].salary;

        vector<unsigned int>::iterator lower=  lower_bound (sortedSalary.begin(), sortedSalary.end(), oldSalary);
        sortedSalary[lower] = salary;
        sort (sortedSalary.begin(), sortedSalary.end());
        return true;
    }

    unsigned int  GetSalary ( const string & name, const string & surname ) const {
        int index= binarySearchName(sortedName,employees,count-1,0, name,surname);
        if(index  == -1)
            return 0;
        return employees[sortedName[index]].salary;
    }

    unsigned int  GetSalary ( const string & email ) const {
        int index = binarySearchEmail(sortedEmail, employees, count-1, 0, email);
        if(index == -1)
            return 0;
        return  employees[sortedEmail[index]].salary;
    }

    bool GetRank ( const string    & name, const string    & surname, int & rankMin, int  & rankMax ) const {
        int index= binarySearchName(sortedName,employees,count-1,0, name, surname);
        if(index  == -1)
            return false;
        rankMax= count-1; rankMin=0;
        //sort (sortedSalary.begin(), sortedSalary.end());
        rankMin = binarySearchSalaryRankLow(sortedSalary,count-1,0, employees[sortedName[index]].salary);
        rankMax = binarySearchSalaryRankHigh(sortedSalary,count-1,0,  employees[sortedName[index]].salary);
        /*
        for(int i=0; i < count; ++i){
            if(employees[i].salary < employees[sortedName[index]].salary){
                rankMin++;
            }
            if(employees[i].salary > employees[sortedName[index]].salary){
                rankMax--;
            }
        }*/
        return true;
    }

    bool GetRank ( const string & email, int & rankMin, int & rankMax ) const {
        int index= binarySearchEmail(sortedEmail,employees,count-1,0, email);
        if(index  == -1)
            return false;

        rankMax= count-1; rankMin=0;
       // sort (sortedSalary.begin(), sortedSalary.end());
        rankMin = binarySearchSalaryRankLow(sortedSalary,count-1,0, employees[sortedEmail[index]].salary);
        rankMax = binarySearchSalaryRankHigh(sortedSalary,count-1,0,  employees[sortedEmail[index]].salary);
        /*
        for(int i=0; i < count; ++i){
            if(employees[i].salary < employees[sortedEmail[index]].salary){
                rankMin++;
            }
            if(employees[i].salary > employees[sortedEmail[index]].salary){
                rankMax--;
            }
        }*/


        return true;
    }

    bool GetFirst ( string & outName, string & outSurname ) const {
        if(count == 0){
            return false;
        }

        outName = employees[sortedName.front()].name;
        outSurname = employees[sortedName.front()].surname;
        return  true;

    }

    bool GetNext ( const string & name, const string & surname, string & outName, string  & outSurname ) const {
        int index= binarySearchName(sortedName, employees,count-1,0, name, surname);

        if(index ==-1 || index == count-1)
            return false;
        outSurname = employees[sortedName[index+1]].surname;
        outName = employees[sortedName[index+1]].name;
        return true;

    }

};

#ifndef __PROGTEST__
int main ( void )
{
    string outName, outSurname;
    int lo, hi;

   CPersonalAgenda b1;
    assert ( b1 . Add ( "John", "Smith", "john", 30000 ) );
    assert ( b1 . Add ( "John", "Miller", "johnm", 35000 ) );
    assert ( b1 . Add ( "Peter", "Smith", "peter", 23000 ) );
    assert ( b1 . GetFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . GetNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( b1 . GetNext ( "John", "Smith", outName, outSurname )
             && outName == "Peter"
             && outSurname == "Smith" );
    assert ( ! b1 . GetNext ( "Peter", "Smith", outName, outSurname ) );
    assert ( b1 . SetSalary ( "john", 32000 ) );
    assert ( b1 . GetSalary ( "john" ) ==  32000 );
    assert ( b1 . GetSalary ( "John", "Smith" ) ==  32000 );
    assert ( b1 . GetRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . GetRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . GetRank ( "johnm", lo, hi )
             && lo == 2
             && hi == 2 );


    assert ( b1 . SetSalary ( "John", "Smith", 35000 ) );

    assert ( b1 . GetSalary ( "John", "Smith" ) ==  35000 );
    assert ( b1 . GetSalary ( "john" ) ==  35000 );
    assert ( b1 . GetRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . GetRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . GetRank ( "johnm", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . ChangeName ( "peter", "James", "Bond" ) );
    assert ( b1 . GetSalary ( "peter" ) ==  23000 );
    assert ( b1 . GetSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . GetSalary ( "Peter", "Smith" ) ==  0 );
    assert ( b1 . GetFirst ( outName, outSurname )
             && outName == "James"
             && outSurname == "Bond" );
    assert ( b1 . GetNext ( "James", "Bond", outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . GetNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . GetNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . ChangeEmail ( "James", "Bond", "james" ) );
    assert ( b1 . GetSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . GetSalary ( "james" ) ==  23000 );
    assert ( b1 . GetSalary ( "peter" ) ==  0 );
    assert ( b1 . Del ( "james" ) );
   assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 0
             && hi == 1 );
    assert ( b1 . Del ( "John", "Miller" ) );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . GetFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . GetNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . Del ( "john" ) );
    assert ( ! b1 . GetFirst ( outName, outSurname ) );
    assert ( b1 . Add ( "John", "Smith", "john", 31000 ) );
    assert ( b1 . Add ( "john", "Smith", "joHn", 31000 ) );
    assert ( b1 . Add ( "John", "smith", "jOhn", 31000 ) );

    CPersonalAgenda b2;
    assert ( ! b2 . GetFirst ( outName, outSurname ) );
    assert ( b2 . Add ( "James", "Bond", "james", 70000 ) );
    assert ( b2 . Add ( "James", "Smith", "james2", 30000 ) );
    assert ( b2 . Add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( ! b2 . Add ( "James", "Bond", "james3", 60000 ) );
    assert ( ! b2 . Add ( "Peter", "Bond", "peter", 50000 ) );
    assert ( ! b2 . ChangeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . ChangeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . SetSalary ( "Joe", "Black", 90000 ) );
    assert ( ! b2 . SetSalary ( "joe", 90000 ) );
    assert ( b2 . GetSalary ( "Joe", "Black" ) ==  0 );
    assert ( b2 . GetSalary ( "joe" ) ==  0 );
    assert ( ! b2 . GetRank ( "Joe", "Black", lo, hi ) );
    assert ( ! b2 . GetRank ( "joe", lo, hi ) );
    assert ( ! b2 . ChangeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . ChangeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . Del ( "Joe", "Black" ) );
    assert ( ! b2 . Del ( "joe" ) );
    assert ( ! b2 . ChangeName ( "james2", "James", "Bond" ) );
    assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "james" ) );
    assert ( ! b2 . ChangeName ( "peter", "Peter", "Smith" ) );
    assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "peter" ) );
    assert ( b2 . Del ( "Peter", "Smith" ) );
    assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "peter2" ) );
    assert ( ! b2 . SetSalary ( "Peter", "Smith", 35000 ) );
    assert ( b2 . GetSalary ( "Peter", "Smith" ) ==  0 );
    assert ( ! b2 . GetRank ( "Peter", "Smith", lo, hi ) );
    assert ( ! b2 . ChangeName ( "peter", "Peter", "Falcon" ) );
    assert ( ! b2 . SetSalary ( "peter", 37000 ) );
    assert ( b2 . GetSalary ( "peter" ) ==  0 );
    assert ( ! b2 . GetRank ( "peter", lo, hi ) );
    assert ( ! b2 . Del ( "Peter", "Smith" ) );
    assert ( ! b2 . Del ( "peter" ) );
    assert ( b2 . Add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( b2 . GetSalary ( "peter" ) ==  40000 );



    return 0;
}
#endif /* __PROGTEST__ */
