#ifndef RENJU_CONSOLE_AI_H
#define RENJU_CONSOLE_AI_H

#include "GameLogic.h"

class Attack {
protected:
    int x;
    int y;
    int power;
    int potential;
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

    friend ostream& operator << (ostream& out, Attack& tmp) {
        out << tmp.x << ' ' << tmp.y << ' ' << tmp.power << ' ' << tmp.potential << ' ' << tmp.getAttackCost() << "\n";
        return out;
    }

    friend class AI;
};


class AI {
public:
    static Attack bestPosition(Field &field, int x, int y, CellOwner cellOwner);

    static Cell bestMove(Field &field, PlayerColor playerColor);
};


#endif //RENJU_CONSOLE_AI_H
