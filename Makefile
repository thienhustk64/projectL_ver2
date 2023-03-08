CC = g++
BUILDDIR = build
SOURCEDIR = src\game\cpp
SOCKETDIR = src\socket\cpp
INCLUDE_PATHS = -I src\sdl\include -I src\game\header
LIBRARY_PATHS = -L src\sdl\lib
COMPILER_FLAGS = -Wall -g -c
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lws2_32 -lSDL2_image -pthread

OBJS := $(BUILDDIR)\main.o $(BUILDDIR)\CommonFunc.o $(BUILDDIR)\BaseObject.o $(BUILDDIR)\GameMap.o $(BUILDDIR)\Animation.o $(BUILDDIR)\Player.o $(BUILDDIR)\tinyxml2.o $(BUILDDIR)\ultis.o $(BUILDDIR)\Input.o $(BUILDDIR)\Collision.o $(BUILDDIR)\menu_state.o $(BUILDDIR)\Screen.o $(BUILDDIR)\Ingame.o
OBJ_NAME = main

all : $(OBJS)
	$(CC) $(OBJS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	

$(BUILDDIR)\tinyxml2.o: $(SOURCEDIR)\tinyxml2.cpp
	$(CC) $(SOURCEDIR)\tinyxml2.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\tinyxml2.o

$(BUILDDIR)\packet.o: $(SOCKETDIR)\packet.cpp
	$(CC) $(SOCKETDIR)\packet.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\packet.o

$(BUILDDIR)\client.o: $(SOCKETDIR)\client.cpp
	$(CC) $(SOCKETDIR)\client.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\client.o

$(BUILDDIR)\ultis.o: $(SOURCEDIR)\ultis.cpp
	$(CC) $(SOURCEDIR)\ultis.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\ultis.o

$(BUILDDIR)\CommonFunc.o: $(SOURCEDIR)\CommonFunc.cpp
	$(CC) $(SOURCEDIR)\CommonFunc.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\CommonFunc.o

$(BUILDDIR)\BaseObject.o: $(SOURCEDIR)\BaseObject.cpp
	$(CC) $(SOURCEDIR)\BaseObject.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\BaseObject.o

$(BUILDDIR)\GameMap.o: $(SOURCEDIR)\GameMap.cpp
	$(CC) $(SOURCEDIR)\GameMap.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\GameMap.o

$(BUILDDIR)\Animation.o: $(SOURCEDIR)\Animation.cpp
	$(CC) $(SOURCEDIR)\Animation.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\Animation.o

$(BUILDDIR)\Player.o: $(SOURCEDIR)\Player.cpp
	$(CC) $(SOURCEDIR)\Player.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\Player.o

$(BUILDDIR)\Input.o: $(SOURCEDIR)\Input.cpp
	$(CC) $(SOURCEDIR)\Input.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\Input.o

$(BUILDDIR)\Collision.o: $(SOURCEDIR)\Collision.cpp
	$(CC) $(SOURCEDIR)\Collision.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\Collision.o

$(BUILDDIR)\menu_state.o: $(SOURCEDIR)\menu_state.cpp
	$(CC) $(SOURCEDIR)\menu_state.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\menu_state.o

$(BUILDDIR)\Screen.o: $(SOURCEDIR)\Screen.cpp
	$(CC) $(SOURCEDIR)\Screen.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\Screen.o

$(BUILDDIR)\Ingame.o: $(SOURCEDIR)\Ingame.cpp
	$(CC) $(SOURCEDIR)\Ingame.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\Ingame.o

$(BUILDDIR)\main.o: main.cpp
	$(CC) main.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\main.o

clean:
	del $(BUILDDIR)\*.o
	del main.exe