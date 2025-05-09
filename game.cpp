#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iomanip>
#include <cstdlib>
#include "game.hpp"
#include "cell.hpp"
#include "console_handler.hpp"

#define ROW 9
#define COL 11

using namespace std;

const int CONST_G_DIR[4][2] = {{-1,0}, {0,-1}, {1,0}, {0,1}};

/*  ---------------------------------------------------------------------[<]-
    Struct: coord
    Synopsis: Coordinate helper structure.
 ---------------------------------------------------------------------[>]-*/
struct coord{
    int row;
    int col;
    coord(int row = 0, int col = 0){
        this->row = row;
        this->col = col;
    }
};

/*  ---------------------------------------------------------------------[<]-
    Constructor: game
    Synopsis: Fills the field according to the initial conditions.
 ---------------------------------------------------------------------[>]-*/
game :: game(){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            field[i][j] = cell();
            zone[i][j] = 0;
        }
    }

    field[2][0] = cell(1);
    field[2][10] = cell(19);

    field[3][0] = cell(5);
    field[3][10] = cell(19);

    field[5][0] = cell(1);
    field[5][10] = cell(19);

    field[6][0] = cell(3);
    field[6][10] = cell(4);

    field[8][2] = cell(5);
    field[8][3] = cell(1);
    field[8][7] = cell(14);
    field[8][8] = cell(14);

    zone[1][1] = 3;
    zone[1][2] = 3;
    zone[1][3] = 4;
    zone[1][4] = 4;
    zone[1][6] = 12;
    zone[1][7] = 12;
    zone[1][8] = 11;
    zone[1][9] = 11;

    zone[2][1] = 2;
    zone[2][4] = 5;
    zone[2][6] = 13;

    zone[3][4] = 5;
    zone[3][6] = 13;

    zone[4][1] = 7;
    zone[4][2] = 7;
    zone[4][3] = 6;
    zone[4][4] = 6;
    zone[4][6] = 14;
    zone[4][7] = 20;
    zone[4][8] = 20;
    zone[4][9] = 19;

    zone[5][1] = 8;
    zone[5][6] = 15;
    zone[5][9] = 18;

    zone[6][1] = 8;
    zone[6][4] = 11;
    zone[6][6] = 15;
    zone[6][9] = 18;

    zone[7][1] = 9;
    zone[7][2] = 9;
    zone[7][3] = 10;
    zone[7][4] = 10;

    zone[7][6] = 16;
    zone[7][7] = 16;
    zone[7][8] = 17;
    zone[7][9] = 17;

    max_number = find_max_fixed_number(0, row_max,col_max);
}

/*  ---------------------------------------------------------------------[<]-
    Constructor: game
    Synopsis: Fills the field by user.
 ---------------------------------------------------------------------[>]-*/
game :: game (int notused){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            field[i][j] = cell();
            zone[i][j] = 0;
        }
    }

    cout << setw(52) <<" Starting field" << endl;
    print_field(false);
    cout <<endl << setw(47) <<"Zones" << endl;
    print_field(true);

    char confirm;
    cout << "Do you want fill(Input 0 to exit): ";
    cin >> confirm;
    clear_console();

    while(confirm != '0'){

        cout << setw(50) <<" Starting field" << endl;
        print_field(false);
        int tmp_row;
        int tmp_col;
        cout << "Input coords(ROW COL): ";
        cin >> tmp_row >> tmp_col;

        if(cin.fail() || tmp_row < 0 || tmp_col < 0 || tmp_row >= ROW || tmp_col >= COL){
            cin.clear();
            cin.ignore(100,'\n');
            cout << " Invalid coordinates. Try again." << endl;
            continue;
        }
  
        int new_val;
        cout << "Input new value: ";
        cin >> new_val;
    
        if (cin.fail() || new_val <= 0) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid value. Try again." << endl;
            continue;
        }

        field[tmp_row][tmp_col].set_value(new_val);
        field[tmp_row][tmp_col].set_connected();
        cout << "Value updated." << endl;
        clear_console();
        cout << setw(50) <<" Starting field" << endl;
        print_field(false);


        cout << "Do you want to fill another cell? (Input 0 to exit): ";
        cin >> confirm;
        clear_console();
    }

    max_number = find_max_fixed_number(0, row_max,col_max);
    clear_console();
}

/*  ---------------------------------------------------------------------[<]-
    Function: clear_field
    Synopsis: Clears game field.
 ---------------------------------------------------------------------[>]-*/
void game::clear_field() {
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            field[i][j] = cell(); 
        }
    }
}

/*  ---------------------------------------------------------------------[<]-
    Function: check_cell
    Synopsis: Checks neighboring cells have same value with a given value.
 ---------------------------------------------------------------------[>]-*/
 bool game :: check_cell(int value, int start_row, int start_col){
    for(int i = 0; i < 4; i++){
        int new_row = start_row + CONST_G_DIR[i][0];
        int new_col = start_col + CONST_G_DIR[i][1];

        if(field[new_row][new_col].get_value() == value) return true;
    }
    return false;
}

