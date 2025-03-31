#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#define ROW 9
#define COL 11

const int dir[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

using namespace std;

struct coord
{
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
};

bool can_place(int row, int col, cell obj);
int find_max_fixed_number(cell field[ROW][COL], int max_number, int &row, int &col);
bool find_same_fixed_number_BFS(cell field[ROW][COL], int value, int start_row, int start_col);
void solve(cell field[ROW][COL]);
void print_field(cell field[ROW][COL]);

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

    return 0;
}

bool can_place(int row, int col, cell obj){
    if(row < 0 || col < 0) return false;
    else if(row > ROW || col > 0) return false;
    else if(obj.get_fixed()) return false;
    else return true; 
}

int find_max_fixed_number(cell field[ROW][COL], int currect_max_number, int &row, int &col){
    int max_num = 0;
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(currect_max_number !=0){
                if(field[i][j].get_value() > max_num && field[i][j].get_value() < currect_max_number){
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

bool find_same_fixed_number_BFS(cell field[ROW][COL], int value, int start_row, int start_col){
    queue<coord> search;
    bool visited[ROW][COL] = {false};

    search.push(coord(start_row, start_col));
    visited[start_row][start_col] = true;

    coord currect = search.front();
    search.pop();
    while(currect.x + currect.y < value){
        currect = search.front();
        search.pop();
        for(int i = 0; i < 4; i++){
            int new_row = currect.x + dir[i][0];
            int new_col = currect.y + dir[i][1];

            if((new_row >= 0 && new_row < ROW && new_col >= 0 && new_col < COL) && !visited[new_row][new_col]){
                visited[new_row][new_col] = true;
                search.push(coord(new_row, new_col));

                if(field[new_row][new_col].get_value() == value && !field[new_row][new_col].get_connected()){
                    //Створити метод для з'єднання 
                }
            }
        }
    }
}

void print_field(cell field[ROW][COL]){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            cout << setw(2) << field[i][j].get_value() << " ";
        }
        cout << endl;
    }
}


