#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "CommonFunc.h"

int main( int argc, char *argv[]){

    if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == 1 )
        return -1;
    return 0;
}