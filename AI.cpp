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

Attack AI::attackEfficiency(Field &field, int x, int y, CellOwner cellOwner) {
    Attack best;
    field.setCellValue(x, y, cellOwner);
    for (int i = 0; i < field.getFieldSize(); ++i) {
        for (int j = 0; j < field.getFieldSize(); ++j) {
            if (field.at(i, j) == cellOwner) {
                //Считаем все возможные отрезки, которые проходят через эту точку
                Segment horizontal = GameLogic::findSegmentEnds(field, i, j, HORIZONTAL),
                        vertical = GameLogic::findSegmentEnds(field, i, j, VERTICAL),
                        main_diagonal = GameLogic::findSegmentEnds(field, i, j, MAIN_DIAGONAL),
                        secondary_diagonal = GameLogic::findSegmentEnds(field, i, j, SECONDARY_DIAGONAL);

                //Перебираем все отрезки, смотрим на их длину и количество пустых смежных клеток, обновляем лучший результат
                best = max(best, Attack(x, y, horizontal.segmentLength(),
                                        GameLogic::canSegmentExtends(field, horizontal)));
                best = max(best, Attack(x, y, vertical.segmentLength(),
                                        GameLogic::canSegmentExtends(field, vertical)));
                best = max(best, Attack(x, y, main_diagonal.segmentLength(),
                                        GameLogic::canSegmentExtends(field, main_diagonal)));
                best = max(best, Attack(x, y, secondary_diagonal.segmentLength(),
                                        GameLogic::canSegmentExtends(field, secondary_diagonal)));
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
                bestAttack = max(bestAttack, attackEfficiency(field, i, j, FIRST_PLAYER));
                if (GameLogic::isCorrectCoords(field, i, j, BLACK))
                    bestDefense = max(bestDefense, attackEfficiency(field, i, j, SECOND_PLAYER));
            } else {
                if (GameLogic::isCorrectCoords(field, i, j, BLACK))
                    bestAttack = max(bestAttack, attackEfficiency(field, i, j, SECOND_PLAYER));
                bestDefense = max(bestDefense, attackEfficiency(field, i, j, FIRST_PLAYER));
            }
        }
    }

    bool isCorrectAttack = GameLogic::isCorrectCoords(field, bestAttack.x, bestAttack.y, playerColor),
            isCorrectDefense = GameLogic::isCorrectCoords(field, bestDefense.x, bestDefense.y, playerColor);

    if (!isCorrectAttack && !isCorrectDefense)
        return Cell(-1, -1);
    if (!isCorrectDefense)
        return Cell(bestAttack.x, bestAttack.y);
    if (!isCorrectAttack)
        return Cell(bestDefense.x, bestDefense.y);

    return (bestDefense.getAttackCost() > bestAttack.getAttackCost() ?
            Cell(bestDefense.x, bestDefense.y) : Cell(bestAttack.x, bestAttack.y));
}

void NeighbourMatrix::clear() {
    neighbours_level1.clear();
    neighbours_level2.clear();
    neighbours_level3.clear();
}

//Добавление соседа (образующего двойку) на нужный уровень
void addNeighbour(NeighbourMatrix (&matrix)[15][15], int i, int j, int k, int x, int y){
    switch (k) {
        case 1:
            matrix[i][j].neighbours_level1.push_back(Cell(x, y));
            break;
        case 2:
            matrix[i][j].neighbours_level2.push_back(Cell(x, y));
            break;
        default:
            matrix[i][j].neighbours_level3.push_back(Cell(x, y));
            break;
    }
}

