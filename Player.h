#ifndef RENJU_CONSOLE_PLAYER_H
#define RENJU_CONSOLE_PLAYER_H

#include "GameLogic.h"
#include "Field.h"
#include "AI.h"

///Класс игрока, в зависимости от его PlayerType-а и PlayerColor-а имеет свои особенности у возможных ходов
class Player {
private:
    PlayerType playerType;
    PlayerColor playerColor;

    ///Количество ходов, сделанных игроком за время игры
    int movesCount;

    void firstMove(Field &field);
    void secondMove(Field &field);
    void thirdMove(Field &field);

    ///"Обычный" ход, т.е. практически в любую ячейку на поле
    void ordinaryMove(Field &field);

public:
    Player();
    Player(PlayerType _playerType, PlayerColor _playerColor);

    ///В зависимости от цвета игрока выбирает правильную последовательность вызова методов first/second/third Move
    void move(Field &field);

    int getMovesCount();
};


#endif //RENJU_CONSOLE_PLAYER_H
