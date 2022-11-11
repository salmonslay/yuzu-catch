# Mac, gcc(g++-12) och sdl2 genom homebrew
# Windows, gcc(g++) och sdl2 genom MSYS2 mingw-w64

# DIN KÄLLKOD-var dina .cpp-filer finns
SRC_DIR = src
# FILNAMNET för ditt program som skall byggas, och VAR
OBJ_NAME = play
BUILD_DIR = build/debug

# fruitwork stuff
FRUITWORK_INCLUDE_DIR = ../fruitwork/include
FRUITWORK_SRC_DIR = ../fruitwork/src

# KOMPILATOR, g++/g++-12/c++/c++-12 beroende på installation
# Mac GCC COMPILER
#CC = g++-12
# Windows GCC COMPILER
CC = g++

# Valbara kompileringsflaggor(options)
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g

# ALLA filer med filändelsen .cpp i foldern SRC_DIR
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(FRUITWORK_SRC_DIR)/*.cpp)

# INKLUDERINGSFILER-var dina header-filer finns
# Mac INCLUDE_PATHS!
#INCLUDE_PATHS = -Iinclude -I/usr/local/include
# Windows INCLUDE_PATHS!
INCLUDE_PATHS = -Iinclude -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/SDL2 -I$(FRUITWORK_INCLUDE_DIR)

# BIBLIOTEKSFILER
# Mac LIBRARY_PATHS!
#LIBRARY_PATHS = -Llib -L/usr/local/lib
# Windows LIBRARY_PATHS
LIBRARY_PATHS = -Llib -LC:/msys64/mingw64/lib

# LÄNKNING - objekfiler som används vid länkning. Enklare program utan SDL behöver normalt inte några speciella länk-flaggor
#LINKER_FLAGS =
# Om SDL2 används, Mac LINKER_FLAGS!
#LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
# Om SDL2 används, Windows LINKER_FLAGS!
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

all:
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) $(LINKER_FLAGS) -o $(BUILD_DIR)/$(OBJ_NAME)