/*  ---------------------------------------------------------------------[<]-
    Function: check_answer
    Synopsis: Checks if the game was completed correctly.
 ---------------------------------------------------------------------[>]-*/
bool game :: check_answer(){
    int near_row = 0;
    int near_col = 0;
    int count = 0;
    for(int row = 0; row < ROW; row++){
        for(int col = 0; col < COL; col++){
            count = count_connect_numbers(field[row][col].get_value(), row, col);
            if(zone[row][col] != 0){
                indetefy_zone(row, col, near_row, near_col);
                if(near_row == 0 && near_col == 0){
                    if((count != field[row][col].get_value()) && 
                        field[row][col].get_value()  != zone[row][col]) return false;
                } else {
                    if((count != field[row][col].get_value()) && 
                        field[row][col].get_value() + field[near_row][near_col].get_value()  != zone[row][col]) return false;
                }
            } else {
                if(count != field[row][col].get_value()) return false;
            }
        }
    }
    return true;
}

/*  ---------------------------------------------------------------------[<]-
    Function: count_connect_numbers
    Synopsis: Counts all connected cells with the same value starting from
              a given coordinate using BFS.
 ---------------------------------------------------------------------[>]-*/
 int game :: count_connect_numbers(int value, int start_row, int start_col){
    queue<coord> search;
    bool visited[ROW][COL] = {false};
    int count = 1;

    search.push(coord(start_row, start_col));
    visited[start_row][start_col] = true;

    while(!search.empty()){
        coord currect = search.front();
        search.pop();

        for(int i = 0; i < 4; i++){
            int new_row = currect.row + CONST_G_DIR[i][0];
            int new_col = currect.col + CONST_G_DIR[i][1];
            if(field[new_row][new_col].get_value() == value) field[new_row][new_col].set_connected();

            if(field[new_row][new_col].get_connected() && !visited[new_row][new_col] 
               && can_place(new_row, new_col) && field[new_row][new_col].get_value() == value){
                visited[new_row][new_col] = true;
                search.push(coord(new_row, new_col));
                count++;
            }
        }
    }
    return count;
}
/*  ---------------------------------------------------------------------[<]-
    Function: count_empty_cell
    Synopsis: Counts the number of empty cells in a zone.
 ---------------------------------------------------------------------[>]-*/
int game :: count_empty_cell(int start_row, int start_col){
    queue<coord> search;
    bool visited[ROW][COL] = {false};
    int count = 1;

    search.push(coord(start_row, start_col));
    visited[start_row][start_col] = true;

    while(!search.empty()){
        coord currect = search.front();
        search.pop();

        for(int i = 0; i < 4; i++){
            int new_row = currect.row + CONST_G_DIR[i][0];
            int new_col = currect.col + CONST_G_DIR[i][1];

            if(!field[new_row][new_col].get_connected() && !visited[new_row][new_col] 
               && can_place(new_row, new_col) && field[new_row][new_col].get_value() == 0){
                visited[new_row][new_col] = true;
                search.push(coord(new_row, new_col));
                count++;
            }
        }
    }
    return count;
}

