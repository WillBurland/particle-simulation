CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc -I./include

LIBS = \
	-L./lib \
	-lImGui-SFML \
	-lsfml-graphics \
	-lsfml-window \
	-lsfml-system \
	-lGL \
	-lGLEW

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))
TARGET = build/particle-sim

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build/*

run: $(TARGET)
	./$(TARGET)
