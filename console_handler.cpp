#include <iostream>

using namespace std;

/*  ---------------------------------------------------------------------[<]-
    Function: reset
    Synopsis: Set input value to 0.
 ---------------------------------------------------------------------[>]-*/
void reset(int &a, int &b){a = 0; b = 0;}

/*  ---------------------------------------------------------------------[<]-
    Function: clear_console
    Synopsis: Clear console.
 ---------------------------------------------------------------------[>]-*/
void clear_console() {
    #ifdef _WIN32
        system("cls");  
    #else
        system("clear");
    #endif
}