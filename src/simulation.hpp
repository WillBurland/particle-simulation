#pragma once

namespace Simulation {
	void step(Grid& grid) {
		for (int i = grid.height - 2; i >= 0; i--) {
			for (int j = 0; j < grid.width; j++) {
				Cell current = grid.getCell(j, i);
				Cell below = grid.getCell(j, i + 1);
				if (below.element.density < current.element.density) {
					grid.swapCells(j, i, j, i + 1);
				}
			}
		}
	}
}