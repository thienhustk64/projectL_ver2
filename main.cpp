#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "Player.h"
#include "Input.h"
#include "Collision.h"
#include "Screen.h"
#include "Ingame.h"

//local variable

BaseObject g_background;
static SDL_Window *g_window = NULL;
static SDL_Renderer *g_screen = NULL;
static SDL_Event g_event;
TTF_Font* font = NULL;
Uint32 start;

Menu *menuState = new Menu();
Menu *menuJoin = new Menu();
Menu *menuHost = new Menu();
Menu *menuLogin = new Menu();
Menu *menuPause = new Menu();

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

bool InitGame(){
    if( InitData() == false){
        printf("data");
        return false;
    }
    if( initLogin( menuLogin, font, g_screen) == false){
        printf("login");
        return false;
    }
    if( initMenu( menuState, font, g_screen) == false){
        printf("menu");
        return false;
    }
    if( initJoin( menuJoin, font, g_screen) == false){
        printf("join");
        return false;
    }
    if( initHost( menuHost, font, g_screen) == false){
        printf("host");
        return false;
    }
    if( initPause( menuPause, font, g_screen) == false){
        printf("host");
        return false;
    }
    if( LoadBackground( "data//background.png") == false){
        printf("background");
        return false;
    }
    return true;
}

void close_game(){
    menuState->Free();
    menuHost->Free();
    menuJoin->Free();
    menuPause->Free();
    g_background.Free();
    menuLogin->Free();
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

State inGame( Player *player1, Player *player2, Input keyboard){
    std::string triggerAction;
    std::string triggerCollision;
    // Xem tk player có out
    int is_quit = 0;
    bool is_collision = false;
    while( !is_quit){
        start = SDL_GetTicks();
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
        
        if( checkWinner( player1, player2) != 0){

            drawGame( player1->getHealth(), player2->getHealth());
            SDL_RenderPresent( g_screen);
            player1->reset( 400, 200);
            player2->reset( 800, 200);
        }else{
            drawGame( player1->getHealth(), player2->getHealth());
            SDL_RenderPresent( g_screen);
        }
        // if(1000/FPS > SDL_GetTicks()-start) {
        //     SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
        // }
    }
    if( is_quit == 1){
        return EXIT;
    }else{
        return PAUSE;
    }
}

void Login(){
    State select = LOGIN;
    bool is_quit = false;
    while( !is_quit){
        switch(select){
            case EXIT:
                is_quit = true;
                select = EXIT;
                break;
            case LOGIN:

                break;
            case SIGNIN:
                break;
            case SIGNUP:
                // select = CHECKLOGIN;
                break;
            case CHECKLOGIN:{
                break;
            }
                
            default:
                select = LOGIN;
                // is_quit = true;
                break;
        }
    }
}

int main( int argc, char *argv[]){
    
    if( !InitGame()){
        return -1;
    }

    State select = MENU;
    State prev_select = MENU;
    Input keyboard;
    Player player1;
    player1.initialize( "Player1", "xml/ryu.xml", "xml/fireball.xml", "xml/fireball_collision.xml", true, 400, 200);
    Player player2;
    player2.initialize( "Player2", "xml/ryu.xml", "xml/fireball.xml", "xml/fireball_collision.xml", false, 800, 200);
    
    bool is_quit = false;
    while( !is_quit){
        switch(select){
            case EXIT:
                is_quit = true;
                break;
            case MENU:
                select = showMenu( menuState, g_screen, g_event);
                if( select != BACK){
                    prev_select = MENU;
                }
                break;
            case HOST:
                select = showHost( menuHost, g_screen, g_event);
                if( select != BACK){
                    prev_select = HOST;
                }
                break;
            case JOIN:
                select = showJoin( menuJoin, g_screen, g_event);
                if( select != BACK){
                    prev_select = JOIN;
                }
                break;
            case BACK:
                select = prev_select;
                break;
            case INGAME:
                select = inGame( &player1, &player2, keyboard);
                break;
            case PAUSE:
                select = showPause( menuPause, g_screen, g_event);
                break;
            default:
                select = MENU;
                break;
        }
    }

    close_game();
    printf("Hello");
    return 0;
}