#pragma once

#include "config.hpp"
#include "element.hpp"
#include "grid.hpp"

#include <SFML/Graphics.hpp>

namespace Utility {
	sf::RenderWindow createWindow();
	void updateFPS(sf::RenderWindow& window, sf::Clock& fpsClock, float& timeAccumulator, int& frameCounter, float dt);
	void drawGui(sf::RenderWindow& window, Grid& grid, Element& selectedElement, float& simSpeed, bool& isPaused);
	void handleInput(Grid& grid, sf::Vector2i& prevMouse, Element& element, sf::RenderWindow& window);
	float randomiseColour();
	sf::Color scaleColor(const sf::Color& color, float factor);
}
