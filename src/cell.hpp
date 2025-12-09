#pragma once

#include "element.hpp"

struct Cell {
	Element element;
	int x, y;
	float colourVariation;
	bool updatedFlag;

	Cell();
	Cell(Element element, int x, int y);

	void setPos(int newX, int newY);
};
