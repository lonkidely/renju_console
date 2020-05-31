#ifndef RENJU_CONSOLE_AI_H
#define RENJU_CONSOLE_AI_H

#include "GameLogic.h"
#include <algorithm>

enum MoveType : int {
    ATTACKING,
    POSITIONAL
};

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

struct NeighbourMatrix{
    vector<Cell> neighbours_level1;
    vector<Cell> neighbours_level2;
    vector<Cell> neighbours_level3;
    void clear();
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

    ///Подсчитаем матрицу соседей(двоек) для каждой непустой ячейки, содержающую информацию о соседях на расстояниях 1, 2 и 3 ячейки
    static void countNeighbourMatrix(Field &field, NeighbourMatrix (&matrix)[15][15]);

    ///Подсчитаем, как много новых двоек появилось, если их стало больше, чем на 1, то ход - позиционный, иначе - атакующий
    static MoveType getMoveType(Field &field, NeighbourMatrix (&before)[15][15], NeighbourMatrix (&after)[15][15], PlayerColor playerColor);

    ///Перебираем все возможные ячейки, если ход в эту ячейку соответствует типу хода, добавляем в список, затем сортируем список
    ///в зависимости от типа хода, возвращаем максимальный по силе/потенциалу
    static Attack attackingAlgorithm(Field &field, PlayerColor playerColor, MoveType moveType);

    ///Проанализируем ход соперника и выполним либо позиционный, либо атакующий алгоритм, либо, если вдруг не получится воспользоваться ни одним из них,
    ///выполним старый алгоритм поиска оптимального хода bestMove и вернем его результат
    static Cell bestSmartMove(Field &field, PlayerColor playerColor);
};


#endif //RENJU_CONSOLE_AI_H
