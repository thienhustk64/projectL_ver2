#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "Player.h"
#include "Input.h"

BaseObject g_background;
GameMap game_map;
static SDL_Window *g_window = NULL;
static SDL_Renderer *g_screen = NULL;
static SDL_Event g_event;
char tileMap[] = "data//map.dat";

bool InitData(){
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if( ret < 0)
        return false;
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    // Tao window
    g_window = SDL_CreateWindow( "Project L", 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SCREEN_WIDTH, SCREEN_HEIGHT, 
                                SDL_WINDOW_SHOWN);
    if( g_window == NULL){
        success = false;
    }else{
        g_screen = SDL_CreateRenderer( g_window, -1, SDL_RENDERER_ACCELERATED);
        if( g_screen == NULL){
            success = false;
        }else{
            SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if( !(IMG_Init( imgFlags) && imgFlags)){
                success = false;
            }
        }
    }
    return success;
}

bool LoadBackground(){
    bool ret = g_background.LoadImg( "data//background.png", g_screen);
    return ret;
}

void close(){
    g_background.Free();
    SDL_DestroyRenderer( g_screen);
    g_screen = NULL;

    SDL_DestroyWindow( g_window);
    g_window = NULL;
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char *argv[]){
    Uint32 start;
    std::string trigger;

    if( InitData() == false){
        return -1;
    }
    if( LoadBackground() == false){
        return -1;
    }

    game_map.LoadMap( tileMap);

    Input keyboard;

    Player player1;
    player1.initialize("xml/ryu.xml", "xml/fireball.xml", "xml/fireball_collision.xml", true, 50, 50);

    // Player player2;
    // player2.initialize("data/ryu/ryu.xml","data/ryu/moves.xml",  false);


    bool is_quit = false;
    while( !is_quit){
        start = SDL_GetTicks();
        is_quit = keyboard.getEvent( &g_event);
        trigger = keyboard.getTrigger();

        player1.updateState( trigger);

        SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear( g_screen);

        g_background.RenderMap( g_screen, NULL);
        player1.draw( g_screen);
        SDL_RenderPresent( g_screen);
        if(1000/FPS > SDL_GetTicks()-start) {
            SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
        }
    }

    close();
    printf("Hello");
    return 0;
}