/*  ---------------------------------------------------------------------[<]-
    Function: connect_same_fixed_number_bfs
    Synopsis: Find and connect same fixed numbers with same value starting 
              from a given coordinate using BFS and function connect_path.
 ---------------------------------------------------------------------[>]-*/
 bool game :: connect_same_fixed_number_bfs(int value, int start_row, int start_col){
    queue<coord> search;
    bool visited[ROW][COL] = {false};

    search.push(coord(start_row, start_col));
    visited[start_row][start_col] = true;

    while(!search.empty()){
        coord currect = search.front();
        search.pop();

        for(int i = 0; i < 4; i++){
            int new_row = currect.row + CONST_G_DIR[i][0];
            int new_col = currect.col + CONST_G_DIR[i][1];

            if((abs(start_row - new_row) + abs(start_col - new_col)) >= value) continue;

            if(can_place(new_row, new_col) && !visited[new_row][new_col]){
                visited[new_row][new_col] = true;
                search.push(coord(new_row, new_col));
                
                if(field[new_row][new_col].get_value() == value &&
                   field[new_row][new_col].get_fixed() &&
                   !field[new_row][new_col].get_connected() &&
                   (new_row != start_row || new_col != start_col) && 
                   count_connect_numbers(value, start_row, start_col) != value){
                    
                    for(int j = 0; j < 4; j++){
                        int adj_row = new_row + CONST_G_DIR[j][0];
                        int adj_col = new_col + CONST_G_DIR[j][1];

                        if(field[adj_row][adj_col].get_connected() && field[adj_row][adj_col].get_value() == value){
                            visited[adj_row][adj_col] = true;
                            visited[new_row][new_col] = true;
                            field[new_row][new_col].set_connected();
                            field[adj_row][adj_col].set_connected();
                            return true;
                        } 
                    }
                    if(connect_path(value, start_row, start_col, new_row, new_col)){
                        field[start_row][start_col].set_connected();
                        field[new_row][new_col].set_connected();

                        cout << setw(51) << "Starting from cell(" << start_row << ", " << start_col << ")." << endl;
                        cout << setw(51) << "Finishing to cell(" << new_row << ", " << new_col << ")." << endl;
                        cout << setw(47) << "Value: " << value << endl;

                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/*  ---------------------------------------------------------------------[<]-
    Function: connect_path
    Synopsis: Connect same fixed numbers with same value starting from
              a given coordinate to finish coordinate using simple
              methods(straight, L-turn, Z-turn). 
 ---------------------------------------------------------------------[>]-*/
 bool game :: connect_path(int value, int start_row, int start_col, int finish_row, int finish_col) {
    if(start_row == finish_row){
        int col_start = min(start_col, finish_col);
        int col_end = max(start_col, finish_col);
        for(int col = col_start + 1; col < col_end ; col++){
            if(field[start_row][col].get_fixed() && field[start_row][col].get_value() != value) return false;
            field[start_row][col].set_value(value);
            field[start_row][col].set_connected();
        }
        return true;
    }

    else if (start_col == finish_col) {
        int row_start = min(start_row, finish_row);
        int row_end = max(start_row, finish_row);
        for (int row = row_start + 1; row < row_end; row++) {
            if (field[row][start_col].get_fixed() && field[row][start_col].get_value() != value) return false; 
            field[row][start_col].set_value(value);
            field[row][start_col].set_connected();
        }
        return true;
    }

    if (!field[finish_row][start_col].get_fixed() || field[finish_row][start_col].get_value() == value) {
        bool through_row = true;
        for (int row = min(start_row, finish_row) + 1; row < max(start_row, finish_row); row++) {
            if (field[row][start_col].get_fixed() && field[row][start_col].get_value() != value) {
                through_row = false;
                break;
            }
            field[row][start_col].set_value(value);
            field[row][start_col].set_connected();
        }
        if (through_row) {
            for (int col = min(start_col, finish_col); col <= max(start_col, finish_col); col++) {
                if (field[finish_row][col].get_fixed() && field[finish_row][col].get_value() != value) return false;
                field[finish_row][col].set_value(value);
                field[finish_row][col].set_connected();
            }
            return true;
        }
    }

    if (!field[start_row][finish_col].get_fixed() || field[start_row][finish_col].get_value() == value) {
        bool through_col = true;
        for (int col = min(start_col, finish_col) + 1; col < max(start_col, finish_col); col++) {
            if (field[start_row][col].get_fixed() && field[start_row][col].get_value() != value) {
                through_col = false;
                break;
            }
            field[start_row][col].set_value(value);
            field[start_row][col].set_connected();

        }
        if (through_col) {
            for (int row = min(start_row, finish_row); row <= max(start_row, finish_row); row++) {
                if (field[row][finish_col].get_fixed() && field[row][finish_col].get_value() != value) return false;
                field[row][finish_col].set_value(value);
                field[row][finish_col].set_connected();
            }
            return true;
        }
    }
     
    for (int mid_row = 0; mid_row < ROW; mid_row++) {
        if (mid_row == start_row || mid_row == finish_row) continue;
        bool ok = true;

        int col_start = min(start_col, finish_col);
        int col_end = max(start_col, finish_col);

        for (int row = min(start_row, mid_row); row <= max(start_row, mid_row); row++) {
            if (field[row][start_col].get_fixed() && field[row][start_col].get_value() != value) { ok = false; break; }
        }

        for (int col = col_start; col <= col_end; col++) {
            if (field[mid_row][col].get_fixed() && field[mid_row][col].get_value() != value) { ok = false; break; }
        }

        for (int row = min(mid_row, finish_row); row <= max(mid_row, finish_row); row++) {
            if (field[row][finish_col].get_fixed() && field[row][finish_col].get_value() != value) { ok = false; break; }
        }

        if (ok) {
            for (int row = min(start_row, mid_row); row <= max(start_row, mid_row); row++) {
                field[row][start_col].set_value(value);
                field[row][start_col].set_connected();
            }
            for (int col = col_start; col <= col_end; col++) {
                field[mid_row][col].set_value(value);
                field[mid_row][col].set_connected();
            }
            for (int row = min(mid_row, finish_row); row <= max(mid_row, finish_row); row++) {
                field[row][finish_col].set_value(value);
                field[row][finish_col].set_connected();
            }
            return true;
        }
    }

    for (int mid_col = 0; mid_col < COL; mid_col++) {
        if (mid_col == start_col || mid_col == finish_col) continue;
        bool ok = true;

        int row_start = min(start_row, finish_row);
        int row_end = max(start_row, finish_row);

        for (int col = min(start_col, mid_col); col <= max(start_col, mid_col); col++) {
            if (field[start_row][col].get_fixed() && field[start_row][col].get_value() != value) { ok = false; break; }
        }

        for (int row = row_start; row <= row_end; row++) {
            if (field[row][mid_col].get_fixed() && field[row][mid_col].get_value() != value) { ok = false; break; }
        }

        for (int col = min(mid_col, finish_col); col <= max(mid_col, finish_col); col++) {
            if (field[finish_row][col].get_fixed() && field[finish_row][col].get_value() != value) { ok = false; break; }
        }

        if (ok) {
            for (int col = min(start_col, mid_col); col <= max(start_col, mid_col); col++) {
                field[start_row][col].set_value(value);
                field[start_row][col].set_connected();
            }
            for (int row = row_start; row <= row_end; row++) {
                field[row][mid_col].set_value(value);
                field[row][mid_col].set_connected();
            }
            for (int col = min(mid_col, finish_col); col <= max(mid_col, finish_col); col++) {
                field[finish_row][col].set_value(value);
                field[finish_row][col].set_connected();
            }
            return true;
        }
    }  
    return false; 
}

/*  ---------------------------------------------------------------------[<]-
    Function: can_place
    Synopsis: Checks if the coordinates are within the grid bounds.
 ---------------------------------------------------------------------[>]-*/
 bool game :: can_place(int row, int col){
    if(row < 0 || col < 0) return false;
    else if(row >= ROW || col >= COL) return false;
    else return true; 
}

/*  ---------------------------------------------------------------------[<]-
    Function: find_max_fixed_number
    Synopsis: Finds the largest fixed number in the field that is less 
              than given current maximum number.
 ---------------------------------------------------------------------[>]-*/
 int game :: find_max_fixed_number(int currect_max_number, int &row, int &col){
    int max_num = 0;
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(currect_max_number !=0){
                if(field[i][j].get_value() > max_num && field[i][j].get_value() < currect_max_number && field[i][j].get_fixed()){
                    max_num = field[i][j].get_value();
                    row = i;
                    col = j;
                } 
            } else {
                if(field[i][j].get_value() > max_num){
                    max_num = field[i][j].get_value();
                    row = i;
                    col = j;
                } 
            }
        }
    }
    return max_num;
}

/*  ---------------------------------------------------------------------[<]-
    Function: find_min_fixed_number
    Synopsis: Finds the lowest fixed number equals to current minimum
              searching in reverse order.
 ---------------------------------------------------------------------[>]-*/
 int game :: find_min_fixed_number(int current_min_number, int &row, int &col){
    int min_num = 0;
    for(int i = ROW - 1; i >= 0; i--){
        for(int j = COL - 1; j >= 0; j--){
            if(field[i][j].get_value() == current_min_number && field[i][j].get_fixed()){
                min_num = field[i][j].get_value();
                row = i;
                col = j;
                return min_num;
            }
        }
    }
    return current_min_number;
}

/*  ---------------------------------------------------------------------[<]-
    Function: find_another_fixed_number
    Synopsis: Finds the same fixed number equals to current number.
 ---------------------------------------------------------------------[>]-*/
 bool game :: find_another_fixed_number(int number, int &row, int &col){
    for(int i = ROW - 1; i >= 0; i--){
        for(int j = COL - 1; j >= 0; j--){
            if(field[i][j].get_value() == number && !field[i][j].get_connected() && ((i < row) || (i == row && j < col))){ 
                row = i;
                col = j;
                return true;
            }
        }
    }
    return false;
}
/*  ---------------------------------------------------------------------[<]-
    Function: find_zone
    Synopsis: Search for the next cell with value 0 and a zone number.
 ---------------------------------------------------------------------[>]-*/
bool game :: find_zone(int &row, int &col){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(zone[i][j] != 0 && ((i > row) || (i == row && j > col)) && field[i][j].get_value() == 0){
                row = i;
                col = j;
                return true;
            }
        }
    }
    return false;
}

/*  ---------------------------------------------------------------------[<]-
    Function: find_empty_cell
    Synopsis: Search empty cell.
 ---------------------------------------------------------------------[>]-*/
bool game :: find_empty_cell(int &row, int &col){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(field[i][j].get_value() == 0 && !field[i][j].get_connected() && ((i > row) || (i == row && j > col))){
                row = i;
                col = j;
                return true;
            } 
        }
    }
    return false;
}

