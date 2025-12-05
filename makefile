CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include -I./include/imgui

LIBS = \
    -L./lib \
    -lImGui-SFML \
    -lsfml-graphics \
    -lsfml-window \
    -lsfml-system \
    -lGL \
    -lGLEW

SRC = main.cpp
TARGET = particle-sim

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
