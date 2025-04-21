#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#define ROW 9
#define COL 11

const int dir[4][2] = {{-1,0}, {0,-1}, {0,1}, {1,0}};

using namespace std;

struct coord{
    int x;
    int y;
    coord(int x = 0, int y = 0){
        this->x = x;
        this->y = y;
    }
};

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

bool find_zone(cell field[ROW][COL], int &row, int &col);
bool indetefy_zone(int row, int col, int &near_row, int &near_col);
bool can_place(int row, int col);
int find_max_fixed_number(cell field[ROW][COL], int max_number, int &row, int &col);
bool find_empty_cell(cell field[ROW][COL], int &row, int &col);
bool find_another_max_fixed_number(cell field[ROW][COL], int max_number, int &row, int &col);
int count_empty_cell(cell field[ROW][COL], int start_row, int start_col);
int count_connect_numbers(cell field[ROW][COL], int value, int start_row, int start_col);
bool connect_same_fixed_number_bfs(cell field[ROW][COL], int value, int start_row, int start_col);
bool connect_path(cell field[ROW][COL], int value, int start_row, int start_col, int finish_row, int finish_col);
bool fill_fixed_numbers(cell field[ROW][COL], int connected, int value, int start_row, int start_col);
bool fill_zone(cell field[ROW][COL], int start_row, int start_col);
bool fill_empty_cell(cell field[ROW][COL], int value, int start_row, int start_col);
bool check_nearby(cell field[ROW][COL], int value, int start_row, int start_col);
bool check_cell(cell field[ROW][COL], int value, int start_row, int start_col);
void solve(cell field[ROW][COL]);
void print_field(cell field[ROW][COL]);

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
    
    cout <<" Starting field" << endl;
    print_field(field);

    solve(field);

    cout <<" Solved field" << endl;
    print_field(field);
    return 0;
}

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

bool indetefy_zone(int row, int col, int &near_row, int &near_col){
    for(int i = 0; i < 4;i++){
        if((CONST_G_ZONE[row + dir[i][0]][col + dir[i][1]] == CONST_G_ZONE[row][col])){
            near_row = row + dir[i][0];
            near_col = col + dir[i][1];
            return true;
        } 
    }
    near_row = 0;
    near_col = 0;
    return false;
}

bool can_place(int row, int col){
    if(row < 0 || col < 0) return false;
    else if(row >= ROW || col >= COL) return false;
    else return true; 
}

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

bool find_empty_cell(cell field[ROW][COL], int &row, int &col){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(field[i][j].get_value() == 0 && !field[i][j].get_connected() && (i > row)){
                row = i;
                col = j;
                return true;
            } 
        }
    }
    return false;
}

int count_empty_cell(cell field[ROW][COL], int start_row, int start_col){
    queue<coord> search;
    bool visited[ROW][COL] = {false};
    int count = 1;

    search.push(coord(start_row, start_col));
    visited[start_row][start_col] = true;

    while(!search.empty()){
        coord currect = search.front();
        search.pop();

        for(int i = 0; i < 4; i++){
            int new_row = currect.x + dir[i][0];
            int new_col = currect.y + dir[i][1];

            if(!field[new_row][new_col].get_connected() && !visited[new_row][new_col] 
               && can_place(new_row, new_col) && field[new_row][new_col].get_value() == 0){
                visited[new_row][new_col] = true;
                search.push(coord(new_row, new_col));
                field[new_row][new_col].set_connected();
                count++;
            }
        }
    }
    return count;
}

bool find_another_max_fixed_number(cell field[ROW][COL], int max_number, int &row, int &col){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(field[i][j].get_value() == max_number && !field[i][j].get_connected() && (i > row && j > col)){ 
                row = i;
                col = j;
                return true;
            }
        }
    }
    return false;
}

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
            int new_row = currect.x + dir[i][0];
            int new_col = currect.y + dir[i][1];

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

