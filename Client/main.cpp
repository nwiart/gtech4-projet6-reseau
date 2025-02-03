#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Player.h"
#include "Score.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pong Multiplayer", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    Player player1(0.02f, 0.5f, sf::Keyboard::W, sf::Keyboard::S, true);
    Player player2(0.96f, 0.5f, sf::Keyboard::Up, sf::Keyboard::Down, false);
    Ball ball(640, 360);
    Score score(font, window.getSize());

    int score1 = 0;
    int score2 = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                player1.resize(window.getSize());
                player2.resize(window.getSize());
                score.update(score1, score2, window.getSize());
            }
        }

        player1.update(window.getSize().y, window);
        player2.update(window.getSize().y, window);

        if (ball.getPosition().x < 0) {
            score2++;
            ball.reset(window.getSize().x / 2.f, window.getSize().y / 2.f);
            score.update(score1, score2, window.getSize());
        }
        else if (ball.getPosition().x > window.getSize().x) {
            score1++;
            ball.reset(window.getSize().x / 2.f, window.getSize().y / 2.f);
            score.update(score1, score2, window.getSize());
        }

        ball.update(window, player1.getPaddle(), player2.getPaddle());

        window.clear();
        score.draw(window);
        player1.draw(window);
        player2.draw(window);
        ball.draw(window);
        window.display();
    }

    return 0;
}
