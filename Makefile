OBJS = main.cpp
CC = gcc
INCLUDE_PATHS = -I src\include -I src\game -I src\game\header
LIBRARY_PATHS = -L src\lib
COMPILER_FLAGS = -Wall 
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lws2_32 -lSDL2_image
OBJ_NAME = main
all : $(OBJS) 
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
clean:
	del main.exe