/*  ---------------------------------------------------------------------[<]-
    Function: fill_fixed_numbers
    Synopsis: Fill the cells in the field with the value starting from
              starting coordinate using speial methods.
              1)If the number of cell equals connected cell.
              2)If the number of cell equals connected cell - 1.
              3)Else using BFS to find all connected cell after which
                fill empty cell.
 ---------------------------------------------------------------------[>]-*/
 bool game :: fill_fixed_numbers(int connected, int value, int start_row, int start_col) {
    if (connected == value) return true;
    if(value - connected == 1){
        for (int i = 0; i < 4; i++) {
            int new_row = start_row + CONST_G_DIR[i][0];
            int new_col = start_col + CONST_G_DIR[i][1];
            if(can_place(new_row, new_col) && field[new_row][new_col].get_value() == 0){
                field[new_row][new_col].set_value(value);
                field[new_row][new_col].set_connected();
                return true;
            }
        }
    }

    int count = connected;
    deque<coord> queue;
    deque<coord> connected_coords;
    bool visited[ROW][COL] = {false};

    visited[start_row][start_col] = true;
    queue.push_back(coord(start_row, start_col));

    while (!queue.empty()) {
        coord current = queue.front();
        queue.pop_front();
        connected_coords.push_front(current);

        for (int i = 0; i < 4; i++) {
            int new_row = current.row + CONST_G_DIR[i][0];
            int new_col = current.col + CONST_G_DIR[i][1];

            if (can_place(new_row, new_col) &&
                field[new_row][new_col].get_value() == value &&
                !visited[new_row][new_col]) {
                visited[new_row][new_col] = true;
                queue.push_back(coord(new_row, new_col));
            }
        }
    }
    coord currect;
    bool visited_fixed[ROW][COL] = {false};

    while (!connected_coords.empty() && count < value) {
        coord currect;
        currect = connected_coords.back();
        connected_coords.pop_back();

        for (int i = 0; i < 4; i++) {
            int new_row = currect.row + CONST_G_DIR[i][0];
            int new_col = currect.col + CONST_G_DIR[i][1];
            
            while (can_place(new_row, new_col) && 
                  (field[new_row][new_col].get_value() == 0 || field[new_row][new_col].get_value() == value) 
                   && !visited_fixed[new_row][new_col]) {
                field[new_row][new_col].set_value(value);
                field[new_row][new_col].set_connected();
                visited_fixed[new_row][new_col] = true;
                count = count_connect_numbers(value, start_row, start_col);
                
                if (count == value) return true;

                connected_coords.push_back(coord(new_row, new_col));

                new_row += CONST_G_DIR[i][0];
                new_col += CONST_G_DIR[i][1];   
            }
            if(connected_coords.size() > 0) currect = connected_coords.back();
        }
    }
    if(value - count == 1){
        for (int i = 0; i < 4; i++) {
            int new_row = start_row + CONST_G_DIR[i][0];
            int new_col = start_col + CONST_G_DIR[i][1];
            if(can_place(new_row, new_col) && field[new_row][new_col].get_value() == 0){
                field[new_row][new_col].set_value(value);
                field[new_row][new_col].set_connected();
                return true;
            }
        }
    }
    return false;
}
/*  ---------------------------------------------------------------------[<]-
    Function: fill_zone
    Synopsis: Fill the cells in the field with value of zone starting from
              starting coordinate using speial methods.
              1)If zone size equals 2.
                1.1)Zone value < 5 (Fisrt cell set (value of zone) - 1,
                    Second cell set 1).
                1.2)Zone value >= 5 && zone value <=10 (Fisrt cell set 
                    (value of zone) / 2, Second cell set (value of zone) / 2).
                1.3)Zone value >= 11 && zone value <=20 (Fisrt cell set 
                    (value of zone) - (nearest fixed number), Second cell set
                    (nearest fixed number)).
              2)If zone size equals 1(Set cell value of zone).
 ---------------------------------------------------------------------[>]-*/
