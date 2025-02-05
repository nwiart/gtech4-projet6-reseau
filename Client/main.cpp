#include "SFMLInclude.h"
#include "Scene.h"
#include "ConnectScreen.h"
#include "MainMenu.h"
#include "Networking.h"

int main()
{
    if (network::initializeWinsock() != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
    }
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pong", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    Network::init();

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    ConnectScreen* menu = new ConnectScreen(font);

    Scene::setCurrentScene(menu);

    while (window.isOpen())
    {
        Network::pollEvents();

        Scene* scene = Scene::getCurrentScene();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else {
                scene->handleEvent(event, window);
            }
        }

        scene->update(window);

        window.clear();
        scene->draw(window);
        window.display();
    }

    return 0;
}
