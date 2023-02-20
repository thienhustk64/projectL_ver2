#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "menu_state.h"

using namespace std;

BaseObject g_background;
Menu menuState;
static SDL_Window *g_window = NULL;
static SDL_Renderer *g_screen = NULL;
static SDL_Event g_event;
TTF_Font* font = NULL;
char tileMap[] = "data//map.dat";

bool check_mouse_pos(SDL_Rect rect){
    int x, y;
    SDL_GetMouseState(&x, &y);
    bool value = true;
    if(x < rect.x){
        value = false;
    }
    else if(x > rect.x + rect.w){
        value = false;
    }
    else if(y < rect.y){
        value = false;
    }
    else if(y > rect.y + rect.h){
        value = false;
    }
    return value;
}


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
    if (TTF_Init() < 0){
        SDL_Log("%s", TTF_GetError());
        success = false;

    }
    font = TTF_OpenFont("font/arial.ttf", 30);
    if ( !font ) {
        SDL_Log("%s", TTF_GetError());
        success = false;
    }
    return success;
}

bool LoadBackground( string path){
    bool ret = g_background.LoadImg( path, g_screen);
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
    TTF_Quit();
}

bool initMenu(){
    if( menuState.initBackground( "data//background.png", g_screen) == false){
        return false;
    }
    if( menuState.addText( "GAME 2v2", font, g_screen, 400, 100) == false){
        return false;
    }
    if( menuState.addText( "JOIN GAME", font, g_screen, 400, 200) == false){
        return false;
    }

    if( menuState.addText( "HOST GAME", font, g_screen, 400, 300) == false){
        return false;
    }
    if( menuState.addText( "EXIT", font, g_screen, 450, 400) == false){
        return false;
    }
    return true;
}

int main( int argc, char *argv[]){

    if( InitData() == false){
        return -1;
    }
    if(initMenu() == false){
        return -1;
    }

    bool is_quit = false;
    while( !is_quit){
        // Load background
       

        while( SDL_PollEvent( &g_event)){
            switch( g_event.type){
                case SDL_WINDOWEVENT_CLOSE:
                    is_quit = true;
                    break;
                case SDL_KEYDOWN:{
                    switch ( g_event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            is_quit = true;
                            break;
                        default:
                            break;
                    }
                }
                case SDL_MOUSEBUTTONDOWN:{
                    if(check_mouse_pos(menuState.getRect(0))){
                        printf("123");
                    }
                    
                    break;
                }
                case SDL_QUIT:
                    is_quit = true;
                    break;
            }
        }
         SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear( g_screen);
        menuState.drawBackground( g_screen );
            if( check_mouse_pos( menuState.getRect(0))){
                            SDL_Rect box = menuState.getRect(0);
                            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
                            SDL_RenderFillRect(g_screen, &box);
                        }
        menuState.drawButton( g_screen );
        SDL_RenderPresent( g_screen);
    }
        

    close();
    printf("Hello");
    return 0;
}