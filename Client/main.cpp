#include "SFMLInclude.h"
#include "ConnectScreen.h"
#include "Networking.h"
#include "Network.h"

sf::Font font;

int playerID;

int main()
{
    if (network::initializeWinsock() != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
    }

    Network::init();

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pong", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    ConnectScreen* menu = new ConnectScreen(font);

    Scene::setCurrentScene(menu);

    while (window.isOpen())
    {
        Scene::sceneSwitch();
        Scene* s = Scene::getCurrentScene();

        Network::pollEvents();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else {
                s->handleEvent(event, window);
            }
        }

        s->update(window);

        window.clear();
        s->draw(window);
        window.display();
    }

    return 0;
}