void AI::countNeighbourMatrix(Field &field, NeighbourMatrix (&matrix)[15][15]) {
    for (int i = 0; i < field.getFieldSize(); ++i){
        for (int j = 0; j < field.getFieldSize(); ++j){
            matrix[i][j].clear();
        }
    }

    for (int i = 0; i < field.getFieldSize(); ++i){
        for (int j = 0; j < field.getFieldSize(); ++j){
            if (field.at(i, j) == EMPTY) continue;

            for (int k = 1; k <= 3; ++k){
                int x = i - k, y = j;
                if (GameLogic::isCorrectCoords(field, x, y) && field.at(x, y) == field.at(i, j)) {
                    addNeighbour(matrix, i, j, k, x, y);
                }
                x = i, y = j - k;
                if (GameLogic::isCorrectCoords(field, x, y) && field.at(x, y) == field.at(i, j)) {
                    addNeighbour(matrix, i, j, k, x, y);
                }
                x = i + k, y = j;
                if (GameLogic::isCorrectCoords(field, x, y) && field.at(x, y) == field.at(i, j)) {
                    addNeighbour(matrix, i, j, k, x, y);
                }
                x = i, y = j + k;
                if (GameLogic::isCorrectCoords(field, x, y) && field.at(x, y) == field.at(i, j)) {
                    addNeighbour(matrix, i, j, k, x, y);
                }
                x = i - k, y = j - k;
                if (GameLogic::isCorrectCoords(field, x, y) && field.at(x, y) == field.at(i, j)) {
                    addNeighbour(matrix, i, j, k, x, y);
                }
                x = i - k, y = j + k;
                if (GameLogic::isCorrectCoords(field, x, y) && field.at(x, y) == field.at(i, j)) {
                    addNeighbour(matrix, i, j, k, x, y);
                }
                x = i + k, y = j - k;
                if (GameLogic::isCorrectCoords(field, x, y) && field.at(x, y) == field.at(i, j)) {
                    addNeighbour(matrix, i, j, k, x, y);
                }
                x = i + k, y = j + k;
                if (GameLogic::isCorrectCoords(field, x, y) && field.at(x, y) == field.at(i, j)) {
                    addNeighbour(matrix, i, j, k, x, y);
                }
            }
        }
    }

}

MoveType AI::getMoveType(Field &field, NeighbourMatrix (&before)[15][15], NeighbourMatrix (&after)[15][15], PlayerColor playerColor) {
    int countPairsBefore = 0, countPairsAfter = 0;
    for (int i = 0; i < 15; ++i){
        for (int j = 0; j < 15; ++j){
            if (field.at(i, j) != (playerColor == BLACK ? SECOND_PLAYER : FIRST_PLAYER)) continue;
            countPairsBefore += before[i][j].neighbours_level1.size() + before[i][j].neighbours_level2.size() + before[i][j].neighbours_level3.size();
            countPairsAfter += after[i][j].neighbours_level1.size() + after[i][j].neighbours_level2.size() + after[i][j].neighbours_level3.size();
        }
    }
    return (abs(countPairsBefore - countPairsAfter) > 1 ? POSITIONAL : ATTACKING);
}

///Поиск максимальной мощности, перебираем все возможные позиции двоек и проверяем на пустые ячейки
int getMaxPower(Field &field, int i, int j, PlayerColor playerColor);

Attack AI::attackingAlgorithm(Field &field, PlayerColor playerColor, MoveType moveType) {
    NeighbourMatrix before[15][15], after[15][15], postAfter[15][15];
    countNeighbourMatrix(field, before);

    vector<Attack> attackList;

    for (int i = 0; i < field.getFieldSize(); ++i){
        for (int j = 0; j < field.getFieldSize(); ++j){
            if (field.at(i, j) != EMPTY) continue;
            if (GameLogic::isCorrectCoords(field, i, j, playerColor)){
                Attack tmp;
                field.setCellValue(i, j, playerColor == BLACK ? SECOND_PLAYER : FIRST_PLAYER);
                countNeighbourMatrix(field, after);
                field.eraseCellValue(i, j);
                if (getMoveType(field, before, after, playerColor) == moveType){
                    tmp.x = i, tmp.y = j;
                    for (int x = i - 3; x <= i + 3; ++x){
                        for (int y = j - 3; y <= j + 3; ++y){
                            if (!(x == i && y == j) && GameLogic::isCorrectCoords(field, x, y, playerColor)){
                                field.setCellValue(x, y, playerColor == BLACK ? SECOND_PLAYER : FIRST_PLAYER);
                                countNeighbourMatrix(field, postAfter);
                                field.eraseCellValue(x, y);
                                if (getMoveType(field, after, postAfter, playerColor) == POSITIONAL) tmp.potential++;
                            }
                        }
                    }
                    tmp.power = getMaxPower(field, i, j, playerColor);
                    attackList.push_back(tmp);
                }
            }
        }
    }

    if (moveType == ATTACKING){
        sort(attackList.rbegin(), attackList.rend(), [](const Attack &left, const Attack &right){
            if (left.power == right.power)
                return left.potential < right.potential;
            return left.power < right.power;
        });
    } else {
        sort(attackList.rbegin(), attackList.rend(), [](const Attack &left, const Attack &right){
            if (left.potential == right.potential)
                return left.power < right.power;
            return left.potential < right.potential;
        });
    }
    if (attackList.empty())
        return Attack(-1, -1, 0, 0);
    return attackList[0];
}

