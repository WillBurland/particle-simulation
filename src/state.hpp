#pragma once

#include <string>

struct State {
	std::string name;
	int moves[3][3];
};

struct States {
	static inline const State SOLID = { "Solid", {
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 1, 2, 1 }
	}};
	static inline const State LIQUID = { "Liquid", {
		{ 0, 0, 0 },
		{ 1, 0, 1 },
		{ 2, 3, 2 }
	}};
	static inline const State GAS = { "Gas", {
		{ 1, 1, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 }
	}};
};
