#pragma once

#include "SFMLInclude.h"

/// <summary>
/// Base class for all game scenes, managing scene transitions, event handling,
/// updating, and rendering.
/// </summary>
class Scene {
private:
	static Scene* m_currentScene; ///< Pointer to the currently active scene.
	static Scene* m_nextScene; ///< Pointer to the scene that will be switched to.

	static sf::Font m_font; ///< Global font used for all scenes.

public:
	/// <summary>
	/// Virtual destructor for the Scene class.
	/// </summary>
	virtual ~Scene() = default;

	/// <summary>
	/// Handles user input events such as keyboard and mouse interactions.
	/// </summary>
	/// <param name="event">SFML event to process.</param>
	/// <param name="window">Reference to the render window.</param>
	virtual void handleEvent(sf::Event event, sf::RenderWindow& window) = 0;

	/// <summary>
	/// Updates the scene state. Can be overridden by derived classes.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	/// <param name="dt">Time delta for smooth updates.</param>
	virtual void update(sf::RenderWindow& window, double dt) {}

	/// <summary>
	/// Renders the scene's elements onto the window.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	virtual void draw(sf::RenderWindow& window) = 0;

	/// <summary>
	/// Retrieves the global font used across all scenes.
	/// </summary>
	/// <returns>Reference to the global font.</returns>
	static inline sf::Font& getGlobalFont() { return m_font; }

	/// <summary>
	/// Loads and sets the global font from a specified file.
	/// </summary>
	/// <param name="fileName">Path to the font file.</param>
	/// <returns>True if the font was successfully loaded, false otherwise.</returns>
	static bool setGlobalFont(const char* fileName);

	/// <summary>
	/// Quits the application by clearing the current scene.
	/// </summary>
	static void quit();

	/// <summary>
	/// Retrieves the currently active scene.
	/// </summary>
	/// <returns>Pointer to the current scene.</returns>
	static inline Scene* getCurrentScene() { return m_currentScene; }

	/// <summary>
	/// Sets a new scene as the active scene.
	/// </summary>
	/// <param name="s">Pointer to the new scene to be set.</param>
	static void setCurrentScene(Scene* s);

	/// <summary>
	/// Switches to the next scene if one is queued for transition.
	/// </summary>
	static void sceneSwitch();
};
