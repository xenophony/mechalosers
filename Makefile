####################################################################
# Declare some Makefile variables
####################################################################
CC = g++
LANG_STD = -std=c++17
INCLUDE_PATH = -I./libs -I/usr/local/include -I/opt/homebrew/include

SRC_FILES = ./src/*.cpp \
            ./src/Game/*.cpp \
            ./src/Logger/*.cpp \
            ./src/AssetStore/*.cpp

LINKER_FLAGS = `sdl2-config --libs --cflags` \
                -lSDL2_image \
                -lSDL2_ttf \
                -lSDL2_mixer \
                -llua5.4
OBJ_NAME = Maganstein3D

####################################################################
# Declare some Makefile rules
####################################################################

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./$(OBJ_NAME)

clean:
	rm $(OBJ_NAME)

verbose: 
	$(CC) $(COMPILER_FLAGS) -v $(LANG_STD) $(INCLUDE_PATH) $(SRC_FILES) $(LINKER_FLAGS)  -o $(OBJ_NAME)


