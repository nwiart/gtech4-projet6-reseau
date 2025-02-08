#include "SFMLInclude.h"

#include "Networking.h"
#include "Client.h"

#include "Scene/ConnectScreen.h"

#include <iostream>
#include <chrono>
using namespace std;


int main()
{
    if (network::initializeWinsock() != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
    }

    Client::getInstance().init();

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pong", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(false);

    if (!Scene::setGlobalFont("arial.ttf")) {
        return -1;
    }

    ConnectScreen* menu = new ConnectScreen();

    Scene::setCurrentScene(menu);

    auto start = chrono::high_resolution_clock::now();
    auto last = start;
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

        auto current = chrono::high_resolution_clock::now();
        double dt = chrono::duration_cast<chrono::nanoseconds>(current - last).count() / 1000000000.0;
        last = current;

        s->update(window, dt);

        window.clear();
        s->draw(window);
        window.display();
    }

    Scene::quit();

    return 0;
}
