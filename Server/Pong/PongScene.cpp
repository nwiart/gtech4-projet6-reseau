#include "PongScene.h"


PongScene::PongScene(int sizeX, int sizeY)
	: m_sizeX(sizeX), m_sizeY(sizeY)
	, m_started(false)
{
	m_ball.reset(sizeX / 2, sizeY / 2);
}

void PongScene::update(float dt)
{
	m_ball.update(dt, this);
}
