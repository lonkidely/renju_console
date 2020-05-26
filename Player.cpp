#include "Player.h"

Player::Player() : playerColor(WHITE), playerType(HUMAN), movesCount(0) {}

Player::Player(PlayerType _playerType, PlayerColor _playerColor) : playerType(_playerType), playerColor(_playerColor),
                                                                 movesCount(0) {}

void Player::firstMove(Field &field) {
    field.setCellValue(7, 7, SECOND_PLAYER);
}

void Player::secondMove(Field &field) {
    int x = 1, y = -1;
    if (playerType == BOT) {
        do {
            x = 6 + rand() % 3;
            y = 6 + rand() % 3;
        } while (!GameLogic::isCorrectSecondMove(field, x, y));

    } else {
        cout << "Введите координаты хода\n";
        cin >> x >> y, x--, y--;
        while (!(GameLogic::isCorrectSecondMove(field, x, y))) {
            cout << "Некорректные координаты, введите другие\n";
            cin >> x >> y, x--, y--;
        }
    }
    field.setCellValue(x, y, FIRST_PLAYER);
}

void Player::thirdMove(Field &field) {
    int x, y;
    if (playerType == BOT) {
        Cell move = AI::bestMove(field, BLACK);
        x = move.x;
        y = move.y;
    } else {
        cout << "Введите координаты хода\n";
        cin >> x >> y, x--, y--;
        while (!(GameLogic::isCorrectThirdMove(field, x, y))) {
            cout << "Некорректные координаты, введите другие\n";
            cin >> x >> y, x--, y--;
        }
    }
    field.setCellValue(x, y, SECOND_PLAYER);
}

void Player::ordinaryMove(Field &field) {
    int x, y;
    if (playerType == BOT) {
        Cell move = AI::bestMove(field, playerColor);
        if (move.x == -1 || move.y == -1) return;
        x = move.x;
        y = move.y;
    } else {
        cout << "Введите координаты хода\n";
        cin >> x >> y, x--, y--;
        while (!(GameLogic::isCorrectCoords(field, x, y, playerColor))) {
            cout << "Некорректные координаты, введите другие\n";
            cin >> x >> y, x--, y--;
        }
    }
    field.setCellValue(x, y, (playerColor == WHITE ? FIRST_PLAYER : SECOND_PLAYER));
}

void Player::move(Field &field) {
    if (movesCount == 0) {
        if (playerColor == BLACK)
            firstMove(field);
        else
            secondMove(field);
    } else if (movesCount == 1 && playerColor == BLACK) {
        thirdMove(field);
    } else {
        ordinaryMove(field);
    }
    movesCount++;
}

int Player::getMovesCount() {
    return movesCount;
}