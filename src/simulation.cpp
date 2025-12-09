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
		for (int y = grid.height - 1; y >= 0; y--) {
			std::vector<int> order = Utility::randomIntArray(grid.width);
			for (int _x = 0; _x < grid.width; _x++) {
				int x = order.at(_x);
				Cell& current = grid.getCellRef(x, y);
				if (current.element == Elements::VOID || current.updatedFlag == currentFrameFlag)
					continue;

				current.updatedFlag = currentFrameFlag;

				int dx, dy;
				if (pickHighestPriorityMove(evaluateMoves(current, grid.getNeighbourhood(x, y)), dx, dy))
					grid.swapCells(x, y, x + dx, y + dy);
			}
		}

		currentFrameFlag = !currentFrameFlag;
	}
}
