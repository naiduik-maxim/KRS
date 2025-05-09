#include <iostream>
#include "cell.hpp"

using namespace std;

cell :: cell (){value = 0; fixed = false; connected = false;}
cell :: cell(int v){value = v; fixed = true; connected = false;}
int cell :: get_value() {return value;}
bool cell :: get_fixed() {return fixed;}
bool cell :: get_connected() {return connected;}
void cell :: set_connected() {connected = true;}
void cell :: set_value(int v) {value = v;}
void cell :: set_fixed() {fixed = true;}