#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

const int SIM_WIDTH = 800;
const int UI_WIDTH = 320;
const int HEIGHT = 600;
const int SCALE = 10;
const int GRID_WIDTH = SIM_WIDTH / SCALE;
const int GRID_HEIGHT = HEIGHT / SCALE;

int main() {
	sf::VideoMode mode({SIM_WIDTH + UI_WIDTH, HEIGHT});
	sf::RenderWindow window(mode, "Particle Simulator", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	if (!ImGui::SFML::Init(window)) {
		return 1;
	}

	sf::RectangleShape simulation({static_cast<float>(SIM_WIDTH), static_cast<float>(HEIGHT)});
	simulation.setFillColor(sf::Color::Red);
	simulation.setPosition({static_cast<float>(UI_WIDTH), 0.f});

	float color[3] = {1.0f, 0.0f, 0.0f};

	sf::Clock deltaClock;
	sf::Clock fpsClock;
	float timeAccumulator = 0.f;
	int frameCounter = 0;

	while (window.isOpen()) {
		while (auto event = window.pollEvent()) {
			ImGui::SFML::ProcessEvent(window, *event);
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		float dt = deltaClock.restart().asSeconds();
		timeAccumulator += dt;
		frameCounter++;

		if (fpsClock.getElapsedTime().asSeconds() >= 0.5f) {
			float avgFPS = frameCounter / timeAccumulator;
			window.setTitle("Particle Simulator - FPS: " + std::to_string(static_cast<int>(avgFPS)));

			fpsClock.restart();
			timeAccumulator = 0.f;
			frameCounter = 0;
		}

		ImGui::SFML::Update(window, sf::seconds(dt));

		ImGui::Begin("Color Picker");
		ImGui::ColorEdit3("Rect Color", color);
		ImGui::End();

		simulation.setFillColor(sf::Color(
			static_cast<std::uint8_t>(color[0] * 255),
			static_cast<std::uint8_t>(color[1] * 255),
			static_cast<std::uint8_t>(color[2] * 255)
		));

		window.clear(sf::Color::Black);
		window.draw(simulation);

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}
