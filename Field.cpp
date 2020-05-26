#include "Field.h"

Field::Field() {

    countFreeCells = FIELD_SIZE * FIELD_SIZE;

    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            field[i][j] = EMPTY;
        }
    }
}

void Field::displayField() {
    system("cls");
    cout << setw(3) << ' ';
    for (int i = 0; i < FIELD_SIZE; ++i) {
        cout << setw(3) << i + 1;
    }
    cout << setw(3) << " \n";
    for (int i = 0; i < FIELD_SIZE; ++i) {
        cout << setw(3) << i + 1;
        for (int j = 0; j < FIELD_SIZE; ++j) {
            switch (field[i][j]) {
                case FIRST_PLAYER:
                    cout << setw(3) << "W";
                    break;
                case SECOND_PLAYER:
                    cout << setw(3) << "B";
                    break;
                default:
                    cout << setw(3) << " ";
                    break;
            }
        }
        cout << setw(3) << i + 1 << "\n";
    }
    cout << setw(3) << ' ';
    for (int i = 0; i < FIELD_SIZE; ++i) {
        cout << setw(3) << i + 1;
    }
    cout << setw(3) << " \n";
}

int Field::getFieldSize() {
    return FIELD_SIZE;
}

int Field::getCountFreeCells() {
    return countFreeCells;
}

void Field::setCellValue(int x, int y, CellOwner value) {
    field[x][y] = value;
    countFreeCells--;
}

void Field::eraseCellValue(int x, int y) {
    field[x][y] = EMPTY;
    countFreeCells++;
}

CellOwner &Field::at(int i, int j) {
    return field[i][j];
}