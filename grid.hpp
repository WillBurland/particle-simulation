#include <SFML/Graphics.hpp>
#include <vector>

class Grid {
	public:
		Grid(int width, int height, int scale, int uiOffset) :
		width(width), height(height), scale(scale), uiOffset(uiOffset), dirty(true) {
			cells.resize(width * height, sf::Color::Black);
			vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
			vertices.resize(width * height * 6);
			rebuildVertices();
		}

		void draw(sf::RenderWindow& window) {
			if (dirty)
				rebuildVertices();
			window.draw(vertices);
		}

		sf::Color getCell(int x, int y) {
			if (x < 0 || x >= width || y < 0 || y >= height)
				return elements[0].colour;
			return cells[y * width + x];
		}

		void setCell(int x, int y, const sf::Color& color) {
			if (x < 0 || x >= width || y < 0 || y >= height)
				return;

			int idx = y * width + x;
			if (cells[idx] == color)
				return;

			cells[idx] = color;
			dirty = true;
		}

		void drawLine(int x0, int y0, int x1, int y1, const sf::Color& color) {
			int dx = std::abs(x1 - x0);
			int dy = std::abs(y1 - y0);

			int sx = x0 < x1 ? 1 : -1;
			int sy = y0 < y1 ? 1 : -1;

			bool steep = dy > dx;
			if (steep)
				std::swap(dx, dy);

			int err = dx / 2;

			while (true) {
				setCell(x0, y0, color);

				if (x0 == x1 && y0 == y1)
					break;

				if (steep) {
					err -= dy;
					if (err < 0) { x0 += sx; err += dx; }
					y0 += sy;
				} else {
					err -= dy;
					if (err < 0) { y0 += sy; err += dx; }
					x0 += sx;
				}
			}
		}

	private:
		void rebuildVertices() {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					int idx = y * width + x;
					int i = idx * 6;
					sf::Color c = cells[idx];

					vertices[i + 0].position = sf::Vector2f(x * scale + uiOffset, y * scale);
					vertices[i + 1].position = sf::Vector2f((x + 1) * scale + uiOffset, y * scale);
					vertices[i + 2].position = sf::Vector2f((x + 1) * scale + uiOffset, (y + 1) * scale);

					vertices[i + 3].position = sf::Vector2f(x * scale + uiOffset, y * scale);
					vertices[i + 4].position = sf::Vector2f((x + 1) * scale + uiOffset, (y + 1) * scale);
					vertices[i + 5].position = sf::Vector2f(x * scale + uiOffset, (y + 1) * scale);

					for (int j = 0; j < 6; ++j)
						vertices[i + j].color = c;
				}
			}
			dirty = false;
		}

	private:
		int width, height, scale, uiOffset;
		std::vector<sf::Color> cells;
		sf::VertexArray vertices;
		bool dirty;
};
