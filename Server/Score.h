#ifndef SCORE_H
#define SCORE_H

/// <summary>
/// Manages the score system for a Pong game, tracking points for both players.
/// </summary>
class Score {
public:
	/// <summary>
	/// Default constructor initializing both players' scores to zero.
	/// </summary>
	Score();

	/// <summary>
	/// Increments the score of the player who scored a point.
	/// </summary>
	/// <param name="player1Scored">True if player 1 scored, false if player 2 scored.</param>
	void addPoint(bool player1Scored);

	/// <summary>
	/// Retrieves the current score of player 1.
	/// </summary>
	/// <returns>Player 1's score.</returns>
	int getScore1() const;

	/// <summary>
	/// Retrieves the current score of player 2.
	/// </summary>
	/// <returns>Player 2's score.</returns>
	int getScore2() const;

	/// <summary>
	/// Retrieves the current scores of both players for network transmission.
	/// </summary>
	/// <param name="score1">Output parameter for player 1's score.</param>
	/// <param name="score2">Output parameter for player 2's score.</param>
	void getNetworkData(int& score1, int& score2) const;

private:
	int scorePlayer1; ///< Player 1's current score.
	int scorePlayer2; ///< Player 2's current score.
};

#endif // SCORE_H
