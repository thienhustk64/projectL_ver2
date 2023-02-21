#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "menu_state.h"
#include <stdlib.h>

using namespace std;

enum State {
    MENU,
    HOST,
    JOIN,
    EXIT,
    BACK,
    LOGIN,
    PAUSE,
    NOCHANGE
};

BaseObject g_background;
Menu menuLogin;
Menu menuState;
Menu menuJoin;
Menu menuHost;
Menu menuPause;
static SDL_Window *g_window = NULL;
static SDL_Renderer *g_screen = NULL;
static SDL_Event g_event;
TTF_Font* font = NULL;
char tileMap[] = "data//map.dat";
char* userName;
char* password;
char* room;
SDL_Rect userNameRect = {500, 200, 300, 30};
SDL_Rect passwordRect = {500, 300, 300, 30};
SDL_Rect roomRect = {300, 300, 300, 300};
int tr = 0;
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
State showLogin(){
    State select = NOCHANGE;
    while( SDL_PollEvent( &g_event)){
        switch( g_event.type){
            case SDL_TEXTINPUT:
                    if(strlen(userName) < 30){
                        if(tr == 0){
                            strcat(userName, g_event.text.text);
                        }
                        if(strlen(userName) >= 0 ){
                            menuLogin.addTextInput( userName, font, g_screen, 500, 200);
                            menuLogin.addTextInputx( password, font, g_screen, 500, 300);
                        }
                    }
                    if(strlen(password) < 30){
                        if(tr == 1){
                            strcat(password, g_event.text.text);
                        }
                        if(strlen(password) >= 0 ){
                            menuLogin.addTextInput( userName, font, g_screen, 500, 200);
                            menuLogin.addTextInputx( password, font, g_screen, 500, 300);
                        }
                    }
                break;
            case SDL_WINDOWEVENT_CLOSE:
                select = EXIT;
                break;
            case SDL_KEYDOWN:
                switch ( g_event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        select = EXIT;
                        break;
                    case SDLK_BACKSPACE:
                            if(tr == 0){
                                userName[strlen(userName) - 1] = '\0';
                            }
                            if(strlen(userName) >= 0 ){
                                menuLogin.addTextInput( userName, font, g_screen, 500, 200);
                                menuLogin.addTextInputx( password, font, g_screen, 500, 300);
                            }
                            if(tr == 1){
                                password[strlen(password) - 1] = '\0';
                            }
                            if(strlen(password) >= 0 ){
                                menuLogin.addTextInput( userName, font, g_screen, 500, 200);
                                menuLogin.addTextInputx( password, font, g_screen, 500, 300);
                            }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(check_mouse_pos(menuLogin.getRect(3))){
                    select = MENU;
                }else if(check_mouse_pos(menuLogin.getRect(4))){
                    select = NOCHANGE;
                } else if(check_mouse_pos(menuLogin.getRect(5))){
                    select = EXIT;
                }else if(check_mouse_pos(userNameRect)){
                    tr = 0;
                }else if(check_mouse_pos(passwordRect)){
                    tr = 1;
                }
                break;
            case SDL_QUIT:
                select = EXIT;
                break;
        }
    }
    SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    SDL_RenderClear( g_screen);
    
    //menuLogin.drawBackground
    menuLogin.drawBackground( g_screen );
    SDL_Rect box;
        SDL_SetRenderDrawColor(g_screen, 0, 255, 255, 255);
        SDL_RenderFillRect(g_screen, &userNameRect);
        SDL_RenderFillRect(g_screen, &passwordRect);
    if( check_mouse_pos( menuLogin.getRect(3))){
         box = menuLogin.getRect(3);
        SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
        SDL_RenderFillRect(g_screen, &box);
    }else if(check_mouse_pos( menuLogin.getRect(4))){
         box = menuLogin.getRect(4);
        SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
        SDL_RenderFillRect(g_screen, &box);
    }else if(check_mouse_pos( menuLogin.getRect(5))){
         box = menuLogin.getRect(5);
        SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
        SDL_RenderFillRect(g_screen, &box);
    }
    menuLogin.drawTextInput( g_screen);
    menuLogin.drawButton( g_screen );
    SDL_RenderPresent( g_screen);
    return select;
}
// //-----------------------------------------------------------------------------
bool initLogin(){
    if( menuLogin.initBackground( "data//background.png", g_screen) == false){
        return false;
    }
     if( menuLogin.addText( "LOGIN", font, g_screen, 400, 100) == false){
        return false;
    }
    if( menuLogin.addText( "USER NAME", font, g_screen, 200, 200) == false){
        return false;
    }
    if( menuLogin.addText( "PASS", font, g_screen, 200, 300) == false){
        return false;
    }
    if( menuLogin.addText( "SIGN IN", font, g_screen, 150, 500) == false){
        return false;
    }
    // text input
    if( menuLogin.addText( "SIGN UP", font, g_screen, 450, 500) == false){
        return false;
    }
    if( menuLogin.addText( "EXIT", font, g_screen, 750, 500) == false){
        return false;
    }
    return true;
}
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
                    select = LOGIN;
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
            case SDL_KEYDOWN:
             if(strlen(room) < 30){
                strcat(room, g_event.text.text);
                if(strlen(room) >= 0 ){
                    menuLogin.addTextInput( userName, font, g_screen, 300, 300);
                        
                }
            }
            break;
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
    SDL_Rect box;
        SDL_SetRenderDrawColor(g_screen, 0, 255, 255, 255);
        SDL_RenderFillRect(g_screen, &roomRect);
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
    if( menuJoin.addText( "NAME", font, g_screen, 150, 100) == false){
        return false;
    }
    if( menuJoin.addText( "BACK", font, g_screen, 700, 500) == false){
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
    if( menuHost.addText( "NAME", font, g_screen, 150, 100) == false){
        return false;
    }
    if( menuHost.addText( "BACK", font, g_screen, 700, 500) == false){
        return false;
    }
    return true;
}

//----------------------------ShowPAUSE--------------------------------------
State showPause(){
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
                if(check_mouse_pos(menuPause.getRect(1))){
                    select = NOCHANGE;
                }else if(check_mouse_pos(menuPause.getRect(3))){
                    select = MENU;
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
bool initPause(){
    if( menuPause.initBackground( "data//background.png", g_screen) == false){
        return false;
    }
    if( menuPause.addText( "PAUSE", font, g_screen, 400, 100) == false){
        return false;
    }
    if( menuPause.addText( "CONTINUE ", font, g_screen, 400, 250) == false){
        return false;
    }
    if( menuPause.addText( "EXIT", font, g_screen, 400, 350) == false){
        return false;
    }
    return true;
}
//------------------------------------------------------------------------
int main( int argc, char *argv[]){

    if(InitData() == false){
        return -1;
    }
    if( initLogin() == false){
        return -1;
    }
    if( initMenu() == false){
        return -1;
    }
    if( initJoin() == false){
        return -1;
    }
    if( initHost() == false){
        return -1;
    }
    if( initPause() == false){
        return -1;
    }


//-----------------------------------------------------------------
    bool is_quit = false;
    State select = LOGIN;
    State select_now = LOGIN;
    State prev_select = LOGIN;
    // 0: exit, 1: menu, 2 host, 3 join
    userName = (char*)calloc(30, sizeof(char));
    password = (char*)calloc(30, sizeof(char));
    while( !is_quit){
        // Load background
        switch(select){
            case EXIT:
                is_quit = true;
                break;
            case LOGIN:
                select_now = select;
                select = showLogin();
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