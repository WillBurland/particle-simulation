#include "utility.hpp"

#include "simulation.hpp"

#include <algorithm>
#include <cstdint>
#include <imgui.h>
#include <imgui-SFML.h>
#include <random>
#include <string>

namespace {
	bool firstFrame = true;
	sf::Clock deltaClock;
	sf::Clock fpsClock;
	float timeAccumulator = 0.0f;
	int frameCounter = 0;
}

namespace Utility {
	sf::RenderWindow createWindow() {
		sf::VideoMode mode({static_cast<unsigned int>(SIM_WIDTH + UI_WIDTH), static_cast<unsigned int>(HEIGHT)});
		sf::RenderWindow window(mode, "Particle Simulator", sf::Style::Titlebar | sf::Style::Close);
		window.setVerticalSyncEnabled(true);
		return window;
	}

	void updateFPS(sf::RenderWindow& window, sf::Clock& fpsClock,  float& timeAccumulator, int& frameCounter, float dt) {
		timeAccumulator += dt;
		frameCounter++;

		if (fpsClock.getElapsedTime().asSeconds() >= 0.5f) {
			window.setTitle("Particle Simulator - FPS: " + std::to_string(static_cast<int>(frameCounter / timeAccumulator)));
			fpsClock.restart();
			timeAccumulator = 0.0f;
			frameCounter = 0;
		}
	}

	void drawGui(sf::RenderWindow& window, Grid& grid, Element& selectedElement, float& simSpeed, bool& isPaused) {
		float dt = deltaClock.restart().asSeconds();
		updateFPS(window, fpsClock, timeAccumulator, frameCounter, dt);
		ImGui::SFML::Update(window, sf::seconds(dt));

		if (firstFrame) {
			float uiPadding = 10.0f;
			ImGui::SetNextWindowPos(ImVec2(uiPadding, uiPadding));
			ImGui::SetNextWindowSize(ImVec2(UI_WIDTH - 2 * uiPadding, HEIGHT - 2 * uiPadding));
			firstFrame = false;
		}

		ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		if (ImGui::BeginCombo("Element", selectedElement.name.c_str())) {
			int index = 0;
			for (Element element : Elements::all()) {
				bool isSelected = (selectedElement.id == index);
				if (ImGui::Selectable(element.name.c_str(), isSelected))
					selectedElement = Elements::fromId(index);
				if (isSelected)
					ImGui::SetItemDefaultFocus();
				index++;
			}
			ImGui::EndCombo();
		}

		ImGui::PushStyleColor(ImGuiCol_Button, isPaused ? ImVec4(0.8f, 0.1f, 0.1f, 1.0f)
														 : ImVec4(0.1f, 0.8f, 0.1f, 1.0f));
		if (ImGui::Button(isPaused ? "Paused" : "Playing"))
			isPaused = !isPaused;
		ImGui::PopStyleColor();

		ImGui::SliderFloat("Speed", &simSpeed, 0.1f, 3.0f, "%.1fx");

		ImGui::BeginDisabled(!isPaused);
		if (ImGui::Button("Step"))
			Simulation::step(grid);
		ImGui::EndDisabled();

		if (ImGui::Button("Clear"))
			grid.clear();

		ImGui::End();
	}

	void handleInput(Grid& grid, sf::Vector2i& prevMouse, Element& element, sf::RenderWindow& window) {
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse) return;

		bool leftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
		bool rightPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

		if (!leftPressed && !rightPressed) {
			prevMouse = {-1, -1};
			return;
		}

		Element newElement = element;
		if (rightPressed) newElement = Elements::VOID;

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		int gridX = (mousePos.x - UI_WIDTH) / SIM_SCALE;
		int gridY = mousePos.y / SIM_SCALE;

		if (prevMouse.x != -1)
			grid.drawLine(prevMouse.x, prevMouse.y, gridX, gridY, newElement);
		else
			grid.setCell(gridX, gridY, newElement);

		prevMouse = {gridX, gridY};
	}

	float randomiseColour() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		float variation = 0.2f;
		std::uniform_real_distribution<float> dis(1.0f - variation, 1.0f + variation);
		return dis(gen);
	}

	std::vector<int> randomIntArray(int n) {
		std::vector<int> result(n);
		std::iota(result.begin(), result.end(), 0);
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::shuffle(result.begin(), result.end(), gen);
		return result;
	}

	sf::Color scaleColor(const sf::Color& color, float factor) {
		return sf::Color{
			static_cast<uint8_t>(std::clamp(int(color.r * factor), 0, 255)),
			static_cast<uint8_t>(std::clamp(int(color.g * factor), 0, 255)),
			static_cast<uint8_t>(std::clamp(int(color.b * factor), 0, 255))
		};
	}
}
