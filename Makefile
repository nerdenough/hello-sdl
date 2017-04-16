CC = g++
COMPILER_FLAGS = -Wall
LINKER_FLAGS = -framework SDL2 -framework SDL2_image

all: surfaces textures geometry

surfaces:
	$(CC) src/surfaces.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bin/surfaces

textures:
	$(CC) src/textures.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bin/textures

geometry:
	$(CC) src/geometry.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bin/geometry
