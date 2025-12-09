#include "cell.hpp"

#include "utility.hpp"

Cell::Cell() :
	element(Elements::VOID),
	x(0), y(0),
	updatedFlag(false),
	colourVariation(Utility::randomiseColour()) {}

Cell::Cell(Element element, int x, int y) :
	element(element),
	x(x), y(y),
	updatedFlag(false),
	colourVariation(Utility::randomiseColour()) {}

void Cell::setPos(int newX, int newY) {
	x = newX;
	y = newY;
}
