struct Element {
	int id;
	std::string name;
	sf::Color colour;
};

static std::vector<Element> elements = {
	{ 0, "Void",  sf::Color(  0,   0,   0) },
	{ 1, "Sand",  sf::Color(255, 255,  64) },
	{ 2, "Water", sf::Color( 64,  64, 255) }
};