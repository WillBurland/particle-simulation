struct Element {
	int id;
	std::string name;
	sf::Color colour;
};

struct Elements {
	static const Element VOID;
	static const Element SAND;
	static const Element WATER;

	static const std::vector<Element>& all() {
		static const std::vector<Element> elems = {
			VOID,
			SAND,
			WATER
		};
		return elems;
	}

	static const Element& fromId(int id) {
		const auto& elems = all();
		for (const auto& e : elems)
			if (e.id == id)
				return e;
		return VOID;
	}
};

const Element Elements::VOID  = { 0, "Void",  sf::Color(  0,   0,   0) };
const Element Elements::SAND  = { 1, "Sand",  sf::Color(255, 255,  64) };
const Element Elements::WATER = { 2, "Water", sf::Color( 64,  64, 255) };
