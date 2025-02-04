#include "Score.h"

Score::Score() {
    reset();
}

void Score::addPointPlayer1() {
    scorePlayer1++;
}

void Score::addPointPlayer2() {
    scorePlayer2++;
}

void Score::reset() {
    scorePlayer1 = 0;
    scorePlayer2 = 0;
}

bool Score::isGameOver() const {
    return (scorePlayer1 >= maxScore || scorePlayer2 >= maxScore);
}