bool connect_same_fixed_number_bfs(cell field[ROW][COL], int value, int start_row, int start_col){
    queue<coord> search;
    bool visited[ROW][COL] = {false};

    search.push(coord(start_row, start_col));
    visited[start_row][start_col] = true;

    while(!search.empty()){
        coord currect = search.front();
        search.pop();

        for(int i = 0; i < 4; i++){
            int new_row = currect.x + dir[i][0];
            int new_col = currect.y + dir[i][1];

            //cout << start_row << " , " << start_col << " -> " << new_row << " , " << new_col << endl;

            if((abs(start_row - new_row) + abs(start_col - new_col)) > value) continue;

            if(can_place(new_row, new_col) && !visited[new_row][new_col]){
                visited[new_row][new_col] = true;
                search.push(coord(new_row, new_col));
                
                if(field[new_row][new_col].get_value() == value &&
                   field[new_row][new_col].get_fixed() &&
                   !field[new_row][new_col].get_connected() &&
                   (new_row != start_row || new_col != start_col)){

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

    if (start_col == finish_col) {
        int row_start = min(start_row, finish_row);
        int row_end = max(start_row, finish_row);
        for (int row = row_start + 1; row < row_end; row++) {
            if (field[row][start_col].get_fixed() && field[row][start_col].get_value() != value) return false; 
            field[row][start_col].set_value(value);
            field[row][start_col].set_connected();;
        }
        return true;
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
                field[row][finish_col].set_connected();;
            }
            return true;
        }
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
    return false; 
}

bool fill_fixed_numbers(cell field[ROW][COL], int connected, int value, int start_row, int start_col) {
    deque<coord> queue;
    int count = 0;

    queue.push_front(coord(start_row, start_col));
    
    while (count < value - connected && !queue.empty()) {
        coord current = queue.front();
        queue.pop_front();

        for (int i = 0; i < 4; i++) { 
            int new_row = current.x + dir[i][0];
            int new_col = current.y + dir[i][1];
            //cout <<"Current: "<< current.x << " , " << current.y << endl;

            while (can_place(new_row, new_col) && field[new_row][new_col].get_value() == 0) {
                if(check_nearby(field, value, new_row, new_col)){
                    field[new_row][new_col].set_value(value);
                    field[new_row][new_col].set_connected();
                    count++;
                } else{
                    if(count == value - connected - 1){
                        field[new_row][new_col].set_value(value);
                        field[new_row][new_col].set_connected();
                        count++;
                    } 
                    else break;
                }
                if (count == value - connected) return true;
                queue.push_front(coord(new_row, new_col));
                
                new_row += dir[i][0];
                new_col += dir[i][1];

                if(can_place(new_row + dir[i][0], new_col + dir[i][1])) i = 0;
            }
            if(queue.size() >= 1)current = queue.front();
            //cout <<"Turn: "<< current.x << " , " << current.y << endl;
        }
    }
    return false;
}

bool fill_empty_cell(cell field[ROW][COL], int value, int start_row, int start_col){
    deque<coord> queue;
    int count = 0;
    if(value == 1){
        field[start_row][start_col].set_value(value);
        field[start_row][start_col].set_connected();
        return true;
    }

    queue.push_front(coord(start_row, start_col));
    
    while (count < value && !queue.empty()) {
        coord current = queue.front();
        queue.pop_front();

        for (int i = 0; i < 4; i++) { 
            int new_row = current.x + dir[i][0];
            int new_col = current.y + dir[i][1];
            //cout <<"Current: "<< current.x << " , " << current.y << endl;

            while (can_place(new_row, new_col) && field[new_row][new_col].get_value() == 0) {
                field[new_row][new_col].set_value(value);
                field[new_row][new_col].set_connected();
                count++;

                if (count == value) return true;
                queue.push_front(coord(new_row, new_col));
                
                new_row += dir[i][0];
                new_col += dir[i][1];

                if(can_place(new_row + dir[i][0], new_col + dir[i][1])) i = 0;
            }
            if(queue.size() >= 1)current = queue.front();
            //cout <<"Turn: "<< current.x << " , " << current.y << endl;
        }
    }
    return false;
}

bool fill_zone(cell field[ROW][COL], int start_row, int start_col){
    int near_row;
    int near_col;
    bool two_cell_zone = indetefy_zone(start_row, start_col, near_row, near_col);
    if(two_cell_zone){
        if(CONST_G_ZONE[start_row][start_col] < 5){
            field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col] - 1);
            field[near_row][near_col].set_value(1);
            return true;
        } else if(CONST_G_ZONE[start_row][start_col] >= 5 && CONST_G_ZONE[start_row][start_col] <= 10){
            if(CONST_G_ZONE[start_row][start_col] % 2 == 0){
                field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col] / 2);
                field[near_row][near_col].set_value(CONST_G_ZONE[start_row][start_col] / 2);
                return true;
            } else {
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
                return true;
            }
        } else if(CONST_G_ZONE[start_row][start_col] >= 11 && CONST_G_ZONE[start_row][start_col] <= 20){       
            int fixed_row = 0;
            int fixed_col = 0;
            int fixed_num = find_max_fixed_number(field, CONST_G_ZONE[start_row][start_col], fixed_row, fixed_col);
            if(abs(fixed_row - start_row) + abs(fixed_col - start_col) < fixed_num / 2 + 1 || 
               abs(fixed_row - near_row) + abs(fixed_col - near_col) < fixed_num / 2 + 1){
                if(fixed_num == 19) {
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
            } else {
                if(CONST_G_ZONE[start_row][start_col] % 2 == 0){
                    field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col] / 2);
                    field[near_row][near_col].set_value(CONST_G_ZONE[start_row][start_col] / 2);
                    return true;
                } else {
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
                    return true;
                }
            }
        }
    } else {
        field[start_row][start_col].set_value(CONST_G_ZONE[start_row][start_col]);
        field[start_row][start_col].set_fixed();
        return true;
    }
    return false;
}

