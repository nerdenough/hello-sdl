CC = g++
SRC_DIR = src
OBJS = $(SRC_DIR)/main.cpp
EXECUTABLE = bin/program
COMPILER_FLAGS = -w
LINKER_FLAGS = -framework SDL2 -framework SDL2_image

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE)
