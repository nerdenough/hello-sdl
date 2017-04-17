CC = g++
COMPILER_FLAGS = -Wall
LINKER_FLAGS = -framework SDL2 -framework SDL2_image

all: surfaces textures geometry viewports

surfaces:
	$(CC) src/surfaces.cc $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bin/surfaces

textures:
	$(CC) src/textures.cc $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bin/textures

geometry:
	$(CC) src/geometry.cc $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bin/geometry

viewports:
	$(CC) src/viewports.cc $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bin/viewports