bool game :: fill_zone(int start_row, int start_col){
    int near_row;
    int near_col;
    bool two_cell_zone = indetefy_zone(start_row, start_col, near_row, near_col);
    if(two_cell_zone){
        if(zone[start_row][start_col] < 5){
            field[start_row][start_col].set_value(zone[start_row][start_col] - 1);
            field[near_row][near_col].set_value(1);
            field[start_row][start_col].set_fixed();
            field[near_row][near_col].set_fixed();

            cout << setw(45) << "Fill cell(" << start_row << ", " << start_col << "): " << zone[start_row][start_col] - 1 << endl;
            cout << setw(45) << "Fill cell(" << near_row << ", " << near_col << "): " << 1 << endl;
            return true;
        } 

        else if(zone[start_row][start_col] >= 5 && zone[start_row][start_col] <= 10){
            if(zone[start_row][start_col] % 2 == 0){
                field[start_row][start_col].set_value(zone[start_row][start_col] / 2);
                field[near_row][near_col].set_value(zone[start_row][start_col] / 2);
                field[start_row][start_col].set_fixed();
                field[near_row][near_col].set_fixed();

                cout << setw(45) << "Fill cell(" << start_row << ", " << start_col << "): " << zone[start_row][start_col] / 2<< endl;
                cout << setw(45) << "Fill cell(" << near_row << ", " << near_col << "): " << zone[near_row][near_col] / 2 << endl;
                return true;
            } 

            else {
                if(check_cell(zone[start_row][start_col] / 2 + 1, near_row, near_col) && 
                   check_cell(zone[start_row][start_col] / 2, start_row, start_col)){
                    int tmp = start_row;
                    start_row = near_row;
                    near_row = tmp;

                    tmp = start_col;
                    start_col = near_col;
                    near_col = tmp;
                }
                field[start_row][start_col].set_value(zone[start_row][start_col] / 2 + 1);
                field[near_row][near_col].set_value(zone[start_row][start_col] / 2);
                field[start_row][start_col].set_fixed();
                field[near_row][near_col].set_fixed();

                cout << setw(45) << "Fill cell(" << start_row << ", " << start_col << "): " << zone[start_row][start_col] / 2 + 1<< endl;
                cout << setw(45) << "Fill cell(" << near_row << ", " << near_col << "): " << zone[near_row][near_col] / 2 << endl;
                return true;
            }
        } 

        else if(zone[start_row][start_col] >= 11 && zone[start_row][start_col] <= 20){       
            int fixed_row = 0;
            int fixed_col = 0;
            int fixed_num = find_max_fixed_number(zone[start_row][start_col], fixed_row, fixed_col);
            if(abs(fixed_row - start_row) + abs(fixed_col - start_col) < fixed_num / 2 + 1 || 
               abs(fixed_row - near_row) + abs(fixed_col - near_col) < fixed_num / 2 + 1){
                if(fixed_num == max_number) {
                    fixed_num = find_max_fixed_number(zone[start_row][start_col] - 1, fixed_row, fixed_col);
                }
                if(zone[start_row][start_col] != 20){
                if((check_cell(zone[start_row][start_col] - fixed_num, near_row, near_col) || 
                    check_cell(fixed_num, start_row, start_col))){
                    int tmp = start_row;
                    start_row = near_row;
                    near_row = tmp;

                    tmp = start_col;
                    start_col = near_col;
                    near_col = tmp;
                }
            }
                
                field[start_row][start_col].set_value(zone[start_row][start_col]- fixed_num);
                field[near_row][near_col].set_value(fixed_num);
                field[start_row][start_col].set_fixed();
                field[near_row][near_col].set_fixed();

                cout << setw(45) << "Fill cell(" << start_row << ", " << start_col << "): " << zone[start_row][start_col] - fixed_num << endl;
                cout << setw(45) << "Fill cell(" << near_row << ", " << near_col << "): " << fixed_num << endl;
                return true;
            }

            else {
                if(zone[start_row][start_col] % 2 == 0){
                    field[start_row][start_col].set_value(zone[start_row][start_col] / 2);
                    field[near_row][near_col].set_value(zone[start_row][start_col] / 2);
                    field[start_row][start_col].set_fixed();
                    field[near_row][near_col].set_fixed();

                    cout << setw(45) << "Fill cell(" << start_row << ", " << start_col << "): " << zone[start_row][start_col] / 2 << endl;
                    cout << setw(45) << "Fill cell(" << near_row << ", " << near_col << "): " << zone[near_row][near_col] / 2 << endl;
                    return true;
                } 
                
                else {
                    if(check_cell(zone[start_row][start_col] / 2 + 1, near_row, near_col) && 
                       check_cell(zone[start_row][start_col] / 2, start_row, start_col)){
                        int tmp = start_row;
                        start_row = near_row;
                        near_row = tmp;
    
                        tmp = start_col;
                        start_col = near_col;
                        near_col = tmp;
                    }
                    field[start_row][start_col].set_value(zone[start_row][start_col] / 2 + 1); 
                    field[near_row][near_col].set_value(zone[start_row][start_col] / 2);
                    field[start_row][start_col].set_fixed();
                    field[near_row][near_col].set_fixed();

                    cout << setw(45) << "Fill cell(" << start_row << ", " << start_col << "): " << zone[start_row][start_col] / 2 + 1 << endl;
                    cout << setw(45) << "Fill cell(" << near_row << ", " << near_col << "): " << zone[near_row][near_col] / 2 << endl;
                    return true;
                }
            }
        }
    } 
    
    else {
        field[start_row][start_col].set_value(zone[start_row][start_col]);
        field[start_row][start_col].set_fixed();

        cout << setw(45) << "Fill cell(" << start_row << ", " << start_col << "): " << zone[start_row][start_col] << endl;
        return true;
    }
    return false;
}

