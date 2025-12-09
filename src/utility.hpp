#pragma once

#include "config.hpp"
#include "element.hpp"
#include "grid.hpp"

#include <SFML/Graphics.hpp>

namespace Utility {
	sf::RenderWindow createWindow();
	int updateFPS(sf::RenderWindow& window, sf::Clock& fpsClock, float& timeAccumulator, int& frameCounter, float dt);
	void drawGui(sf::RenderWindow& window, Grid& grid, Element& selectedElement, float& simSpeed, float& tempBlend, bool& isPaused);
	void handleInput(Grid& grid, sf::Vector2i& prevMouse, Element& element, sf::RenderWindow& window);
	float randomiseColour();
	std::vector<int> randomIntArray(int n);
	sf::Color scaleColour(const sf::Color& colour, float factor);
}
