#ifndef RENJU_CONSOLE_PLAYER_H
#define RENJU_CONSOLE_PLAYER_H

#include "GameLogic.h"
#include "Field.h"
#include "AI.h"

class Player {
private:
    PlayerType playerType;
    PlayerColor playerColor;
    int MovesCount;
public:
    Player();
    Player(PlayerType _playerType, PlayerColor _playerColor);

    void firstMove(Field &field);
    void secondMove(Field &field);
    void thirdMove(Field &field);
    void ordinaryMove(Field &field);
    void move(Field &field);

    int getMovesCount();
};


#endif //RENJU_CONSOLE_PLAYER_H
