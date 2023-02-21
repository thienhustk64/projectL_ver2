#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "menu_state.h"

using namespace std;

enum State {
    MENU,
    HOST,
    JOIN,
    EXIT,
    BACK,
    // LOGIN,
    NOCHANGE
};

BaseObject g_background;
Menu menuState;
Menu menuJoin;
Menu menuHost;
// Menu menuLogin;
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

//----------------------------------------------------
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
//-----------------------------------------------DANGNHAP------------------------
// State showLogin(){
//     State select = NOCHANGE;
//     while( SDL_PollEvent( &g_event)){
//         switch( g_event.type){
//             case SDL_WINDOWEVENT_CLOSE:
//                 select = EXIT;
//                 break;
//             case SDL_KEYDOWN:{
//                 switch ( g_event.key.keysym.sym){
//                     case SDLK_ESCAPE:
//                         select = EXIT;
//                         break;
//                     default:
//                         break;
//                 }
//             }
//             case SDL_MOUSEBUTTONDOWN:{
//                 if(check_mouse_pos(menuLogin.getRect(1))){
//                     select = NOCHANGE;
//                 // }else if(check_mouse_pos(menuJoin.getRect(2))){
//                 //     select = NOCHANGE;
//                 }else if(check_mouse_pos(menuLogin.getRect(3))){
//                     select = BACK;
//                 }
//                 break;
//             }
//             case SDL_QUIT:
//                 select = EXIT;
//                 break;
//         }
//     }
//     SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
//     SDL_RenderClear( g_screen);
//     //menuLogin.drawBackground
//     menuLogin.drawBackground( g_screen );
//     if( check_mouse_pos( menuLogin.getRect(1))){
//         SDL_Rect box = menuLogin.getRect(1);
//         SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
//         SDL_RenderFillRect(g_screen, &box);
//     }else if(check_mouse_pos( menuLogin.getRect(3))){
//         SDL_Rect box = menuLogin.getRect(3);
//         SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
//         SDL_RenderFillRect(g_screen, &box);
//     }
//     menuLogin.drawButton( g_screen );
//     SDL_RenderPresent( g_screen);
//     return select;
// }
// // //-----------------------------------------------------------------------------
// bool initLogin(){
//     if( menuLogin.initBackground( "data//background.png", g_screen) == false){
//         return false;
//     }
//     if( menuLogin.addText( "LOGIN", font, g_screen, 50, 25) == false){
//         return false;
//     }
//     if( menuLogin.addText( "TEN ", font, g_screen, 150, 500) == false){
//         return false;
//     }
//     // text input
//     if( menuLogin.addText( "MATKHAU", font, g_screen, 150, 100) == false){
//         return false;
//     }
//     if( menuLogin.addText( "EXIT", font, g_screen, 700, 500) == false){
//         return false;
//     }
//     return true;
// }
// -------------------------------------------------showMenu-----------------------------
State showMenu(){
    State select = NOCHANGE;
    while( SDL_PollEvent( &g_event)){
        switch( g_event.type){
            case SDL_WINDOWEVENT_CLOSE:
                select = EXIT;
                break;
            case SDL_KEYDOWN:{
                switch ( g_event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        select = EXIT;
                        break;
                    default:
                        break;
                }
            }
            case SDL_MOUSEBUTTONDOWN:{
                if(check_mouse_pos(menuState.getRect(1))){
                    select = HOST;
                }else if(check_mouse_pos(menuState.getRect(2))){
                    select = JOIN;
                }else if(check_mouse_pos(menuState.getRect(3))){
                    select = EXIT;
                }
                break;
            }
            case SDL_QUIT:
                select = EXIT;
                break;
        }
    }
    SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    SDL_RenderClear( g_screen);
    //menuState.drawBackground
    menuState.drawBackground( g_screen );
    if( check_mouse_pos( menuState.getRect(1))){
        SDL_Rect box = menuState.getRect(1);
        SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
        SDL_RenderFillRect(g_screen, &box);
    }else if(check_mouse_pos( menuState.getRect(2))){
        SDL_Rect box = menuState.getRect(2);
        SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
        SDL_RenderFillRect(g_screen, &box);
    }else if(check_mouse_pos( menuState.getRect(3))){
        SDL_Rect box = menuState.getRect(3);
        SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
        SDL_RenderFillRect(g_screen, &box);
    }
    menuState.drawButton( g_screen );
    SDL_RenderPresent( g_screen);
    return select;
}
//-------------------------------------------------------------
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

