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
#include <vector>
#include <queue>
#include <stack>
#include <iomanip>
#include <conio.h>
#define ROW 9
#define COL 11
using namespace std;

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
    Class: cell
    Synopsis: Represents a cell in the grid.
 ---------------------------------------------------------------------[>]-*/
class cell {
    int value;
    bool fixed;
    bool connected;
public:
    cell(){value = 0; fixed = false; connected = false;}
    cell(int v){value = v; fixed = true; connected = false;}
    int get_value() {return value;}
    bool get_fixed() {return fixed;}
    bool get_connected() {return connected;}
    void set_connected() {connected = true;}
    void set_value(int v) {value = v;}
    void set_fixed() {fixed = true;}
};


bool check_cell(cell field[ROW][COL], int value, int start_row, int start_col);
int count_connect_numbers(cell field[ROW][COL], int value, int start_row, int start_col);
bool connect_same_fixed_number_bfs(cell field[ROW][COL], int value, int start_row, int start_col);
bool connect_path(cell field[ROW][COL], int value, int start_row, int start_col, int finish_row, int finish_col);
bool can_place(int row, int col);
bool find_zone(cell field[ROW][COL], int &row, int &col);
int find_max_fixed_number(cell field[ROW][COL], int max_number, int &row, int &col);
int find_min_fixed_number(cell field[ROW][COL], int current_min_number, int &row, int &col);
bool find_another_fixed_number(cell field[ROW][COL], int number, int &row, int &col);
bool fill_fixed_numbers(cell field[ROW][COL], int connected, int value, int start_row, int start_col);
bool fill_zone(cell field[ROW][COL], int start_row, int start_col);
bool indetefy_zone(int row, int col, int &near_row, int &near_col);
void solve(cell field[ROW][COL]);
void print_field(cell field[ROW][COL]);
void reset(int &a, int &b);

int g_max_number;
const int CONST_G_DIR[4][2] = {{-1,0}, {0,-1}, {1,0}, {0,1}};
const int CONST_G_ZONE[ROW][COL] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                    {0, 3, 3, 4, 4, 0,12,12,11,11, 0},
                                    {0, 2, 0, 0 ,5, 0,13, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 5, 0,13, 0, 0, 0, 0},
                                    {0, 7, 7, 6, 6, 0,14,20,20,19, 0},
                                    {0, 8, 0, 0, 0, 0,15, 0, 0,18, 0},
                                    {0, 8, 0, 0,11, 0,15, 0, 0,18, 0},
                                    {0, 9, 9,10,10, 0,16,16,17,17, 0},
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

int main(){
    cell field[ROW][COL] = {cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(),
                            cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(),
                            cell(1), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(19),
                            cell(5), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(19),
                            cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(),
                            cell(1), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(19),
                            cell(3), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(4),
                            cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(), cell(),
                            cell(), cell(), cell(5), cell(1), cell(), cell(), cell(), cell(14), cell(14), cell(), cell(),};
    
    cout << setw(50) <<" Starting field" << endl;
    print_field(field);

    int notused = 0;
    g_max_number = find_max_fixed_number(field, 0, notused, notused);
    solve(field);

    cout << endl << setw(50) <<" Solved field" << endl;
    print_field(field);
    return 0;
}

/*  ---------------------------------------------------------------------[<]-
    Function: check_cell
    Synopsis: Checks neighboring cells have same value with a given value.
 ---------------------------------------------------------------------[>]-*/
 bool check_cell(cell field[ROW][COL], int value, int start_row, int start_col){
    for(int i = 0; i < 4; i++){
        int new_row = start_row + CONST_G_DIR[i][0];
        int new_col = start_col + CONST_G_DIR[i][1];

        if(field[new_row][new_col].get_value() == value) return true;
    }
    return false;
}

