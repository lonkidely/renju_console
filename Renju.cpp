#include "Renju.h"

Renju::Renju() : field(), firstPlayer(HUMAN, WHITE), secondPlayer(BOT, BLACK) {}

void Renju::displayField() {
    field.displayField();
}

CellOwner Renju::getWinner() {
    CellOwner winner = EMPTY;

    for (int i = 0; i < field.getFieldSize(); ++i) {
        for (int j = 0; j < field.getFieldSize(); ++j) {
            if (field.at(i, j) == EMPTY) continue;

            //Считаем все возможные отрезки, которые проходят через эту точку
            Segment horizontal = GameLogic::findSegmentEnds(field, i, j, HORIZONTAL),
                    vertical = GameLogic::findSegmentEnds(field, i, j, VERTICAL),
                    main_diagonal = GameLogic::findSegmentEnds(field, i, j, MAIN_DIAGONAL),
                    secondary_diagonal = GameLogic::findSegmentEnds(field, i, j, SECONDARY_DIAGONAL);

            //Если есть хотя бы одна пятерка (или длинный ряд в случае белых) - победитель найден
            if (horizontal.segmentLength() >= 5 || vertical.segmentLength() >= 5
                || main_diagonal.segmentLength() >= 5 || secondary_diagonal.segmentLength() >= 5) {
                    winner = field.at(i, j);
                    break;
            }
        }
    }

    return winner;
}

void Renju::play() {
    secondPlayer.move(field);
    displayField();

    firstPlayer.move(field);
    displayField();

    secondPlayer.move(field);
    displayField();

    while (!GameLogic::gameIsOver(field)) {
        firstPlayer.move(field);
        displayField();

        if (GameLogic::gameIsOver(field)) break;

        secondPlayer.move(field);
        displayField();
    }

    CellOwner winner = getWinner();

    displayField();

    switch (winner) {
        case FIRST_PLAYER:
            cout << "Победил первый игрок\n";
            break;
        case SECOND_PLAYER:
            cout << "Победил второй игрок\n";
            break;
        default:
            cout << "Ничья\n";
            break;
    }
    cout << "Сделано ходов: " << firstPlayer.getMovesCount() + secondPlayer.getMovesCount() << "\n";
}