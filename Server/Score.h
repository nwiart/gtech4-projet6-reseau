#ifndef SCORE_H
#define SCORE_H

class Score {
public:
    Score();

    void addPointPlayer1();
    void addPointPlayer2();
    void reset();

    int getScorePlayer1() const { return scorePlayer1; }
    int getScorePlayer2() const { return scorePlayer2; }

    bool isGameOver() const;

private:
    int scorePlayer1;
    int scorePlayer2;

    const int maxScore = 99;
};

#endif