bool check_cell(cell field[ROW][COL], int value, int start_row, int start_col){
    for(int i = 0; i < 4; i++){
        int new_row = start_row + dir[i][0];
        int new_col = start_col + dir[i][1];

        if(field[new_row][new_col].get_value() == value) return true;
    }
    return false;
}

bool check_nearby(cell field[ROW][COL], int value, int start_row, int start_col){
    for(int i = 0; i < 4; i++){
        int new_row = start_row + dir[i][0];
        int new_col = start_col + dir[i][1];

        if(count_connect_numbers(field, value, new_row, new_col) == value) return false;
    }
    return true;
}

void solve(cell field[ROW][COL]){
    int x = 0;
    int y = 0;
    int near_x = 0;
    int near_y = 0;
    //int max = 0;

    while(find_zone(field, x, y)){
        if(!indetefy_zone(x,y,near_x,near_y))
            fill_zone(field, x,y);
    }
    x = 0;
    y = 0;
    while(find_zone(field, x, y)){
        fill_zone(field, x,y);
    }
    /*while((max = find_max_fixed_number(field, max, x, y)) != 1){
        while(connect_same_fixed_number_bfs(field, max, x,y));
        //cout <<"Count connected numbers: "<< count_connect_numbers(field, max, x, y) << endl;
        fill_fixed_numbers(field, count_connect_numbers(field, max, x, y), max, x ,y);
        //cout << max <<"  |" << x << " , " << y <<endl;
        while(find_another_max_fixed_number(field, max, x, y)){
           // cout << max <<"  |" << x << " , " << y <<endl;
            connect_same_fixed_number_bfs(field, max, x, y);
            fill_fixed_numbers(field, count_connect_numbers(field, max, x, y), max, x, y);
        }
    }
    int count;
    x = 0;
    y = 0;
    while(find_empty_cell(field, x,y)){
        cout << "coord empty" <<"  |" << x << " , " << y <<endl;
        count = count_empty_cell(field, x, y);
        cout << "Count empty cell: " << count <<endl;
        fill_empty_cell(field, count, x,y);
    }
        //cout << max <<"  |" << x << " , " << y <<endl;
        //cout <<"Count connected numbers: "<< count_connect_numbers(field, max, x, y) << endl;*/
}   

void print_field(cell field[ROW][COL]){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            cout << setw(2) << field[i][j].get_value() << " ";
        }
        cout << endl;
    }
}