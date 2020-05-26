#include "GameLogic.h"

Cell::Cell() {}

Cell::Cell(int _x, int _y) : x(_x), y(_y) {}

Segment::Segment() {}

Segment::Segment(Cell _first, Cell _second) : first(_first), second(_second) {}

int Segment::segmentLength() {
    return max(abs(first.x - second.x), abs(first.y - second.y)) + 1;
}

Segment GameLogic::findSegmentEnds(Field &field, int startX, int startY, SegmentOrientation orientation) {

    Cell first(startX, startY), second(startX, startY);

    switch (orientation) {
        case HORIZONTAL:
            while (isCorrectCoords(first.x - 1, first.y) && field.at(first.x - 1, first.y) == field.at(startX, startY)) first.x--;
            while (isCorrectCoords(second.x + 1, second.y) && field.at(second.x + 1, second.y) == field.at(startX, startY)) second.x++;
            break;
        case VERTICAL:
            while (isCorrectCoords(first.x, first.y - 1) && field.at(first.x, first.y - 1) == field.at(startX, startY)) first.y--;
            while (isCorrectCoords(second.x, second.y + 1) && field.at(second.x, second.y + 1) == field.at(startX, startY)) second.y++;
            break;
        case MAIN_DIAGONAL:
            while (isCorrectCoords(first.x - 1, first.y - 1) && field.at(first.x - 1, first.y - 1) == field.at(startX, startY)) first.x--, first.y--;
            while (isCorrectCoords(second.x + 1, second.y + 1) && field.at(second.x + 1, second.y + 1) == field.at(startX, startY)) second.x++, second.y++;
            break;
        default:
            while (isCorrectCoords(first.x - 1, first.y + 1) && field.at(first.x - 1, first.y + 1) == field.at(startX, startY)) first.x--, first.y++;
            while (isCorrectCoords(second.x + 1, second.y - 1) && field.at(second.x + 1, second.y - 1) == field.at(startX, startY)) second.x++, second.y--;
            break;
    }

    return Segment(first, second);
}

int GameLogic::canSegmentExtends(Field &field, Segment segment, SegmentOrientation orientation) {
    int countDirection = 0;
    switch (orientation) {
        case HORIZONTAL:
            if (isCorrectCoords(segment.first.x - 1, segment.first.y) &&
                field.at(segment.first.x - 1, segment.first.y) == EMPTY) countDirection++;
            if (isCorrectCoords(segment.second.x + 1, segment.second.y) &&
                field.at(segment.second.x + 1, segment.second.y) == EMPTY) countDirection++;
            break;
        case VERTICAL:
            if (isCorrectCoords(segment.first.x, segment.first.y - 1) &&
                field.at(segment.first.x, segment.first.y - 1) == EMPTY) countDirection++;
            if (isCorrectCoords(segment.second.x, segment.second.y + 1) &&
                field.at(segment.second.x, segment.second.y + 1) == EMPTY) countDirection++;
            break;
        case MAIN_DIAGONAL:
            if (isCorrectCoords(segment.first.x - 1, segment.first.y - 1) &&
                field.at(segment.first.x - 1, segment.first.y - 1) == EMPTY) countDirection++;
            if (isCorrectCoords(segment.second.x + 1, segment.second.y + 1) &&
                field.at(segment.second.x + 1, segment.second.y + 1) == EMPTY) countDirection++;
            break;
        default:
            if (isCorrectCoords(segment.first.x - 1, segment.first.y + 1) &&
                field.at(segment.first.x - 1, segment.first.y + 1) == EMPTY) countDirection++;
            if (isCorrectCoords(segment.second.x + 1, segment.second.y - 1) &&
                field.at(segment.second.x + 1, segment.second.y - 1) == EMPTY) countDirection++;
            break;
    }
    return countDirection;
}

bool GameLogic::isCorrectSecondMove(Field &field, int x, int y) {
    return (6 <= x && x <= 8 && 6 <= y && y <= 8 && field.at(x, y) == EMPTY);
}