/*  ---------------------------------------------------------------------[<]-
    Function: fill_empty_cell
    Synopsis: Combines empty cells into a single area.
 ---------------------------------------------------------------------[>]-*/
bool game :: fill_empty_cell(int value, int start_row, int start_col){ 
    deque<coord> queue;
    deque<coord> connected_coords;
    bool visited[ROW][COL] = {false};

    visited[start_row][start_col] = true;
    queue.push_back(coord(start_row, start_col));

    while (!queue.empty()) {
        coord current = queue.front();
        queue.pop_front();
        connected_coords.push_front(current);

        for (int i = 0; i < 4; i++) {
            int new_row = current.row + CONST_G_DIR[i][0];
            int new_col = current.col + CONST_G_DIR[i][1];

            if (can_place(new_row, new_col) &&
                field[new_row][new_col].get_value() == 0 &&
                !visited[new_row][new_col]) {
                visited[new_row][new_col] = true;
                queue.push_back(coord(new_row, new_col));
            }

            if(can_place(new_row, new_col) && 
               field[new_row][new_col].get_value() == value &&
               !visited[new_row][new_col]){
                fill_empty_cell(value - 1, start_row, start_col);
            }
        }
    }
    coord currect;

    int count = 0;
    while (!connected_coords.empty()) {
        coord currect;
        currect = connected_coords.back();
        connected_coords.pop_back();

        field[currect.row][currect.col].set_value(value);
        field[currect.row][currect.col].set_connected();
                
        count = count_connect_numbers(value, start_row, start_col);
                
        if (count == value) return true;
    }
    
    return false;
}

