/*  ----------------------------------------------------------------<Header>-
    Name:       task22.cpp
    Title:      Path Solver on a Fixed Grid

    Group:      TV-43
    Student:    Naiduik M.O
    Written:    2025-05-02
    Revised:    2025-05-02

    Description:
        Solves a puzzle on a 9x11 grid using a cell class. The goal is to
        fill zones and connect fixed values according to specified rules.
 ------------------------------------------------------------------</Header>-*/
#include <iostream>
#include <iomanip>
#include "game.hpp"
#include "console_handler.hpp"

#define ROW 9
#define COL 11
using namespace std;


int main(){
    game game1;
    char ans1;

    cout << setw(45) << "Menu" << endl;
    cout << setw(30) << " " << "1) Input playing field by user." << endl;
    cout << setw(30) << " " << "2) Using programmed field." << endl;
    cout << setw(30) << " " << "3) Exit." << endl;
       
    cout << setw(30) << " " << "Choose option: ";
    cin >> ans1;

    switch (ans1){
    case '1':
        game1 = game(0);
        break;
    case '2':
        game1 = game();
        break;
    case '3':
        clear_console();
        cout << "Exiting!" << endl;
        clear_console();
        return 0;
    default:
        cout << endl << setw(46) << "Auto-choose programmed!" << endl;
        game1 = game();
        break;
    }

    char ans2;
    while(true){
        cout << setw(52) <<" Starting field" << endl;
        game1.print_field(false);
        cout <<endl << setw(47) <<"Zones" << endl;
        game1.print_field(true);
        cout << setw(50) << "Rules: The number in the area of each block ,is equal to the sum of all the numbers inside that block." << endl;

        cout << setw(45) << "Menu" << endl;
        cout << setw(30) << " " << "1) Input by user." << endl;
        cout << setw(30) << " " << "2) Show step-by-step answer." << endl;
        cout << setw(30) << " " << "3) Exit." << endl;

        
        cout << setw(30) << " " << "Choose option: ";
        cin >> ans2;

        switch (ans2){
        case '1':
            clear_console();
            cout << endl << setw(55) << "Please input cells." << endl;
            game1.solve_by_user();
            cout << "Press button to continue." << endl;
            cin.get();
            clear_console();
            break;
        case '2':
            cout << endl << setw(58) << "Showing step-by-step answer." << endl;
            game1.solve();
            cout << "Press button to continue." << endl;
            cin.get();
            clear_console();
            break;
        case '3':
            clear_console();
            cout << "Exiting!" << endl;
            clear_console();
            return 0;
        default:
            cout << endl << setw(46) << "Choose from 1-3!" << endl;
            break;
        }
        cout << endl << endl;
        if(ans1 == '1'){
            game1 = game(0);
        } else if (ans1 == '2'){
            game1 = game();
        }
    }
}
