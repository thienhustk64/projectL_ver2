#include "CommonFunc.h"
#include "menu_state.h"
#include <windows.h>

enum State {
    MENU,
    HOST,
    JOIN,
    EXIT,
    BACK,
    NOCHANGE,
    INGAME,
    LOGIN,
    SIGNIN,
    SIGNUP,
    CHECKLOGIN,
    PAUSE
};

bool check_mouse_pos(SDL_Rect rect);
bool initMenu( Menu* menuState, TTF_Font* font, SDL_Renderer *g_screen);
bool initJoin( Menu* menuJoin, TTF_Font* font, SDL_Renderer *g_screen);
bool initHost( Menu* menuHost, TTF_Font* font, SDL_Renderer *g_screen);
bool initLogin( Menu* menuLogin, TTF_Font* font, SDL_Renderer *g_screen);
bool initPause(  Menu* menuPause, TTF_Font* font, SDL_Renderer *g_screen);
State showMenu( Menu *menuState, SDL_Renderer *g_screen, SDL_Event g_event);
State showJoin( Menu *menuJoin, SDL_Renderer *g_screen, SDL_Event g_event);
State showHost( Menu *menuHost, SDL_Renderer *g_screen, SDL_Event g_event);
State showLogin( Menu *menuLogin, SDL_Renderer *g_screen, SDL_Event g_event, TTF_Font* font, char* userName, char* password);
State showPause( Menu *menuPause, SDL_Renderer *g_screen, SDL_Event g_event);
