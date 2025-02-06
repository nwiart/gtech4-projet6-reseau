#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "SFMLInclude.h"
#include <string>

class TextField {
public:
    TextField(float x, float y, sf::Font& font, bool isIpField = false);
    void handleEvent(sf::Event event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    const std::string& getInput() const { return input; }
    void clear() { input.clear(); }

private:
    sf::RectangleShape box;
    sf::Text text;
    std::string input;
    bool isActive;
    bool isIpField;
};

#endif
