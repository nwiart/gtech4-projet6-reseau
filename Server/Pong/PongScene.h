#pragma once

#include "Ball.h"


class PongScene
{
public:

	PongScene(int sizeX, int sizeY);

	void update(float dt);

	inline const Ball& getBall() const { return m_ball; }

	inline int getSizeX() const { return m_sizeX; }
	inline int getSizeY() const { return m_sizeY; }


private:

	Ball m_ball;

	int m_sizeX;
	int m_sizeY;

	bool m_started;
};
