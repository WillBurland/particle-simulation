#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "elements.hpp"
#include "grid.hpp"

const int SIM_WIDTH = 800;
const int UI_WIDTH = 320;
const int HEIGHT = 600;
const int SIM_SCALE = 10;
const int GRID_WIDTH = SIM_WIDTH / SIM_SCALE;
const int GRID_HEIGHT = HEIGHT / SIM_SCALE;

static bool firstFrame = true;
const float uiPadding = 10.0f;

static Element selectedElement = Elements::SAND;

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

void handleInput(Grid& grid, sf::Vector2i& prevMouse, const float colour[3], sf::RenderWindow& window) {
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;

	bool left = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	bool right = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

	if (!left && !right) {
		prevMouse = {-1, -1};
		return;
	}

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	int gridX = (mousePos.x - UI_WIDTH) / SIM_SCALE;
	int gridY = mousePos.y / SIM_SCALE;

	sf::Color newColour;
	if (right) {
		newColour = Elements::VOID.colour;
	} else {
		newColour = sf::Color(
			static_cast<std::uint8_t>(colour[0] * 255),
			static_cast<std::uint8_t>(colour[1] * 255),
			static_cast<std::uint8_t>(colour[2] * 255)
		);
	}

	if (prevMouse.x != -1)
		grid.drawLine(prevMouse.x, prevMouse.y, gridX, gridY, newColour);
	else
		grid.setCell(gridX, gridY, newColour);

	prevMouse = {gridX, gridY};
}

int main() {
	auto window = createWindow();
	if (!ImGui::SFML::Init(window))
		return 1;

	float colour[3] = {
		selectedElement.colour.r / 255.0f,
		selectedElement.colour.g / 255.0f,
		selectedElement.colour.b / 255.0f
	};
	Grid grid(GRID_WIDTH, GRID_HEIGHT, SIM_SCALE, UI_WIDTH);
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
		handleInput(grid, prevMouse, colour, window);

		float dt = deltaClock.restart().asSeconds();
		updateFPS(window, fpsClock, timeAccumulator, frameCounter, dt);

		ImGui::SFML::Update(window, sf::seconds(dt));

		if (firstFrame) {
			ImGui::SetNextWindowPos(ImVec2(uiPadding, uiPadding));
			ImGui::SetNextWindowSize(ImVec2(UI_WIDTH - 2 * uiPadding, HEIGHT - 2 * uiPadding));
			firstFrame = false;
		}

		ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		if (ImGui::BeginCombo("Element", selectedElement.name.c_str())) {
			int index = 0;
			for (auto& element : Elements::all()) {
				bool isSelected = (selectedElement.id == index);
				if (ImGui::Selectable(element.name.c_str(), isSelected)) {
					selectedElement = Elements::fromId(index);
					colour[0] = selectedElement.colour.r / 255.0f;
					colour[1] = selectedElement.colour.g / 255.0f;
					colour[2] = selectedElement.colour.b / 255.0f;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
				index++;
			}
			ImGui::EndCombo();
		}
		ImGui::End();

		window.clear(sf::Color(32, 32, 32));
		grid.draw(window);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}
