#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include <windows.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// Screen
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;
const int FPS = 15;
const int GROUND = 550;
const int SCALE = 20;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xff;

#define TILE_SIZE 64

#define MAX_MAP_X 15
#define MAX_MAP_Y 10

typedef struct Map{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
}Map;
#endif