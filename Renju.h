#ifndef RENJU_CONSOLE_RENJU_H
#define RENJU_CONSOLE_RENJU_H

#include <iomanip>
#include "Player.h"
#include "GameLogic.h"
#include "AI.h"
#include "Field.h"

///Класс игры, по сути своей - интерфейс. Вызывает методы игроков и проверяет, не закончилась ли игра.
/// В случае, если игра закончилась, выводит победителя и количество завершенных ходов.
class Renju {
private:
    //само поле
    Field field;

    //Игроки
    Player firstPlayer;
    Player secondPlayer;

public:
    Renju();

    void displayField();

    void play();

    CellOwner getWinner();
};


#endif //RENJU_CONSOLE_RENJU_H