Cell AI::bestSmartMove(Field &field, PlayerColor playerColor) {

    Cell opponentMove = field.getLastMove();

    NeighbourMatrix beforeMove[15][15], afterMove[15][15];

    field.eraseCellValue(opponentMove.x, opponentMove.y);
    countNeighbourMatrix(field, beforeMove);
    field.setCellValue(opponentMove.x, opponentMove.y, playerColor == BLACK ? FIRST_PLAYER : SECOND_PLAYER);
    countNeighbourMatrix(field, beforeMove);

    MoveType opponentMoveType = getMoveType(field, beforeMove, afterMove, playerColor == BLACK ? WHITE : BLACK);

    Attack attackingMove = attackingAlgorithm(field, playerColor, ATTACKING),
            positionalMove = attackingAlgorithm(field, playerColor, POSITIONAL);

    if (opponentMoveType == ATTACKING && positionalMove.x != -1){
        return Cell(positionalMove.x, positionalMove.y);
    } else if (opponentMoveType == POSITIONAL && attackingMove.x != -1){
        return Cell(attackingMove.x, attackingMove.y);
    } else {
        if (attackingMove.x == -1 && attackingMove.y == -1)
            return bestMove(field, playerColor);
        else if (attackingMove.x == -1)
            return Cell(positionalMove.x, positionalMove.y);
        else
            return Cell(attackingMove.x, attackingMove.y);
    }
}

