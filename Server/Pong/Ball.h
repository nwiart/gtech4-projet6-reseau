#ifndef BALL_H
#define BALL_H

#include <SFML/System.hpp>

class PongScene;

class Ball {
public:
    Ball();

    void update(float dt, PongScene* scene);
    void resetPosition();

    void reverseX();
    void reverseY();

    void setPosition(float x, float y);
    void setVelocity(float vx, float vy);
    void setRadius(float r);

    const sf::Vector2f& getPosition() const { return position; }
    const sf::Vector2f& getVelocity() const { return velocity; }
    float getRadius() const;

    // Fonction pour envoyer les données aux clients
    void getNetworkData(float& x, float& y, float& r) const;

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float radius;
};

#endif
