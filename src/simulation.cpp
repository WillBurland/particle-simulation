#include "simulation.hpp"

#include "cell.hpp"
#include "element.hpp"
#include "grid.hpp"

#include <cstdlib>

namespace Simulation {
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
		for (int y = grid.height - 2; y >= 0; y--) {
			for (int x = 0; x < grid.width; x++) {
				Cell& current = grid.getCellRef(x, y);
				if (current.element == Elements::VOID)
					continue;

				auto n = grid.getNeighbourhood(x, y);
				MoveOptions moves = evaluateMoves(current, n);

				int dx, dy;
				if (pickHighestPriorityMove(moves, dx, dy))
					grid.swapCells(x, y, x + dx, y + dy);
			}
		}
	}
}
