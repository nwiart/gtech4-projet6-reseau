#include "TextField.h"

TextField::TextField(float x, float y, sf::Font& font, bool isIpField) {
    box.setSize(sf::Vector2f(300, 40));
    box.setFillColor(sf::Color(50, 50, 50));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(2);
    box.setPosition(x, y);

    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(x + 10, y + 5);

    isActive = false;
    this->isIpField = isIpField;
}

void TextField::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        isActive = box.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
    }

    if (isActive && event.type == sf::Event::TextEntered) {
        char enteredChar = static_cast<char>(event.text.unicode);

        if (event.text.unicode == 8 && !input.empty()) {
            input.pop_back();
        }
        else if (event.text.unicode == 13) {
            isActive = false;
        }
        else if (isIpField) {
            if ((enteredChar >= '0' && enteredChar <= '9') || enteredChar == '.') {
                if (input.size() < 15)
                    input += enteredChar;
            }
        }
        else { 
            if (input.size() < 15)
                input += enteredChar;
        }
    }
}

void TextField::draw(sf::RenderWindow& window) {
    text.setString(input + (isActive ? "|" : ""));
    window.draw(box);
    window.draw(text);
}
