#include <SFML/Graphics.hpp>

#include "Networking.h"
#include "Socket.h"


class ServerConnection
{
public:

    ServerConnection() { }

    int connect(const char* ip);

private:

    static const uint16_t serverBasePort = 27015;

    Socket m_serverTCP;
    Socket m_serverUDP;
};

int ServerConnection::connect(const char* ip)
{
    m_serverTCP.createSocketTCP();
    m_serverUDP.createSocketUDP();

    m_serverTCP.connectTCP(ip, serverBasePort);
}


int main()
{
    network::initializeWinsock();

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pong Multiplayer", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    bool isFullscreen = false;

    const float RECT_WIDTH_RATIO = 0.02f;
    const float RECT_HEIGHT_RATIO = 0.20f;
    const float MARGIN_RATIO = 0.02f;

    sf::RectangleShape rectangle;
    rectangle.setFillColor(sf::Color::Blue);

    sf::RectangleShape rectangle2;
    rectangle2.setFillColor(sf::Color::Blue);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11 || event.type == sf::Event::Resized)
            {
                isFullscreen = !isFullscreen;

                if (isFullscreen)
                {
                    window.create(sf::VideoMode(1920, 1080), "Pong Multiplayer", sf::Style::Default);
                }
                else
                {
                    window.create(sf::VideoMode(1280, 720), "Pong Multiplayer", sf::Style::Default);
                }

                window.setVerticalSyncEnabled(true);
            }
        }

        sf::Vector2u winSize = window.getSize();

        float rectWidth = winSize.x * RECT_WIDTH_RATIO;
        float rectHeight = winSize.y * RECT_HEIGHT_RATIO;
        float margin = winSize.x * MARGIN_RATIO;

        rectangle.setSize(sf::Vector2f(rectWidth, rectHeight));
        rectangle2.setSize(sf::Vector2f(rectWidth, rectHeight));

        rectangle.setPosition(margin, winSize.y / 2.f - rectHeight / 2.f);
        rectangle2.setPosition(winSize.x - rectWidth - margin, winSize.y / 2.f - rectHeight / 2.f);

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        float newY = static_cast<float>(mousePosition.y);
        float minY = 0;
        float maxY = winSize.y - rectHeight;

        if (newY < minY) newY = minY;
        if (newY > maxY) newY = maxY;

        rectangle.setPosition(rectangle.getPosition().x, newY);

        window.clear();
        window.draw(rectangle);
        window.draw(rectangle2);
        window.display();
    }

    network::cleanupWinsock();

    return 0;
}
