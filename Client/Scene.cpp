#include "Scene.h"


Scene* Scene::m_currentScene = 0;

void Scene::setCurrentScene(Scene* s)
{
	if (m_currentScene) {
		delete m_currentScene;
	}

	m_currentScene = s;
}
