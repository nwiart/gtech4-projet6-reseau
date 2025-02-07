#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/System.hpp>

class Player {
public:
    Player(float x, float y);

    void update(float dt, bool upPressed, bool downPressed, float windowHeight);
    void setPosition(float y);
    float getPositionY() const;

    void getNetworkData(float& y) const;

private:
    sf::Vector2f position;
    float speed = 300.0f;
    float paddleHeight = 100.0f;
};

#endif
