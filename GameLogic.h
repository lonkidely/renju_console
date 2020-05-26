#ifndef RENJU_CONSOLE_GAMELOGIC_H
#define RENJU_CONSOLE_GAMELOGIC_H

#include "Field.h"

///Перечисление типов игроков - либо человек, либо компьютер(бот)
enum PlayerType : int {
    HUMAN,
    BOT
};

///Цвета игроков: первый игрок - белый, второй игрок - черный
enum PlayerColor : int {
    WHITE,
    BLACK
};

///Ячейка с говорящим названием, вместо pair<int, int>
struct Cell {
    int x;
    int y;

    Cell();

    Cell(int _x, int _y);
};

///Перечисление возможных ориентаций отрезка, для удобства
enum SegmentOrientation : int {
    HORIZONTAL,
    VERTICAL,
    MAIN_DIAGONAL,
    SECONDARY_DIAGONAL
};

///Отрезок - набор из стоящих вплотную друг к другу фигур - либо горизонтально, либо вертикально, либо диагонально
struct Segment {
    Cell first;
    Cell second;

    int segmentLength();

    SegmentOrientation segmentOrientation;

    Segment();

    Segment(Cell _first, Cell _second, SegmentOrientation = HORIZONTAL);
};



///Основной класс, описывающий ограничения игры и имеющий некоторые вспомогательные методы
class GameLogic {
public:
    static bool isCorrectCoords(Field &field, int x, int y);

    ///Проверка на корректность хода: для белых фигур требуется только пустая ячейка в пределах поля, для черных
    ///необходимо также учитывать правила игры
    static bool isCorrectCoords(Field &field, int x, int y, PlayerColor playerColor);

    ///Проверка на корректность второго хода: белая фигура должна стоять вплотную к первой черной - в пределах квадрата 3х3 по центру поля
    static bool isCorrectSecondMove(Field &field, int x, int y);

    ///Проверка на корректность третьего хода: черная фигура должна стоять в пределах квадрата 5х5 по центру поля
    static bool isCorrectThirdMove(Field &field, int x, int y);

    ///Проверка, закончилась ли игра: либо не осталось свободных ячеек, либо кто-то из игроков собрал пятерку - отрезок из 5 фигур
    static bool gameIsOver(Field &field);

    ///В зависимости от ориентации отрезка, находит точки, являющиеся его границами, при этом в нём содержится фигура с координатами (startX, startY)
    static Segment findSegmentEnds(Field &field, int startX, int startY, SegmentOrientation orientation);

    ///Проверяет, можно ли расширить данный отрезок, возвращает количество свободных граничных клеток (0 - отрезок "зажат")
    static int canSegmentExtends(Field &field, Segment segment);
};


#endif //RENJU_CONSOLE_GAMELOGIC_H
