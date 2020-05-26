#ifndef RENJU_CONSOLE_AI_H
#define RENJU_CONSOLE_AI_H

#include "GameLogic.h"

///Вспомогательный класс, нужен для поиска оптимального хода компьютера.
///Содержит в себе эффективность атаки и координаты в будущем возможно поставленной фигуры, при участии которой она достигается
class Attack {
protected:
    int x;
    int y;
    int power;
    int potential;
    ///Вес атаки, значения подобраны примерно, вес атаки = attackCost[power][potential]
    const int attackCost[6][3] = {{0,     0,     0},
                                  {0,     10,    25},
                                  {0,     200,   500},
                                  {0,     400,   700},
                                  {0,     600,   10000},
                                  {20000, 20000, 20000}};
public:
    Attack();

    Attack(int _x, int _y, int _power, int _potential);

    Attack operator=(Attack right);

    bool operator<(const Attack &right) const;

    int getAttackCost();

    ///Перегрузка вывода, нужна только для debug-а
    friend ostream& operator << (ostream& out, Attack& tmp) {
        out << tmp.x << ' ' << tmp.y << ' ' << tmp.power << ' ' << tmp.potential << ' ' << tmp.getAttackCost() << "\n";
        return out;
    }

    friend class AI;
};

///Основной класс, содержит в себе методы для поиска наилучшего хода компьютера
class AI {
public:
    ///Метод, вычисляющий эффективность атаки игрока при условии, что будет поставлена фигура в ячейку (x, y)
    static Attack attackEfficiency(Field &field, int x, int y, CellOwner cellOwner);
    ///Метод, ищущий оптимальный ход: уйти в защиту либо атаковать. Для этого подсчитаем эффективность защиты - переберем
    /// все пустые ячейки поля, мысленно подставим туда фигуру противника, посчитаем его наилучшую атаку (т.е. нашу лучшую защиту),
    /// затем найдем аналогично наилучшую атаку для своих фигур, и
    /// в зависимости от весов найденных атак уже сделаем выбор -  либо защищаться, либо атаковать
    static Cell bestMove(Field &field, PlayerColor playerColor);
};


#endif //RENJU_CONSOLE_AI_H
