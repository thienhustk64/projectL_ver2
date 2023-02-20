#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "Player.h"
#include "Input.h"
#include "Client.c"
#include "Collision.h"

BaseObject g_background;
static SDL_Window *g_window = NULL;
static SDL_Renderer *g_screen = NULL;
static SDL_Event g_event;
TTF_Font* font = NULL;
char tileMap[] = "data//map.dat";
Uint32 start;

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

void drawGame( int health1, int health2){
    SDL_Rect rect1 = {10, 20, 400, 50};
    SDL_Rect rect2 = {550, 20, 400, 50};
    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
    SDL_RenderDrawRect(g_screen, &rect1);
    SDL_RenderDrawRect(g_screen, &rect2);
    rect1.w = max( 0, health1*4);
    rect2.w = max( 0, health2*4);
    SDL_SetRenderDrawColor(g_screen, 235, 7, 60, 0);
    SDL_RenderFillRect(g_screen, &rect1);
    SDL_RenderFillRect(g_screen, &rect2);

}

bool inGame( Player *player1, Player *player2, Input keyboard){
    std::string triggerAction;
    std::string triggerCollision;
    start = SDL_GetTicks();
    // Xem tk player có out
    bool is_quit = false;
    bool is_collision = false;
    is_quit = keyboard.getEvent( &g_event, player2->isInverted());
    // Trigger client
    triggerAction = keyboard.getTrigger();
    // Gửi lên cho server
    player2->updateState( triggerAction);

    SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    SDL_RenderClear( g_screen);
    is_collision = player1->checkCollision( player2->getCollision());
    // trigger of collision
    triggerCollision = player2->getTrigger();
    if( is_collision){
        player1->updateState( triggerCollision);
        player2->checkSkill( player1->isHurt());
    }
    g_background.RenderMap( g_screen, NULL);

    player2->DoPlayer(is_collision);
    player2->draw( g_screen, font);

    player1->DoPlayer(is_collision);
    player1->draw( g_screen, font);
    drawGame( player1->getHealth(), player2->getHealth());
    SDL_RenderPresent( g_screen);
    if(1000/FPS > SDL_GetTicks()-start) {
        SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
    }
    return is_quit;
}

int main( int argc, char *argv[]){
    if( InitData() == false){
        return -1;
    }

    if( LoadBackground( "data//background.png") == false){
        return -1;
    }

    Input keyboard;

    Player player1;
    player1.initialize( "Player1", "xml/ryu.xml", "xml/fireball.xml", "xml/fireball_collision.xml", true, 400, 200);

    Player player2;
    player2.initialize( "Player2", "xml/ryu.xml", "xml/fireball.xml", "xml/fireball_collision.xml", false, 800, 200);

    // Collision test1;
    // Collision test2;
    // test1.initCollision( 0, 0, 10, 10);
    // test2.initCollision( 8, 0, 10, 10);
    // if( test1.checkCollision( test2)){
    //     printf("123");
    // }

    bool is_quit = false;
    
    while( !is_quit){
        is_quit = inGame( &player1, &player2, keyboard);
    }

    close();
    printf("Hello");
    return 0;
}