#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"

using namespace std;

#define HOST_GAME 1
#define JOIN_GAME 2
#define EXIT_GAME 3
#define ESC -1

#define ERROR 0
#define SUCCEED 1
#define INSIDE 1
#define OUTSIDE 0

class Menu{
    private:
        BaseObject *g_background;
        vector<BaseObject*> text_objects;
        BaseObject *text_input;
    public:
        Menu();
        ~Menu();
        bool initBackground( string path, SDL_Renderer *g_screen);
        bool addText( string text, TTF_Font* font, SDL_Renderer *g_screen, int x, int y);
        bool addTextInput( string text, TTF_Font* font, SDL_Renderer *g_screen, int x, int y);
        bool addTextInputx( string text, TTF_Font* font, SDL_Renderer *g_screen, int x, int y);
        SDL_Rect getRect( int index);
         
        void drawBackground( SDL_Renderer *g_screen);
        void drawButton( SDL_Renderer *g_screen);
        void drawTextInput( SDL_Renderer *g_screen);
        void drawTextInputx( SDL_Renderer *g_screen);
};




#endif //MENU_H