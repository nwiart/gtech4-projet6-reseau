#ifndef SCORE_H
#define SCORE_H

class Score {
public:
    Score();

    void addPoint(bool player1Scored);
    int getScore1() const;
    int getScore2() const;

    void getNetworkData(int& score1, int& score2) const;

private:
    int scorePlayer1;
    int scorePlayer2;
};

#endif