/*  ---------------------------------------------------------------------[<]-
    Function: count_connect_numbers
    Synopsis: Counts all connected cells with the same value starting from
              a given coordinate using BFS.
 ---------------------------------------------------------------------[>]-*/
 int count_connect_numbers(cell field[ROW][COL], int value, int start_row, int start_col){
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
    Function: connect_same_fixed_number_bfs
    Synopsis: Find and connect same fixed numbers with same value starting 
              from a given coordinate using BFS and function connect_path.
 ---------------------------------------------------------------------[>]-*/
 bool connect_same_fixed_number_bfs(cell field[ROW][COL], int value, int start_row, int start_col){
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
                   count_connect_numbers(field, value, start_row, start_col) != value){
                    
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
                    if(connect_path(field, value, start_row, start_col, new_row, new_col)){
                        field[start_row][start_col].set_connected();
                        field[new_row][new_col].set_connected();
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
 bool connect_path(cell field[ROW][COL], int value, int start_row, int start_col, int finish_row, int finish_col) {
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
 bool can_place(int row, int col){
    if(row < 0 || col < 0) return false;
    else if(row >= ROW || col >= COL) return false;
    else return true; 
}

/*  ---------------------------------------------------------------------[<]-
    Function: find_max_fixed_number
    Synopsis: Finds the largest fixed number in the field that is less 
              than given current maximum number.
 ---------------------------------------------------------------------[>]-*/
 int find_max_fixed_number(cell field[ROW][COL], int currect_max_number, int &row, int &col){
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
 int find_min_fixed_number(cell field[ROW][COL], int current_min_number, int &row, int &col){
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
 bool find_another_fixed_number(cell field[ROW][COL], int number, int &row, int &col){
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
bool find_zone(cell field[ROW][COL], int &row, int &col){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(CONST_G_ZONE[i][j] != 0 && ((i > row) || (i == row && j > col)) && field[i][j].get_value() == 0){
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
 bool fill_fixed_numbers(cell field[ROW][COL], int connected, int value, int start_row, int start_col) {
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
                count = count_connect_numbers(field, value, start_row, start_col);
                
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
bool fill_zone(cell field[ROW][COL], int start_row, int start_col){
    int near_row;
    int near_col;
    bool two_cell_zone = indetefy_zone(start_row, start_col, near_row, near_col);
    if(two_cell_zone){
        if(CONST_G_ZONE[start_row][start_col] < 5){
            field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col] - 1);
            field[near_row][near_col].set_value(1);
            field[start_row][start_col].set_fixed();
            field[near_row][near_col].set_fixed();
            return true;
        } 

        else if(CONST_G_ZONE[start_row][start_col] >= 5 && CONST_G_ZONE[start_row][start_col] <= 10){
            if(CONST_G_ZONE[start_row][start_col] % 2 == 0){
                field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col] / 2);
                field[near_row][near_col].set_value(CONST_G_ZONE[start_row][start_col] / 2);
                field[start_row][start_col].set_fixed();
                field[near_row][near_col].set_fixed();
                return true;
            } 

            else {
                if(check_cell(field, CONST_G_ZONE[start_row][start_col] / 2 + 1, near_row, near_col) && 
                   check_cell(field, CONST_G_ZONE[start_row][start_col] / 2, start_row, start_col)){
                    int tmp = start_row;
                    start_row = near_row;
                    near_row = tmp;

                    tmp = start_col;
                    start_col = near_col;
                    near_col = tmp;
                }
                field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col] / 2 + 1);
                field[near_row][near_col].set_value(CONST_G_ZONE[start_row][start_col] / 2);
                field[start_row][start_col].set_fixed();
                field[near_row][near_col].set_fixed();
                return true;
            }
        } 

        else if(CONST_G_ZONE[start_row][start_col] >= 11 && CONST_G_ZONE[start_row][start_col] <= 20){       
            int fixed_row = 0;
            int fixed_col = 0;
            int fixed_num = find_max_fixed_number(field, CONST_G_ZONE[start_row][start_col], fixed_row, fixed_col);
            if(abs(fixed_row - start_row) + abs(fixed_col - start_col) < fixed_num / 2 + 1 || 
               abs(fixed_row - near_row) + abs(fixed_col - near_col) < fixed_num / 2 + 1){
                if(fixed_num == g_max_number) {
                    fixed_num = find_max_fixed_number(field, CONST_G_ZONE[start_row][start_col] - 1, fixed_row, fixed_col);
                }
                if(CONST_G_ZONE[start_row][start_col] != 20){
                if((check_cell(field, CONST_G_ZONE[start_row][start_col] - fixed_num, near_row, near_col) || 
                    check_cell(field, fixed_num, start_row, start_col))){
                    int tmp = start_row;
                    start_row = near_row;
                    near_row = tmp;

                    tmp = start_col;
                    start_col = near_col;
                    near_col = tmp;
                }
            }
                
                field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col]- fixed_num);
                field[near_row][near_col].set_value(fixed_num);
                field[start_row][start_col].set_fixed();
                field[near_row][near_col].set_fixed();
                return true;
            }

            else {
                if(CONST_G_ZONE[start_row][start_col] % 2 == 0){
                    field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col] / 2);
                    field[near_row][near_col].set_value(CONST_G_ZONE[start_row][start_col] / 2);
                    field[start_row][start_col].set_fixed();
                    field[near_row][near_col].set_fixed();
                    return true;
                } 
                
                else {
                    if(check_cell(field, CONST_G_ZONE[start_row][start_col] / 2 + 1, near_row, near_col) && 
                       check_cell(field, CONST_G_ZONE[start_row][start_col] / 2, start_row, start_col)){
                        int tmp = start_row;
                        start_row = near_row;
                        near_row = tmp;
    
                        tmp = start_col;
                        start_col = near_col;
                        near_col = tmp;
                    }
                    field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col] / 2 + 1); 
                    field[near_row][near_col].set_value(CONST_G_ZONE[start_row][start_col] / 2);
                    field[start_row][start_col].set_fixed();
                    field[near_row][near_col].set_fixed();
                    return true;
                }
            }
        }
    } 
    
    else {
        field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col]);
        field[start_row][start_col].set_fixed();
        return true;
    }
    return false;
}

