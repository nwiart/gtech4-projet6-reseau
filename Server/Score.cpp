#include "Score.h"

Score::Score() {
    scorePlayer1 = 0;
    scorePlayer2 = 0;
}

void Score::addPoint(bool player1Scored) {
    if (player1Scored) {
        scorePlayer1++;
    }
    else {
        scorePlayer2++;
    }
}

int Score::getScore1() const {
    return scorePlayer1;
}

int Score::getScore2() const {
    return scorePlayer2;
}

// Fonction pour envoyer les données du score aux clients
void Score::getNetworkData(int& score1, int& score2) const {
    score1 = scorePlayer1;
    score2 = scorePlayer2;
}
