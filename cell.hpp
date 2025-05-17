#ifndef CELL_HPP
#define CELL_HPP

/*  ---------------------------------------------------------------------[<]-
    Class: cell
    Synopsis: Implements all methods for working with the playing field
 ---------------------------------------------------------------------[>]-*/
class cell {
    int value;
    bool fixed;
    bool connected;
public:
    cell();
    cell(int v);
    int get_value();
    bool get_fixed();
    bool get_connected();
    void set_connected();
    void set_value(int v);
    void set_fixed();
};

#endif