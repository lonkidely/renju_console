#ifndef RENJU_CONSOLE_GAMELOGIC_H
#define RENJU_CONSOLE_GAMELOGIC_H

#include "Field.h"

enum PlayerType : int {
    HUMAN,
    BOT
};

enum PlayerColor : int {
    WHITE,
    BLACK
};

struct Cell {
    int x;
    int y;
    Cell();
    Cell(int _x, int _y);
};

struct Segment {
    Cell first;
    Cell second;
    int segmentLength();
    Segment();
    Segment(Cell _first, Cell _second);
};

enum SegmentOrientation : int {
    HORIZONTAL,
    VERTICAL,
    MAIN_DIAGONAL,
    SECONDARY_DIAGONAL
};

class GameLogic {
public:
    static bool isCorrectCoords(int x, int y);

    static bool isCorrectCoords(Field &field, int x, int y, PlayerColor playerColor);

    static bool isCorrectSecondMove(Field &field, int x, int y);

    static bool isCorrectThirdMove(Field &field, int x, int y);

    static bool gameIsOver(Field &field);

    static Segment findSegmentEnds(Field &field, int startX, int startY, SegmentOrientation orientation);

    static int canSegmentExtends(Field &field, Segment segment, SegmentOrientation orientation);
};


#endif //RENJU_CONSOLE_GAMELOGIC_H
