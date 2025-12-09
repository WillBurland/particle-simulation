#include "grid.hpp"

#include "cell.hpp"
#include "element.hpp"
#include "utility.hpp"

#include <algorithm>
#include <cmath>

Grid::Grid(int width, int height, int scale, int uiOffset) :
width(width), height(height), scale(scale), uiOffset(uiOffset), dirty(true) {
	cells.resize(width * height, Cell());
	vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
	vertices.resize(width * height * 6);
	rebuildVertices(0.0f);
}

void Grid::draw(sf::RenderWindow& window, float tempBlend) {
	if (dirty || tempBlend != oldTempOpacity)
		rebuildVertices(tempBlend);
	window.draw(vertices);
	oldTempOpacity = tempBlend;
}

void Grid::clear() {
	for (Cell& cell : cells)
		cell = Cell();
	dirty = true;
}

Cell Grid::getCell(int x, int y) {
	if (x < 0 || x >= width || y < 0 || y >= height)
		return Cell();
	return cells[y * width + x];
}

Cell& Grid::getCellRef(int x, int y) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		static Cell dummy;
		return dummy;
	}
	return cells[y * width + x];
}

std::array<std::array<Cell*, 3>, 3> Grid::getNeighbourhood(int cx, int cy) {
	std::array<std::array<Cell*, 3>, 3> n{};
	for (int dy = -1; dy <= 1; ++dy) {
		for (int dx = -1; dx <= 1; ++dx) {
			int x = cx + dx;
			int y = cy + dy;
			n[dy + 1][dx + 1] = (x >= 0 && x < width && y >= 0 && y < height)
				? &cells[y * width + x]
				: nullptr;
		}
	}
	return n;
}

void Grid::setCell(int x, int y, const Element& element) {
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;

	int idx = y * width + x;
	if (cells[idx].element.colour != Elements::VOID.colour && element.colour != Elements::VOID.colour)
		return;

	cells[idx] = Cell(element, x, y);
	dirty = true;
}

void Grid::setCell(int x, int y, Cell cell) {
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;
	cell.setPos(x, y);
	int idx = y * width + x;
	cells[idx] = cell;
	dirty = true;
}

void Grid::swapCells(int x0, int y0, int x1, int y1) {
	Cell temp = getCell(x0, y0);
	setCell(x0, y0, getCell(x1, y1));
	setCell(x1, y1, temp);
}

void Grid::drawLine(int x0, int y0, int x1, int y1, const Element& element) {
	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;

	bool steep = dy > dx;
	if (steep) std::swap(dx, dy);

	int err = dx / 2;

	while (true) {
		setCell(x0, y0, element);

		if (x0 == x1 && y0 == y1)
			break;

		if (steep) {
			err -= dy;
			if (err < 0) {
				x0 += sx;
				err += dx;
			}
			y0 += sy;
		} else {
			err -= dy;
			if (err < 0) {
				y0 += sy;
				err += dx;
			}
			x0 += sx;
		}
	}
}

sf::Color temperatureColour(float value) {
    value = std::clamp(value, 0.0f, 1000.0f);
    float t = value / 1000.0f;

	struct ColourStop {
		float position;
		sf::Color colour;
	};
    static const std::vector<ColourStop> stops = {
        {0.0f, sf::Color(  0,   0, 128)},
        {0.1f, sf::Color(  0,   0, 255)},
        {0.2f, sf::Color(  0, 128, 255)},
        {0.3f, sf::Color(  0, 255, 255)},
        {0.4f, sf::Color(  0, 255, 128)},
        {0.5f, sf::Color(  0, 255,   0)},
        {0.6f, sf::Color(128, 255,   0)},
        {0.7f, sf::Color(255, 255,   0)},
        {0.8f, sf::Color(255, 128,   0)},
        {0.9f, sf::Color(255,   0,   0)},
        {1.0f, sf::Color(128,   0,   0)}
    };

    for (size_t i = 0; i < stops.size() - 1; ++i) {
        if (t >= stops[i].position && t <= stops[i + 1].position) {
            float localT = (t - stops[i].position) / (stops[i + 1].position - stops[i].position);
            return sf::Color(
				static_cast<uint8_t>(stops[i].colour.r + localT * (stops[i + 1].colour.r - stops[i].colour.r)),
				static_cast<uint8_t>(stops[i].colour.g + localT * (stops[i + 1].colour.g - stops[i].colour.g)),
				static_cast<uint8_t>(stops[i].colour.b + localT * (stops[i + 1].colour.b - stops[i].colour.b))
			);
        }
    }

    return stops.back().colour;
}

sf::Color blendColours(const sf::Color& base, const sf::Color& overlay, float alpha) {
    alpha = std::clamp(alpha, 0.0f, 1.0f);
    return sf::Color(
		static_cast<uint8_t>(base.r + alpha * (overlay.r - base.r)),
		static_cast<uint8_t>(base.g + alpha * (overlay.g - base.g)),
		static_cast<uint8_t>(base.b + alpha * (overlay.b - base.b)),
		static_cast<uint8_t>(base.a + alpha * (overlay.a - base.a))
	);
}

void Grid::rebuildVertices(float tempBlend) {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int idx = y * width + x;
			int i = idx * 6;

			vertices[i + 0].position = sf::Vector2f(x * scale + uiOffset, y * scale);
			vertices[i + 1].position = sf::Vector2f((x + 1) * scale + uiOffset, y * scale);
			vertices[i + 2].position = sf::Vector2f((x + 1) * scale + uiOffset, (y + 1) * scale);

			vertices[i + 3].position = sf::Vector2f(x * scale + uiOffset, y * scale);
			vertices[i + 4].position = sf::Vector2f((x + 1) * scale + uiOffset, (y + 1) * scale);
			vertices[i + 5].position = sf::Vector2f(x * scale + uiOffset, (y + 1) * scale);

			sf::Color cReal = Utility::scaleColour(cells[idx].element.colour, cells[idx].colourVariation);
			sf::Color cTemp = temperatureColour(cells[idx].temperature);

			if (cells[idx].element == Elements::VOID)
				cTemp = sf::Color::Black;

			sf::Color blended = blendColours(cReal, cTemp, tempBlend);
			for (int j = 0; j < 6; ++j)
				vertices[i + j].color = blended;

		}
	}
	dirty = false;
}
