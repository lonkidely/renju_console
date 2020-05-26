#ifndef RENJU_CONSOLE_FIELD_H
#define RENJU_CONSOLE_FIELD_H

#include <iostream>
#include <iomanip>
#include <random>

using namespace std;

///Перечисление возможных владельцев ячейки: либо ячейка никому не принадлежит, либо принадлежит первому/второму игроку
enum CellOwner : int {
    EMPTY,
    FIRST_PLAYER,
    SECOND_PLAYER
};

///Класс поля, содержит в себе матрицу размера FIELD_SIZE x FIELD_SIZE (по умолчанию 15х15) и вспомогательные методы
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

    ///Доступ к ячейке по её координатам
    CellOwner &at(int i, int j);

};

#endif //RENJU_CONSOLE_FIELD_H
