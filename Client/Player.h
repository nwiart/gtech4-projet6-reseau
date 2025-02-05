#ifndef PLAYER_H
#define PLAYER_H

#include "SFMLInclude.h"

class Player {
public:
    Player(float xRatio, float yRatio, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey, bool useMouse = false);

    void update(float windowHeight, sf::RenderWindow& window);
    void resize(sf::Vector2u windowSize);
    void draw(sf::RenderWindow& window);

    sf::RectangleShape& getPaddle() { return paddle; }

    void setPosition(float y);
    float getPositionY() const;

private:
    sf::RectangleShape paddle;
    sf::Keyboard::Key upKey;
    sf::Keyboard::Key downKey;
    bool useMouse;

    static constexpr float RECT_WIDTH_RATIO = 0.02f;
    static constexpr float RECT_HEIGHT_RATIO = 0.20f;
};

#endif
