CC = g++
BUILDDIR = build
SOURCEDIR = src\game\cpp
INCLUDE_PATHS = -I src\sdl\include -I src\game\header
LIBRARY_PATHS = -L src\sdl\lib
COMPILER_FLAGS = -Wall -g -c
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lws2_32 -lSDL2_image

OBJS := $(BUILDDIR)\trieu.o $(BUILDDIR)\CommonFunc.o  $(BUILDDIR)\BaseObject.o $(BUILDDIR)\menu_state.o
OBJ_NAME = trieu

all : $(OBJS) 
	$(CC) $(OBJS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)

$(BUILDDIR)\CommonFunc.o: $(SOURCEDIR)\CommonFunc.cpp
	$(CC) $(SOURCEDIR)\CommonFunc.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\CommonFunc.o

$(BUILDDIR)\menu_state.o: $(SOURCEDIR)\menu_state.cpp
	$(CC) $(SOURCEDIR)\menu_state.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\menu_state.o

$(BUILDDIR)\BaseObject.o: $(SOURCEDIR)\BaseObject.cpp
	$(CC) $(SOURCEDIR)\BaseObject.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\BaseObject.o

$(BUILDDIR)\trieu.o: trieu.cpp
	$(CC) trieu.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILDDIR)\trieu.o

clean:
	del trieu.exe
	del $(BUILDDIR)\*.o