/*  ---------------------------------------------------------------------[<]-
    Function: indetefy_zone
    Synopsis: Checks whether the cell size is 1x1 or 2x1
 ---------------------------------------------------------------------[>]-*/
bool game :: indetefy_zone(int row, int col, int &near_row, int &near_col){
    for(int i = 0; i < 4;i++){
        if((zone[row + CONST_G_DIR[i][0]][col + CONST_G_DIR[i][1]] == zone[row][col])){
            near_row = row + CONST_G_DIR[i][0];
            near_col = col + CONST_G_DIR[i][1];
            return true;
        } 
    }
    near_row = 0;
    near_col = 0;
    return false;
}

/*  ---------------------------------------------------------------------[<]-
    Function: solve
    Synopsis: Main function for solve field according to rules.
 ---------------------------------------------------------------------[>]-*/
void game :: solve(){
    int row = 0;
    int col = 0;
    int near_x = 0;
    int near_y = 0;
    int max = 0;
    char ch;
    stack<int> max_numbers;

    cout << setw(67) << "After step. Press Enter to continue." << endl;
    while (true) {
        ch = cin.get();
        if (ch == '\n') break;  
    }
    clear_console();

    while(find_zone(row, col)){
        if(!indetefy_zone(row, col, near_x, near_y)){
            cout << setw(65) << "Fill cells that have single-cell zones." << endl;
            fill_zone(row, col);
            pause();
        }
    }

    reset(row,col);
    while(find_zone(row, col)){
        cout << setw(65) << "Fill cells that have two-cell zones." << endl;
        fill_zone(row, col);
        pause();
    }
    
    while((max = find_max_fixed_number(max, row, col)) != 1){
        if(max == 0) break;
        cout << setw(70) << "Find cells that have same value and merge them." << endl;
        while(connect_same_fixed_number_bfs(max, row, col));
        pause();
        max_numbers.push(max);
    }

    
    if(max_number != 0){
        cout << setw(65) << "Fill the field with the largest value" << endl;
        cout << setw(47) << "Value: " << max_number << endl;
        cout << setw(55) << "Starting filling from cell(" << row_max << ", " << col_max << ")." << endl;
        fill_fixed_numbers(count_connect_numbers(max_number, row_max, col_max), max_number, row_max ,col_max);
        pause();
    }
    
    reset(row,col);
    if(max_numbers.size() > 0){
        int min = max_numbers.top();
        max_numbers.pop();
        while(min != max_number){
            min = find_min_fixed_number(min, row, col);
            cout << setw(85) << "Starting from the end, fill in the field from the smallest to the largest number" << endl;
            cout << setw(47) << "Value: " << min << endl;
            cout << setw(55) << "Starting filling from cell(" << row << ", " << col << ")." << endl;
        
            fill_fixed_numbers(count_connect_numbers(min, row, col), min, row, col);
            while(find_another_fixed_number(min, row, col)){
                cout << setw(55) << "Starting filling from cell(" << row << ", " << col << ")." << endl;
                fill_fixed_numbers(count_connect_numbers(min, row, col), min, row ,col);
            }
            pause();
            min = max_numbers.top();
            max_numbers.pop();
        }
    }

    reset(row,col);
    while(find_empty_cell(row, col)){
        cout << setw(60) << "Search empty cell and fill cells." << endl;
        int count_empty_cells = count_empty_cell(row, col);
        fill_empty_cell(count_empty_cells, row, col);
        pause();
    }

    if(check_answer()){
        cout << setw(50) <<"Solved field" << endl; 
        print_field(false);
    } else {
        cout << setw(50) <<"Solution not found!" << endl; 
    }
}

