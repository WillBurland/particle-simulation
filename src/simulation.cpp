#include "simulation.hpp"

#include "cell.hpp"
#include "element.hpp"
#include "grid.hpp"
#include "utility.hpp"

#include <cstdlib>

namespace Simulation {
	bool currentFrameFlag = false;

	MoveOptions evaluateMoves(const Cell& current, const std::array<std::array<Cell*, 3>, 3>& n) {
		MoveOptions out{};
		const int (&rules)[3][3] = current.element.state.moves;
		float density = current.element.density;

		for (int dy = -1; dy <= 1; ++dy) {
			for (int dx = -1; dx <= 1; ++dx) {
				if (dx == 0 && dy == 0)
					continue;

				int p = rules[dy + 1][dx + 1];
				if (p <= 0)
					continue;

				Cell* target = n[dy + 1][dx + 1];
				if (!target)
					continue;

				if (target->element.density < density)
					out.priorites[dy + 1][dx + 1] = p;
			}
		}

		return out;
	}

	bool pickHighestPriorityMove(const MoveOptions& m, int& outDx, int& outDy) {
		int maxPriority = 0;

		for (int dy = 0; dy < 3; ++dy)
			for (int dx = 0; dx < 3; ++dx)
				if (m.priorites[dy][dx] > maxPriority)
					maxPriority = m.priorites[dy][dx];

		if (maxPriority == 0)
			return false;

		std::vector<std::pair<int,int>> candidates;
		for (int dy = 0; dy < 3; ++dy)
			for (int dx = 0; dx < 3; ++dx)
				if (m.priorites[dy][dx] == maxPriority)
					candidates.emplace_back(dx - 1, dy - 1);

		int idx = std::rand() % candidates.size();
		outDx = candidates[idx].first;
		outDy = candidates[idx].second;
		return true;
	}

	void step(Grid& grid) {
		std::vector<std::vector<float>> nextTemperatures(grid.width, std::vector<float>(grid.height));
		for (int y = 0; y < grid.height; y++) {
			for (int x = 0; x < grid.width; x++) {
				Cell& current = grid.getCellRef(x, y);

				if (current.element == Elements::VOID)
					continue;

				float ambientTransfer = 1e-5f;
				float elementTransfer = 1e-1f;

				float delta = 0.0f;
				for (int dy = -1; dy <= 1; dy++) {
					for (int dx = -1; dx <= 1; dx++) {
						if (dx == 0 && dy == 0) continue;
						int nx = x + dx;
						int ny = y + dy;
						if (nx < 0 || ny < 0 || nx >= grid.width || ny >= grid.height)
							continue;

						Cell& neighbor = grid.getCellRef(nx, ny);

						if (neighbor.element == Elements::VOID)
							continue;

						delta += (neighbor.temperature - current.temperature) * elementTransfer;
					}
				}

				delta += (Elements::AMBIENT_TEMP - current.temperature) * ambientTransfer;

				nextTemperatures[x][y] = current.temperature + delta;
			}
		}

		for (int y = 0; y < grid.height; y++) {
			for (int x = 0; x < grid.width; x++) {
				Cell& current = grid.getCellRef(x, y);
				if (current.element == Elements::VOID)
					continue;

				current.temperature = nextTemperatures[x][y];

				if (current.element == Elements::STEAM && current.temperature < 100.0f)
					current.element = Elements::WATER;

				if (current.element == Elements::WATER && current.temperature > 100.0f)
					current.element = Elements::STEAM;

				if (current.element == Elements::LAVA && current.temperature < 200.0f)
					current.element = Elements::STONE;
			}
		}

		for (int y = grid.height - 1; y >= 0; y--) {
			std::vector<int> order = Utility::randomIntArray(grid.width);
			for (int _x = 0; _x < grid.width; _x++) {
				int x = order[_x];
				Cell& current = grid.getCellRef(x, y);
				if (current.element == Elements::VOID || current.updatedFlag == currentFrameFlag)
					continue;

				int dx, dy;
				if (pickHighestPriorityMove(evaluateMoves(current, grid.getNeighbourhood(x, y)), dx, dy)) {
					grid.swapCells(x, y, x + dx, y + dy);
				}

				current.updatedFlag = currentFrameFlag;
			}
		}

		currentFrameFlag = !currentFrameFlag;
	}
}
