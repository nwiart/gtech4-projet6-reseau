#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "MainMenu.h"
#include "GameScene.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pong Multiplayer", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    Scene* currentScene = nullptr;
    MainMenu menu(font, [&](std::string p1, std::string p2) {
        currentScene = new GameScene(font, p1, p2);
        });

    currentScene = &menu;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            currentScene->handleEvent(event, window);
        }

        currentScene->update(window);

        window.clear();
        currentScene->draw(window);
        window.display();
    }

    return 0;
}