int getMaxPower(Field &field, int i, int j, PlayerColor playerColor){
    int answer = 0;
    int tmp = 0;

    //level 1
    if (GameLogic::isCorrectCoords(field, i - 1, j - 1) && field.at(i, j) == field.at(i - 1, j - 1)){
        if (GameLogic::isCorrectCoords(field, i - 2, j - 2) && field.at(i - 2, j - 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 3, j - 3) && field.at(i - 3, j - 3) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 1, j + 1) && field.at(i + 1, j + 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 2, j + 2) && field.at(i + 2, j + 2) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i + 1, j + 1) && field.at(i, j) == field.at(i + 1, j + 1)){
        if (GameLogic::isCorrectCoords(field, i - 1, j - 1) && field.at(i - 1, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 2, j - 2) && field.at(i - 2, j - 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 2, j + 2) && field.at(i + 2, j + 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 3, j + 3) && field.at(i + 3, j + 3) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i - 1, j + 1) && field.at(i, j) == field.at(i - 1, j + 1)){
        if (GameLogic::isCorrectCoords(field, i + 1, j - 1) && field.at(i + 1, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 2, j - 2) && field.at(i + 2, j - 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 2, j + 2) && field.at(i - 2, j + 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 3, j + 3) && field.at(i - 3, j + 3) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i + 1, j - 1) && field.at(i, j) == field.at(i + 1, j - 1)){
        if (GameLogic::isCorrectCoords(field, i - 1, j + 1) && field.at(i - 1, j + 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 2, j + 2) && field.at(i - 2, j + 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 2, j - 2) && field.at(i + 2, j - 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 3, j - 3) && field.at(i + 3, j - 3) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i, j - 1) && field.at(i, j) == field.at(i, j - 1)){
        if (GameLogic::isCorrectCoords(field, i, j - 2) && field.at(i, j - 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j - 3) && field.at(i, j - 3) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j + 1) && field.at(i, j + 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j + 2) && field.at(i, j + 2) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i, j + 1) && field.at(i, j) == field.at(i, j + 1)){
        if (GameLogic::isCorrectCoords(field, i, j + 2) && field.at(i, j + 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j + 3) && field.at(i, j + 3) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j - 1) && field.at(i, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j - 2) && field.at(i, j - 2) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i - 1, j) && field.at(i, j) == field.at(i - 1, j)){
        if (GameLogic::isCorrectCoords(field, i - 2, j) && field.at(i - 2, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 3, j) && field.at(i - 3, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 1, j) && field.at(i + 1, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 2, j) && field.at(i + 2, j) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i + 1, j) && field.at(i, j) == field.at(i + 1, j)){
        if (GameLogic::isCorrectCoords(field, i + 2, j) && field.at(i + 2, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 3, j) && field.at(i + 3, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 1, j) && field.at(i - 1, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 2, j) && field.at(i - 2, j) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;

    //level 2
    if (GameLogic::isCorrectCoords(field, i - 2, j - 2) && field.at(i, j) == field.at(i - 2, j - 2)){
        if (GameLogic::isCorrectCoords(field, i - 3, j - 3) && field.at(i - 3, j - 3) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 1, j - 1) && field.at(i - 1, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 1, j + 1) && field.at(i + 1, j + 1) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i + 2, j + 2) && field.at(i, j) == field.at(i + 2, j + 2)){
        if (GameLogic::isCorrectCoords(field, i - 1, j - 1) && field.at(i - 1, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 1, j + 1) && field.at(i + 1, j + 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 3, j + 3) && field.at(i + 3, j + 3) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i - 2, j + 2) && field.at(i, j) == field.at(i - 2, j + 2)){
        if (GameLogic::isCorrectCoords(field, i + 1, j - 1) && field.at(i + 1, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 1, j + 1) && field.at(i - 1, j + 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 3, j + 3) && field.at(i - 3, j + 3) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i + 2, j - 2) && field.at(i, j) == field.at(i + 2, j - 2)){
        if (GameLogic::isCorrectCoords(field, i + 1, j - 1) && field.at(i + 1, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 1, j + 1) && field.at(i - 1, j + 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 3, j - 3) && field.at(i + 3, j - 3) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i, j - 2) && field.at(i, j) == field.at(i, j - 2)){
        if (GameLogic::isCorrectCoords(field, i, j - 1) && field.at(i, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j + 1) && field.at(i, j + 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j - 3) && field.at(i, j - 3) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i, j + 2) && field.at(i, j) == field.at(i, j + 2)){
        if (GameLogic::isCorrectCoords(field, i, j - 1) && field.at(i, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j + 1) && field.at(i, j + 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j + 3) && field.at(i, j + 3) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i - 2, j) && field.at(i, j) == field.at(i - 2, j)){
        if (GameLogic::isCorrectCoords(field, i - 1, j) && field.at(i - 1, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 1, j) && field.at(i + 1, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 3, j) && field.at(i - 3, j) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i + 2, j) && field.at(i, j) == field.at(i + 2, j)){
        if (GameLogic::isCorrectCoords(field, i - 1, j) && field.at(i - 1, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 1, j) && field.at(i + 1, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 3, j) && field.at(i + 3, j) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;

    //level 3
    if (GameLogic::isCorrectCoords(field, i - 3, j - 3) && field.at(i, j) == field.at(i - 3, j - 3)){
        if (GameLogic::isCorrectCoords(field, i - 2, j - 2) && field.at(i - 2, j - 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 1, j - 1) && field.at(i - 1, j - 1) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i + 3, j + 3) && field.at(i, j) == field.at(i + 3, j + 3)){
        if (GameLogic::isCorrectCoords(field, i + 2, j + 2) && field.at(i + 2, j + 2) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 1, j + 1) && field.at(i + 1, j + 1) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i - 3, j + 3) && field.at(i, j) == field.at(i - 3, j + 3)){
        if (GameLogic::isCorrectCoords(field, i - 1, j + 1) && field.at(i - 1, j + 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 2, j + 2) && field.at(i - 2, j + 2) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i + 3, j - 3) && field.at(i, j) == field.at(i + 3, j - 3)){
        if (GameLogic::isCorrectCoords(field, i + 1, j - 1) && field.at(i + 1, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 2, j - 2) && field.at(i + 2, j - 2) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i, j - 3) && field.at(i, j) == field.at(i, j - 3)){
        if (GameLogic::isCorrectCoords(field, i, j - 1) && field.at(i, j - 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j - 2) && field.at(i, j - 2) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i, j + 3) && field.at(i, j) == field.at(i, j + 3)){
        if (GameLogic::isCorrectCoords(field, i, j + 1) && field.at(i, j + 1) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i, j + 2) && field.at(i, j + 2) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i - 3, j) && field.at(i, j) == field.at(i - 3, j)){
        if (GameLogic::isCorrectCoords(field, i - 1, j) && field.at(i - 1, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i - 2, j) && field.at(i - 2, j) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    if (GameLogic::isCorrectCoords(field, i + 3, j) && field.at(i, j) == field.at(i + 3, j)){
        if (GameLogic::isCorrectCoords(field, i + 1, j) && field.at(i + 1, j) == EMPTY) tmp++;
        if (GameLogic::isCorrectCoords(field, i + 2, j) && field.at(i + 2, j) == EMPTY) tmp++;
    }
    answer = max(answer, tmp), tmp = 0;
    return answer;
}
