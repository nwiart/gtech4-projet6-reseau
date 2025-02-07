#include "Button.h"

Button::Button(float x, float y, std::string label, sf::Font& font, std::function<void()> callback) {
    box.setSize(sf::Vector2f(200, 50));
    box.setFillColor(sf::Color(100, 100, 100));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(2);
    box.setPosition(x, y);

    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(x + 50, y + 15);
    text.setString(label);

    onClick = callback;
}

void Button::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (box.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
            onClick();
        }
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}
