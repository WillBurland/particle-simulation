#pragma once

#include "state.hpp"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct Element {
	int id;
	std::string name;
	sf::Color colour;
	State state;
	float density;

	bool operator==(const Element& other) const {
		return id == other.id;
	}
};

struct Elements {
	static inline const Element VOID  = { 0, "Void",  sf::Color(  0,   0,   0), States::GAS,    0.0f };
	static inline const Element SAND  = { 1, "Sand",  sf::Color(255, 255,  64), States::SOLID,  1.0f };
	static inline const Element STONE = { 2, "Stone", sf::Color(128, 128, 128), States::SOLID,  2.0f };
	static inline const Element WATER = { 3, "Water", sf::Color( 64,  64, 255), States::LIQUID, 0.5f };
	static inline const Element LAVA  = { 4, "Lava",  sf::Color(255, 128,  32), States::LIQUID, 0.6f };
	static inline const Element STEAM = { 5, "Steam", sf::Color(128, 128, 255), States::GAS,    0.1f };

	static const std::vector<Element>& all();
	static const Element& fromId(int id);
};
