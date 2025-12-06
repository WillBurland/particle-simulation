#pragma once

struct Cell {
	Element element;
	int x, y;
	int velocityX, velocityY;
	float temperature;

	Cell() :
		element(Elements::VOID), x(0), y(0), velocityX(0), velocityY(0), temperature(0.0f) {}

	Cell(Element element, int x, int y) :
		element(element), x(x), y(y), velocityX(0), velocityY(0), temperature(0.0f) {}

	Cell(Element element, int x, int y, int velocityX, int velocityY) :
		element(element), x(x), y(y), velocityX(velocityX), velocityY(velocityY), temperature(0.0f) {}

	Cell(Element element, int x, int y, int velocityX, int velocityY, float temperature) :
		element(element), x(x), y(y), velocityX(velocityX), velocityY(velocityY), temperature(temperature) {}

	void setPos(int newX, int newY) {
		x = newX;
		y = newY;
	}
};