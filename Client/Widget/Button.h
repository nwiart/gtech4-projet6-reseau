#ifndef BUTTON_H
#define BUTTON_H

#include "SFMLInclude.h"
#include <functional>
#include <string>

class Button {
public:
    Button(float x, float y, std::string label, sf::Font& font, std::function<void()> callback);
    void handleEvent(sf::Event event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape box;
    sf::Text text;
    std::function<void()> onClick;
};

#endif
