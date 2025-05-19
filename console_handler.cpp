#include <iostream>

using namespace std;

/*  ---------------------------------------------------------------------[<]-
    Function: reset
    Synopsis: Set first input value to 0, second to -1.
 ---------------------------------------------------------------------[>]-*/
void reset(int &a, int &b){a = 0; b = -1;}

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