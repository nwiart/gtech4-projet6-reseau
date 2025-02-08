#include "Scene.h"


Scene* Scene::m_currentScene = 0;
Scene* Scene::m_nextScene = 0;

sf::Font Scene::m_font;


bool Scene::setGlobalFont(const char* fileName)
{
	return m_font.loadFromFile(fileName);
}

void Scene::quit()
{
	m_font = sf::Font();
}

void Scene::setCurrentScene(Scene* s)
{
	m_nextScene = s;
}

void Scene::sceneSwitch()
{
	if (m_nextScene) {
		if (m_currentScene) {
			delete m_currentScene;
		}

		m_currentScene = m_nextScene;
		m_nextScene = 0;
	}
}
