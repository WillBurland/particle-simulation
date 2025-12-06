#include "elements.hpp"
#include "grid.hpp"
#include "utility.hpp"

#include <imgui-SFML.h>

int main() {
	auto window = createWindow();
	if (!ImGui::SFML::Init(window))
		return 1;
	sf::Vector2i prevMouse(-1, -1);

	Grid grid(GRID_WIDTH, GRID_HEIGHT, SIM_SCALE, UI_WIDTH);
	Element selectedElement = Elements::SAND;

	sf::Clock deltaClock, fpsClock;
	float timeAccumulator = 0.0f;
	int frameCounter = 0;
	bool firstFrame = true;

	while (window.isOpen()) {
	   while (auto event = window.pollEvent()) {
			ImGui::SFML::ProcessEvent(window, *event);
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}
		handleInput(grid, prevMouse, selectedElement.colour, window);

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
			for (auto& element : Elements::all()) {
				bool isSelected = (selectedElement.id == index);
				if (ImGui::Selectable(element.name.c_str(), isSelected))
					selectedElement = Elements::fromId(index);
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
