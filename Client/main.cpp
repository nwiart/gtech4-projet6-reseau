#include "SFMLInclude.h"

#include "Networking.h"
#include "Client.h"

#include "Scene/ConnectScreen.h"


int main()
{
    if (network::initializeWinsock() != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
    }

    Client::getInstance().init();

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pong", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    if (!Scene::setGlobalFont("arial.ttf")) {
        return -1;
    }

    ConnectScreen* menu = new ConnectScreen();

    Scene::setCurrentScene(menu);

    while (window.isOpen())
    {
        Scene::sceneSwitch();
        Scene* s = Scene::getCurrentScene();

        Client::getInstance().pollEvents();

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
