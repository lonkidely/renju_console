#include "AI.h"

Attack::Attack() : x(0), y(0), power(0), potential(0) {}

Attack::Attack(int _x, int _y, int _power, int _potential) : x(_x), y(_y), power(_power), potential(_potential) {}

Attack Attack::operator=(Attack right) {
    this->x = right.x;
    this->y = right.y;
    this->potential = right.potential;
    this->power = right.power;
    return *this;
}

bool Attack::operator<(const Attack &right) const {
    int attackCostLeft = attackCost[this->power][this->potential], attackCostRight = attackCost[right.power][right.potential];
    return attackCostLeft < attackCostRight;
}

int Attack::getAttackCost() {
    return attackCost[power][potential];
}

Attack AI::bestPosition(Field &field, int x, int y, CellOwner cellOwner) {
    Attack best;
    field.setCellValue(x, y, cellOwner);
    for (int i = 0; i < field.getFieldSize(); ++i) {
        for (int j = 0; j < field.getFieldSize(); ++j) {
            if (field.at(i, j) == cellOwner) {
                //Считаем все возможные отрезки, которые проходят через эту точку
                Segment horizontal = GameLogic::findSegmentEnds(field, i, j, HORIZONTAL), vertical = GameLogic::findSegmentEnds(field, i, j, VERTICAL),
                        main_diagonal = GameLogic::findSegmentEnds(field, i, j, MAIN_DIAGONAL), secondary_diagonal = GameLogic::findSegmentEnds(field, i, j, SECONDARY_DIAGONAL);

                //Перебираем все отрезки, смотрим на их длину и количество пустых смежных клеток, обновляем лучший результат
                best = max(best, Attack(x, y, horizontal.segmentLength(), GameLogic::canSegmentExtends(field, horizontal, HORIZONTAL)));
                best = max(best, Attack(x, y, vertical.segmentLength(), GameLogic::canSegmentExtends(field, vertical, VERTICAL)));
                best = max(best, Attack(x, y, main_diagonal.segmentLength(), GameLogic::canSegmentExtends(field, main_diagonal, MAIN_DIAGONAL)));
                best = max(best, Attack(x, y, secondary_diagonal.segmentLength(), GameLogic::canSegmentExtends(field, secondary_diagonal, SECONDARY_DIAGONAL)));
            }
        }
    }
    field.eraseCellValue(x, y);
    return best;
}

Cell AI::bestMove(Field &field, PlayerColor playerColor) {
    Attack bestAttack, bestDefense;

    for (int i = 0; i < field.getFieldSize(); ++i) {
        for (int j = 0; j < field.getFieldSize(); ++j) {
            if (field.at(i, j) != EMPTY) continue;

            if (playerColor == WHITE) {
                bestAttack = max(bestAttack, bestPosition(field, i, j, FIRST_PLAYER));
                if (GameLogic::isCorrectCoords(field, i, j, BLACK))
                    bestDefense = max(bestDefense, bestPosition(field, i, j, SECOND_PLAYER));
            } else {
                if (GameLogic::isCorrectCoords(field, i, j, BLACK))
                    bestAttack = max(bestAttack, bestPosition(field, i, j, SECOND_PLAYER));
                bestDefense = max(bestDefense, bestPosition(field, i, j, FIRST_PLAYER));
            }
        }
    }

    return (bestDefense.getAttackCost() > bestAttack.getAttackCost() ? Cell(bestDefense.x, bestDefense.y) : Cell(bestAttack.x, bestAttack.y));
}