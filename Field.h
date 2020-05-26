#ifndef RENJU_CONSOLE_FIELD_H
#define RENJU_CONSOLE_FIELD_H

#include <iostream>
#include <iomanip>
#include <random>

using namespace std;

enum CellOwner : int {
    EMPTY,
    FIRST_PLAYER,
    SECOND_PLAYER
};


class Field {
private:
    static const int FIELD_SIZE = 15;
    int countFreeCells;
    CellOwner field[FIELD_SIZE][FIELD_SIZE];
public:

    Field();

    void displayField();

    int getFieldSize();
    int getCountFreeCells();

    void setCellValue(int x, int y, CellOwner value);
    void eraseCellValue(int x, int y);

    CellOwner &at(int i, int j);

};


#endif //RENJU_CONSOLE_FIELD_H