/*  ---------------------------------------------------------------------[<]-
    Function: solve_by_user
    Synopsis: User input data for game resolution.
 ---------------------------------------------------------------------[>]-*/
void game :: solve_by_user(){
    int num;

    for(int i = 0; i < ROW ; i++){
        for(int j = 0; j < COL; j++){
            cout << setw(50) <<" Starting field" << endl;
            print_field(false, i, j);
            cout <<endl << setw(47) <<"Zones" << endl;
            print_field(true);

            cout << "Rules: The number in the area of each block ,is equal to the sum of all the numbers inside that block." << endl;
            cout << "       You can only fill cell that have value 0." << endl;
            cout << "       Each number on the playing field must become part of a continuous area,  "<< endl;
            cout << "       the number of cells of which corresponds to the value of this number." << endl;

            cout << "Input value in cell(" << i << ", " << j << "): "; 
            if(field[i][j].get_fixed()){
                clear_console();
                continue;
            }

            while(true){
                cin >> num;

                if(cin.fail() || num <= 0){
                    cin.clear();
                    cin.ignore(100, '\n');    
                } else {
                    cin.ignore(100, '\n');  
                    break;
                }
            }
            field[i][j].set_value(num);
            field[i][j].set_connected();
          
            clear_console();
        }
    }

    char confirm = 1;
    cout << setw(50) <<" Starting field" << endl;
    print_field(false);
    cout << "Do you want change(Input 0 to exit): ";
    cin >> confirm;
    clear_console();

    while(confirm != '0'){
        cout << setw(50) <<" Starting field" << endl;
        print_field(false);

        int tmp_row;
        int tmp_col;
        cout << "Input coords(ROW COL): ";
        cin >> tmp_row >> tmp_col;

        if(cin.fail() || tmp_row < 0 || tmp_col < 0 || tmp_row >= ROW || tmp_col >= COL){
            cin.clear();
            cin.ignore(100,'\n');
            cout << " Invalid coordinates. Try again." << endl;
            continue;
        }

        if (field[tmp_row][tmp_col].get_fixed()) {
            cout << "This cell is fixed and cannot be changed." << endl;
        } else {
            int new_val;
            cout << "Input new value: ";
            cin >> new_val;
    
            if (cin.fail() || new_val <= 0) {
                cin.clear();
                cin.ignore(100, '\n');
                cout << "Invalid value. Try again." << endl;
                continue;
            }

            field[tmp_row][tmp_col].set_value(new_val);
            field[tmp_row][tmp_col].set_connected();
            cout << "Value updated." << endl;
            clear_console();
            cout << setw(50) <<" Starting field" << endl;
            print_field(false);
        }

        cout << "Do you want to change another cell? (Input 0 to exit): ";
        cin >> confirm;
        clear_console();
    }
    
    while(getchar() != '\n');
    if(check_answer()){
        cout << setw(70) <<"You solve field correct! Congratulations!!!" << endl; 
    } else {
        cout << setw(64) <<"You solve field wrong! Try again!" << endl; 
    }
    print_field(false);
}


/*  ---------------------------------------------------------------------[<]-
    Function: print_field
    Synopsis: Print field in console.
 ---------------------------------------------------------------------[>]-*/
void game :: print_field(bool zones, int row, int col){
    cout << "┌";
    for (int j = 0; j < COL - 1; j++) cout << "─────" << "──┬"; 
    cout << "─────" << "──┐" << endl; 

    for (int i = 0; i < ROW; i++) {
        cout << "│"; 
        for (int j = 0; j < COL; j++) {
            if(!zones){
                if(i == row && j == col){
                    if(j == 0) cout << setw(4) << "?";
                    else cout << "   │" << setw(4) << "?";
                } else {
                    if(j == 0) cout << setw(4) << field[i][j].get_value();
                    else cout << "   │" << setw(4) << field[i][j].get_value();
                }
            } else {
                if(j == 0) cout << setw(4) << zone[i][j];
                else cout << "   │" << setw(4) << zone[i][j];
            }
        }
        cout << "   │"; 
        cout << endl;

        if (i != ROW - 1) {
            cout << "├"; 
            for (int j = 0; j < COL - 1; j++) cout << "─────" << "──┼"; 
            cout << "─────" << "──┤" << endl; 
        }
    }

    cout << "└"; 
    for (int j = 0; j < COL - 1; j++) cout << "─────" << "──┴"; 
    cout << "─────" << "──┘" << endl;
}

/*  ---------------------------------------------------------------------[<]-
    Function: pause
    Synopsis: Algorithm for step-by-step view. To see the next step, you
              need to press the Enter.
 ---------------------------------------------------------------------[>]-*/
void game :: pause(){
    print_field(false);
    cout << "Press Enter to continue." << endl;
    char ch;
    while (true) {
        ch = cin.get();
        if (ch == '\n') break;  
    }
    clear_console();
}