/*  ---------------------------------------------------------------------[<]-
    Function: indetefy_zone
    Synopsis: Checks whether the cell size is 1x1 or 2x1
 ---------------------------------------------------------------------[>]-*/
bool indetefy_zone(int row, int col, int &near_row, int &near_col){
    for(int i = 0; i < 4;i++){
        if((CONST_G_ZONE[row + CONST_G_DIR[i][0]][col + CONST_G_DIR[i][1]] == CONST_G_ZONE[row][col])){
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
void solve(cell field[ROW][COL]){
    int row = 0;
    int col = 0;
    int near_x = 0;
    int near_y = 0;
    int max = 0;
    stack<int> max_numbers;

    while(find_zone(field, row, col)){
        if(!indetefy_zone(row,col,near_x,near_y))
            fill_zone(field, row,col);
    }

    reset(row,col);
    while(find_zone(field, row, col)){
        fill_zone(field, row,col);
    }
    
    while((max = find_max_fixed_number(field, max, row, col)) != 1){
        while(connect_same_fixed_number_bfs(field, max, row,col));
        max_numbers.push(max);
    }

    max = 0;
    reset(row,col);
    max = find_max_fixed_number(field, max, row, col);
    fill_fixed_numbers(field, count_connect_numbers(field, max, row, col), max, row ,col);
    
    reset(row,col);
    int min = max_numbers.top();
    max_numbers.pop();
    while(min != max){
        min = find_min_fixed_number(field, min, row,col);
        fill_fixed_numbers(field, count_connect_numbers(field, min, row, col), min, row ,col);
        while(find_another_fixed_number(field, min, row, col)){
            fill_fixed_numbers(field, count_connect_numbers(field, min, row, col), min, row ,col);
        }
        min = max_numbers.top();
        max_numbers.pop();
    }
}   

/*  ---------------------------------------------------------------------[<]-
    Function: print_field
    Synopsis: Print field in console.
 ---------------------------------------------------------------------[>]-*/
void print_field(cell field[ROW][COL]){
    for(int i = 0; i < ROW; i++){
        cout << "----------";
    }
    for(int i = 0; i < ROW; i++){
        cout << endl << '|';
        for(int j = 0; j < COL; j++){
            if(j == COL - 1) cout << "|" << setw(4) << field[i][j].get_value() << setw(4);
            else if(j == 0) cout << setw(4) << field[i][j].get_value() << setw(4);
            else cout << "|" << setw(4) << field[i][j].get_value()  << setw(4);
        }
        cout << '|' << endl;
        for(int i = 0; i < ROW; i++){
            cout << "----------";
        }
    }

    cout << endl;
}

/*  ---------------------------------------------------------------------[<]-
    Function: reset
    Synopsis: Set input value to 0.
 ---------------------------------------------------------------------[>]-*/
void reset(int &a, int &b){a = 0; b = 0;}