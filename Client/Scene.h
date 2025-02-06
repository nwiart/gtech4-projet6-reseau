#pragma once

#include "SFMLInclude.h"

class Scene {
private:

    static Scene* m_currentScene;
    static Scene* m_nextScene;

public:
    virtual ~Scene() = default;
    virtual void handleEvent(sf::Event event, sf::RenderWindow& window) = 0;
    virtual void update(sf::RenderWindow& window) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    static inline Scene* getCurrentScene() { return m_currentScene; }
    static void setCurrentScene(Scene* s);

    static void sceneSwitch();
};
