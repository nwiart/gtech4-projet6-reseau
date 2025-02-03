#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
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

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Erreur : Impossible de charger la police !" << std::endl;
        return -1;
    }

    std::string inputText = "";
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(55, 110);

    sf::RectangleShape inputBox(sf::Vector2f(400, 50));
    inputBox.setFillColor(sf::Color(50, 50, 50));
    inputBox.setOutlineColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setPosition(50, 100);

    bool isActive = false;
    bool showCursor = true;
    sf::Clock cursorClock;

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

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (inputBox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                {
                    isActive = true;
                    cursorClock.restart();
                    showCursor = true;
                }
                else
                {
                    isActive = false;
                }
            }

            if (isActive && event.type == sf::Event::TextEntered)
            {
                char enteredChar = static_cast<char>(event.text.unicode);

                if (event.text.unicode == 8) // Touche Backspace
                {
                    if (!inputText.empty())
                    {
                        inputText.pop_back();
                    }
                }
                else if (event.text.unicode == 13) // Touche Entrée
                {
                    //if (isValidIP(inputText)) // Vérification de l'IP
                    //{
                        std::cout << "Adresse IP valide entree : " << inputText << std::endl;
                        inputText = ""; // Effacer le champ
                    //}
                    //else
                    //{
                    //    std::cout << "Adresse IP invalide !" << std::endl;
                    //}
                }
                else if ((enteredChar >= '0' && enteredChar <= '9') || enteredChar == '.')
                {
                    if (inputText.size() < 15)
                    {
                        inputText += enteredChar;
                    }
                }
            }

            if (cursorClock.getElapsedTime().asSeconds() >= 0.5f)
            {
                showCursor = !showCursor;
                cursorClock.restart();
            }

            if (isActive && showCursor)
                text.setString(inputText + "|");
            else
                text.setString(inputText);
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
        window.draw(inputBox);
        window.draw(text);
        window.display();
    }

    return 0;
}
