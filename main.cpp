#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "grid.hpp"

const int SIM_WIDTH = 800;
const int UI_WIDTH = 320;
const int HEIGHT = 600;
const int SCALE = 10;
const int GRID_WIDTH = SIM_WIDTH / SCALE;
const int GRID_HEIGHT = HEIGHT / SCALE;

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
		timeAccumulator = 0.f;
		frameCounter = 0;
	}
}

void handleInput(Grid& grid, sf::Vector2i& prevMouse, const float color[3], sf::RenderWindow& window) {
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		int gridX = (mousePos.x - UI_WIDTH) / SCALE;
		int gridY = mousePos.y / SCALE;

		sf::Color newColor(
			static_cast<std::uint8_t>(color[0] * 255),
			static_cast<std::uint8_t>(color[1] * 255),
			static_cast<std::uint8_t>(color[2] * 255)
		);

		if (prevMouse.x != -1)
			grid.drawLine(prevMouse.x, prevMouse.y, gridX, gridY, newColor);
		else
			grid.setCell(gridX, gridY, newColor);

		prevMouse = {gridX, gridY};
	} else {
		prevMouse = {-1, -1};
	}
}

int main() {
	auto window = createWindow();
	if (!ImGui::SFML::Init(window))
		return 1;

	float color[3] = {1.0f, 1.0f, 1.0f};
	Grid grid(GRID_WIDTH, GRID_HEIGHT, SCALE, UI_WIDTH);
	sf::Vector2i prevMouse(-1, -1);

	sf::Clock deltaClock, fpsClock;
	float timeAccumulator = 0.0f;
	int frameCounter = 0;

	while (window.isOpen()) {
	   while (auto event = window.pollEvent()) {
			ImGui::SFML::ProcessEvent(window, *event);
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}
		handleInput(grid, prevMouse, color, window);

		float dt = deltaClock.restart().asSeconds();
		updateFPS(window, fpsClock, timeAccumulator, frameCounter, dt);

		ImGui::SFML::Update(window, sf::seconds(dt));
		ImGui::Begin("Color Picker");
		ImGui::ColorEdit3("Pixel Color", color);
		ImGui::End();

		window.clear(sf::Color(32, 32, 32));
		grid.draw(window);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}
