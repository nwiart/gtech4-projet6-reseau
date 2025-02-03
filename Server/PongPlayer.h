#pragma once


class PongPlayer
{
public:

	PongPlayer()
		: m_id(-1)
	{
	}

	inline int getID() const { return m_id; }

private:

	int m_id;
};
