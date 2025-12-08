#pragma once

#include "cell.hpp"

#include <array>
#include <SFML/Graphics.hpp>
#include <vector>

struct Cell;
struct Element;
struct Elements;

struct Grid {
	int width, height, scale, uiOffset;
	std::vector<Cell> cells;
	sf::VertexArray vertices;
	bool dirty;

	Grid(int width, int height, int scale, int uiOffset);

	void draw(sf::RenderWindow& window);
	void clear();
	Cell getCell(int x, int y);
	Cell& getCellRef(int x, int y);
	std::array<std::array<Cell*, 3>, 3> getNeighbourhood(int cx, int cy);
	void setCell(int x, int y, const Element& element);
	void setCell(int x, int y, Cell cell);
	void swapCells(int x0, int y0, int x1, int y1);
	void drawLine(int x0, int y0, int x1, int y1, const Element& element);
	void rebuildVertices();
};
