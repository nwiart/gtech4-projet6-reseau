#ifndef PLAYER_H
#define PLAYER_H

#include "SFMLInclude.h"

class Player {
public:
    Player(float xRatio);

    void updateFromServer(float y);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape paddle;
};

#endif
