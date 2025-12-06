#pragma once

#include "config.hpp"

#include <imgui.h>

sf::RenderWindow createWindow() {
	sf::VideoMode mode({static_cast<unsigned int>(SIM_WIDTH + UI_WIDTH), static_cast<unsigned int>(HEIGHT)});
	sf::RenderWindow window(mode, "Particle Simulator", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	return window;
}

void updateFPS(sf::RenderWindow& window, sf::Clock& fpsClock, float& timeAccumulator, int& frameCounter, float dt) {
	timeAccumulator += dt;
	frameCounter++;

	if (fpsClock.getElapsedTime().asSeconds() >= 0.5f) {
		float avgFPS = frameCounter / timeAccumulator;
		window.setTitle("Particle Simulator - FPS: " + std::to_string(static_cast<int>(avgFPS)));
		fpsClock.restart();
		timeAccumulator = 0.0f;
		frameCounter = 0;
	}
}

void handleInput(Grid& grid, sf::Vector2i& prevMouse, sf::Color colour, sf::RenderWindow& window) {
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;

	bool leftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	bool rightPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

	if (!leftPressed && !rightPressed) {
		prevMouse = {-1, -1};
		return;
	}

	sf::Color newColour = leftPressed ? colour : Elements::VOID.colour;

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	int gridX = (mousePos.x - UI_WIDTH) / SIM_SCALE;
	int gridY = mousePos.y / SIM_SCALE;

	if (prevMouse.x != -1)
		grid.drawLine(prevMouse.x, prevMouse.y, gridX, gridY, newColour);
	else
		grid.setCell(gridX, gridY, newColour);

	prevMouse = {gridX, gridY};
}