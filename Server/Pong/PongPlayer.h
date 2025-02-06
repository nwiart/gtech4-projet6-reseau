#pragma once
#include <string>


class PongPlayer
{
public:

    PongPlayer() : m_id(-1), m_name("Unknown") {}

    inline int getID() const { return m_id; }

    inline const std::string& getName() const { return m_name; }
    inline void setName(const std::string& name) { m_name = name; }

private:

    std::string m_name;
	int m_id;
};
