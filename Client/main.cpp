#include <SFML/Graphics.hpp>


int main(int argc, const char** argv)
{
	// Window without resize.
	sf::Window window;
	window.create(sf::VideoMode(1280, 720), "Pong Multiplayer", sf::Style::Default & ~sf::Style::Resize);
	window.setVerticalSyncEnabled(true);

	bool quit = false;
	while (!quit) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				quit = true;
			}
		}
	}

	return 0;
}
