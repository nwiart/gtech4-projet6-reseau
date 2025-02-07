#ifndef PLAYER_H
#define PLAYER_H

#include "SFMLInclude.h"

class Player {
public:
    Player();
    Player(float xRatio, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey);

    void updateFromServer(float y);
    void draw(sf::RenderWindow& window);

    float getPositionY() const;
    void setPosition(float y);

    sf::Keyboard::Key getUpKey() const;
    sf::Keyboard::Key getDownKey() const;

private:
    sf::RectangleShape paddle;
    sf::Keyboard::Key upKey;
    sf::Keyboard::Key downKey;
};

#endif
