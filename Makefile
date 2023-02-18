CC = g++
BUILDDIR = build
SOURCEDIR = src\game\cpp
INCLUDE_PATHS = -I src\sdl\include -I src\game\header
LIBRARY_PATHS = -L src\sdl\lib
COMPILER_FLAGS = -Wall -g -c
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lws2_32 -lSDL2_image

OBJS := $(BUILDDIR)\main.o $(BUILDDIR)\CommonFunc.o $(BUILDDIR)\BaseObject.o $(BUILDDIR)\GameMap.o $(BUILDDIR)\menu.o
OBJ_NAME = main

all : $(OBJS) 
	$(CC) $(OBJS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)

$(BUILDDIR)\CommonFunc.o: $(SOURCEDIR)\CommonFunc.cpp
	$(CC) $(SOURCEDIR)\CommonFunc.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\CommonFunc.o

$(BUILDDIR)\BaseObject.o: $(SOURCEDIR)\BaseObject.cpp
	$(CC) $(SOURCEDIR)\BaseObject.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\BaseObject.o

$(BUILDDIR)\GameMap.o: $(SOURCEDIR)\GameMap.cpp
	$(CC) $(SOURCEDIR)\GameMap.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\GameMap.o

$(BUILDDIR)\menu.o: $(SOURCEDIR)\menu.cpp
	$(CC) $(SOURCEDIR)\menu.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\menu.o

$(BUILDDIR)\main.o: main.cpp
	$(CC) main.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\main.o

clean:
	del main.exe
	del $(BUILDDIR)\*.o