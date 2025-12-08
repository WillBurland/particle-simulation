#include "config.hpp"
#include "element.hpp"
#include "grid.hpp"
#include "simulation.hpp"
#include "utility.hpp"

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window = Utility::createWindow();
	if (!ImGui::SFML::Init(window))
		return 1;
	sf::Vector2i prevMouse(-1, -1);

	Grid grid(GRID_WIDTH, GRID_HEIGHT, SIM_SCALE, UI_WIDTH);
	Element selectedElement = Elements::SAND;

	float speedAccumulator = 0.0f;
	float simSpeed = 1.0f;
	bool isPaused = false;

	while (window.isOpen()) {
	   while (auto event = window.pollEvent()) {
			ImGui::SFML::ProcessEvent(window, *event);
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		Utility::handleInput(grid, prevMouse, selectedElement, window);

		Utility::drawGui(window, grid, selectedElement, simSpeed, isPaused);

		if (!isPaused) {
			speedAccumulator += simSpeed;
			while (speedAccumulator >= 1.0f) {
				Simulation::step(grid);
				speedAccumulator -= 1.0f;
			}
		}

		window.clear(sf::Color(32, 32, 32));
		grid.draw(window);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}