// ----------------------------------------Show JOIN ---------------------------------------------------------------------------
State showJoin(){
    State select = NOCHANGE;
    while( SDL_PollEvent( &g_event)){
        switch( g_event.type){
            case SDL_WINDOWEVENT_CLOSE:
                select = EXIT;
                break;
            case SDL_KEYDOWN:{
                switch ( g_event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        select = EXIT;
                        break;
                    default:
                        break;
                }
            }
            case SDL_MOUSEBUTTONDOWN:{
                if(check_mouse_pos(menuJoin.getRect(1))){
                    select = NOCHANGE;
                // }else if(check_mouse_pos(menuJoin.getRect(2))){
                //     select = NOCHANGE;
                }else if(check_mouse_pos(menuJoin.getRect(3))){
                    select = BACK;
                }
                break;
            }
            case SDL_QUIT:
                select = EXIT;
                break;
        }
    }
    SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    SDL_RenderClear( g_screen);
    //menuJoin.drawBackground
    menuJoin.drawBackground( g_screen );
    if( check_mouse_pos( menuJoin.getRect(1))){
        SDL_Rect box = menuJoin.getRect(1);
        SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
        SDL_RenderFillRect(g_screen, &box);
    }else if(check_mouse_pos( menuJoin.getRect(3))){
        SDL_Rect box = menuJoin.getRect(3);
        SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
        SDL_RenderFillRect(g_screen, &box);
    }
    menuJoin.drawButton( g_screen );
    SDL_RenderPresent( g_screen);
    return select;
}
// //-----------------------------------------------------------------------------
bool initJoin(){
    if( menuJoin.initBackground( "data//background.png", g_screen) == false){
        return false;
    }
    if( menuJoin.addText( "MENU JOIN", font, g_screen, 50, 25) == false){
        return false;
    }
    if( menuJoin.addText( "JOIN ", font, g_screen, 150, 500) == false){
        return false;
    }
    // text input
    if( menuJoin.addText( "Name", font, g_screen, 150, 100) == false){
        return false;
    }
    if( menuJoin.addText( "Back", font, g_screen, 700, 500) == false){
        return false;
    }
    return true;
}
// ----------------------------------------Show HOST ---------------------------------------------------------------------------
State showHost(){
    State select = NOCHANGE;
    while( SDL_PollEvent( &g_event)){
        switch( g_event.type){
            case SDL_WINDOWEVENT_CLOSE:
                select = EXIT;
                break;
            case SDL_KEYDOWN:{
                switch ( g_event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        select = EXIT;
                        break;
                    default:
                        break;
                }
            }
            case SDL_MOUSEBUTTONDOWN:{
                if(check_mouse_pos(menuHost.getRect(1))){
                    select = NOCHANGE;
                // }else if(check_mouse_pos(menuHost.getRect(2))){
                //     select = NOCHANGE;
                }else if(check_mouse_pos(menuHost.getRect(3))){
                    select = BACK;
                }
                break;
            }
            case SDL_QUIT:
                select = EXIT;
                break;
        }
    }
    SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    SDL_RenderClear( g_screen);
    //menuHost.drawBackground
    menuHost.drawBackground( g_screen );
    if( check_mouse_pos( menuHost.getRect(1))){
        SDL_Rect box = menuHost.getRect(1);
        SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
        SDL_RenderFillRect(g_screen, &box);
    }else if(check_mouse_pos( menuHost.getRect(3))){
        SDL_Rect box = menuHost.getRect(3);
        SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
        SDL_RenderFillRect(g_screen, &box);
    }
    menuHost.drawButton( g_screen );
    SDL_RenderPresent( g_screen);
    return select;
}
//-----------------------------------------------------------------------------
bool initHost(){
    if( menuHost.initBackground( "data//background.png", g_screen) == false){
        return false;
    }
    if( menuHost.addText( "MENU JOIN", font, g_screen, 50, 25) == false){
        return false;
    }
    if( menuHost.addText( "CREATE ", font, g_screen, 150, 500) == false){
        return false;
    }
    if( menuHost.addText( "Name", font, g_screen, 150, 100) == false){
        return false;
    }
    if( menuHost.addText( "Back", font, g_screen, 700, 500) == false){
        return false;
    }
    return true;
}

//------------------------------------------------------------------

int main( int argc, char *argv[]){

    if(InitData() == false){
        return -1;
    }
    // if( initLogin() == false){
    //     return -1;
    // }
    if( initMenu() == false){
        return -1;
    }
    if( initJoin() == false){
        return -1;
    }

    if( initHost() == false){
        return -1;
    }


//-----------------------------------------------------------------
    bool is_quit = false;
    State select = MENU;
    State select_now = MENU;
    State prev_select = MENU;
    // 0: exit, 1: menu, 2 host, 3 join
    while( !is_quit){
        // Load background
        switch(select){
            case EXIT:
                is_quit = true;
                break;
            case MENU:
                select_now = select;
                select = showMenu();
                break;
            case HOST:
                select_now = select;
                select = showHost();
                break;
            case JOIN:
                select_now = select;
                select = showJoin();
                break;
            case BACK:
                select = prev_select;
                select_now = prev_select;
                break;
            case NOCHANGE:
                select = select_now;
                break;
        }
    }
        

    close();
    printf("Hello");
    return 0;
}