bool GameLogic::isCorrectThirdMove(Field &field, int x, int y) {
    return (5 <= x && x <= 9 && 5 <= y && y <= 9 && field.at(x, y) == EMPTY);
}

bool GameLogic::isCorrectCoords(int x, int y) {
    return (0 <= x && x < 15 && 0 <= y && y < 15);
}

bool GameLogic::isCorrectCoords(Field &field, int x, int y, PlayerColor playerColor) {
    //общие проверки, чтобы координаты указывали на пустую область внутри поля
    if (!isCorrectCoords(x, y)) return false;
    if (field.at(x, y) != EMPTY) return false;

    //для белых фигур ограничений больше нет
    if (playerColor == WHITE) return true;

    //Поставим мысленно черную фигуру, и проверим, не нарушаются ли условия
    field.setCellValue(x, y, SECOND_PLAYER);
    bool isCorrect = true;

    //Считаем все возможные отрезки, которые проходят через эту точку
    Segment horizontal = findSegmentEnds(field, x, y, HORIZONTAL), vertical = findSegmentEnds(field, x, y, VERTICAL),
        main_diagonal = findSegmentEnds(field, x, y, MAIN_DIAGONAL), secondary_diagonal = findSegmentEnds(field, x, y, SECONDARY_DIAGONAL);

    //Если есть хотя бы 1 длинный ряд - ставить в эту клетку фигуру нельзя
    if (horizontal.segmentLength() > 5 || vertical.segmentLength() > 5 ||
        main_diagonal.segmentLength() > 5 || secondary_diagonal.segmentLength() > 5) isCorrect = false;

    //Считаем количество троек и четверок (такие наборы из 3 и 4 фигур, которые !можно расширить! добавив ещё 1 фигуру(т.е. нужна хотя бы ещё 1 пустая клетка рядом)
    int countFreeThreesome = 0, countFreeQuad = 0;

    if (canSegmentExtends(field, horizontal, HORIZONTAL)) {
        if (horizontal.segmentLength() == 3) countFreeThreesome++;
        else if (horizontal.segmentLength() == 4) countFreeQuad++;
    }
    if (canSegmentExtends(field, vertical, VERTICAL)) {
        if (vertical.segmentLength() == 3) countFreeThreesome++;
        else if (vertical.segmentLength() == 4) countFreeQuad++;
    }
    if (canSegmentExtends(field, main_diagonal, MAIN_DIAGONAL)) {
        if (main_diagonal.segmentLength() == 3) countFreeThreesome++;
        else if (main_diagonal.segmentLength() == 4) countFreeQuad++;
    }
    if (canSegmentExtends(field, secondary_diagonal, SECONDARY_DIAGONAL)) {
        if (secondary_diagonal.segmentLength() == 3) countFreeThreesome++;
        else if (secondary_diagonal.segmentLength() == 4) countFreeQuad++;
    }

    //Если есть вилки 3х3 или 4х4 - ставить в эту клетку фигуру нельзя
    if (countFreeQuad > 1 || countFreeThreesome > 1) isCorrect = false;

    //Уберем мысленно поставленную фигуру
    field.eraseCellValue(x, y);
    return isCorrect;
}

bool GameLogic::gameIsOver(Field &field) {

    if (field.getCountFreeCells() == 0) return true;
    bool isOver = false;

    Segment currentSegment;

    for (int i = 0; i < field.getFieldSize(); ++i){
        for (int j = 0; j < field.getFieldSize(); ++j){
            if (field.at(i, j) == EMPTY) continue;
            currentSegment = findSegmentEnds(field, i, j, VERTICAL);
            if (currentSegment.segmentLength() == 5) isOver = true;

            currentSegment = findSegmentEnds(field, i, j, HORIZONTAL);
            if (currentSegment.segmentLength() == 5) isOver = true;

            currentSegment = findSegmentEnds(field, i, j, MAIN_DIAGONAL);
            if (currentSegment.segmentLength() == 5) isOver = true;

            currentSegment = findSegmentEnds(field, i, j, SECONDARY_DIAGONAL);
            if (currentSegment.segmentLength() == 5) isOver = true;
        }
    }

    return isOver;
}