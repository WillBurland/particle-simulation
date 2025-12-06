#pragma once

#include <SFML/Graphics.hpp>

struct Element {
	int id;
	std::string name;
	sf::Color colour;
	float density;
};

struct Elements {
	static const Element VOID;
	static const Element SAND;
	static const Element WATER;

	static const std::vector<Element>& all() {
		static const std::vector<Element> elems = {
			VOID,
			SAND,
			WATER
		};
		return elems;
	}

	static const Element& fromId(int id) {
		const std::vector<Element>& elems = all();
		for (const Element& e : elems)
			if (e.id == id)
				return e;
		return VOID;
	}
};

const Element Elements::VOID  = { 0, "Void",  sf::Color(  0,   0,   0), 0.0f };
const Element Elements::SAND  = { 1, "Sand",  sf::Color(255, 255,  64), 1.0f };
const Element Elements::WATER = { 2, "Water", sf::Color( 64,  64, 255), 0.5f };
