#ifndef GAME_HPP
#define GAME_HPP
#define ROW 9
#define COL 11

#include"cell.hpp"

class game {
    int max_number = 0;
    int row_max = 0;
    int col_max = 0;
    int zone[ROW][COL];
    bool check_cell(int value, int start_row, int start_col);
    bool check_answer();
    int count_connect_numbers(int value, int start_row, int start_col);
    int count_empty_cell(int start_row, int start_col);
    bool connect_same_fixed_number_bfs(int value, int start_row, int start_col);
    bool connect_path(int value, int start_row, int start_col, int finish_row, int finish_col);
    bool can_place(int row, int col);
    int find_max_fixed_number(int max_number, int &row, int &col);
    int find_min_fixed_number(int current_min_number, int &row, int &col);
    bool find_zone(int &row, int &col);
    bool find_another_fixed_number(int number, int &row, int &col);
    bool find_empty_cell(int &row, int &col);
    bool fill_fixed_numbers(int connected, int value, int start_row, int start_col);
    bool fill_zone(int start_row, int start_col);
    bool fill_empty_cell(int value, int start_row, int start_col);
    bool indetefy_zone(int row, int col, int &near_row, int &near_col);
    void pause();

public:
    game();
    game(int notused);
    cell field[ROW][COL];
    void solve();
    void solve_by_user();
    void print_field(bool zones, int row = -1, int col = -1);
    bool initialized_zones();
    void clear_field();
};

#endif
