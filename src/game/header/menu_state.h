#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"

using namespace std;

class Menu{
    private:
        BaseObject *g_background;
        vector<BaseObject*> text_objects;
        BaseObject *username;
        BaseObject *password;
    public:
        Menu();
        ~Menu();
        bool initBackground( string path, SDL_Renderer *g_screen);
        bool addText( string text, TTF_Font* font, SDL_Renderer *g_screen, int x, int y);
        bool addUsername( string text, TTF_Font* font, SDL_Renderer *g_screen, int x, int y);
        bool addPassword( string text, TTF_Font* font, SDL_Renderer *g_screen, int x, int y);
        SDL_Rect getRect( int index);
         
        void drawBackground( SDL_Renderer *g_screen);
        void drawButton( SDL_Renderer *g_screen);
        void drawUsername( SDL_Renderer *g_screen);
        void drawPassword( SDL_Renderer *g_screen);
        void Free();
};




#endif //MENU_H