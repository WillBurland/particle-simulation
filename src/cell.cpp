#include "cell.hpp"

#include "utility.hpp"

Cell::Cell() :
	element(Elements::VOID),
	x(0), y(0),
	colourVariation(Utility::randomiseColour()),
	temperature(Elements::AMBIENT_TEMP),
	updatedFlag(false) {}

Cell::Cell(Element element, int x, int y) :
	element(element),
	x(x), y(y),
	colourVariation(Utility::randomiseColour()),
	temperature(element.baseTemperature),
	updatedFlag(false) {}

void Cell::setPos(int newX, int newY) {
	x = newX;
	y = newY;
}
