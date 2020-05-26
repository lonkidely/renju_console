#include <iostream>
#include "Renju.h"

using namespace std;

int main() {
    system("chcp 65001");
    srand(time(0));
    Renju game;
    game.play();
    system("pause");
    return 0;
}
