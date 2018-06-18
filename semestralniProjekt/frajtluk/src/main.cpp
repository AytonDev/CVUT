#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "appManager.h"


using namespace std;

int main() {
    cout.flush(); // Flush the output stream
    system("clear"); // Clear the console with the "system" function
    CAppManager app = CAppManager();
    app.PrintMain(cout, cin);

    return 0;
}

//cout.flush(); // Flush the output stream
//system("clear"); // Clear the console with the "system" function

