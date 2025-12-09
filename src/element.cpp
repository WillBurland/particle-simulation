#include "element.hpp"

const std::vector<Element>& Elements::all() {
	static const std::vector<Element> elems = { VOID, SAND, STONE, WATER, LAVA, STEAM };
	return elems;
}

const Element& Elements::fromId(int id) {
	const std::vector<Element>& elems = all();
	for (const Element& e : elems)
		if (e.id == id)
			return e;
	return VOID;
}
