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
DEPS = $(OBJ:.o=.d)
TARGET = build/particle-sim

OBJDIR = build

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -MMD -MF $(patsubst %.o,%.d,$@) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

-include $(DEPS)

clean:
	rm -rf build/*

run: $(TARGET)
	./$(TARGET)
