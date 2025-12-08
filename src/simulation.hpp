#pragma once

#include <array>
#include <vector>
#include <utility>

struct Cell;
struct Grid;
struct Element;
struct Elements;

namespace Simulation {
	struct MoveOptions {
		int priorites[3][3]{};
	};

	MoveOptions evaluateMoves(const Cell& current, const std::array<std::array<Cell*, 3>, 3>& n);
	bool pickHighestPriorityMove(const MoveOptions& m, int& outDx, int& outDy);
	void step(Grid& grid);
}
