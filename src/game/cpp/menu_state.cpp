#include "menu_state.h"

Menu::Menu(){
    g_background = new BaseObject();
    username = new BaseObject();
    password = new BaseObject();
}
Menu::~Menu(){
    g_background->Free();
    username->Free();
    password->Free();
    for( unsigned int i = 0; i < text_objects.size(); i++ ){
        text_objects[i]->Free();
    }
}

bool Menu::initBackground( string path, SDL_Renderer *g_screen){
    bool ret = g_background->LoadImg( path, g_screen);
    return ret;
}

bool Menu::addText( string text, TTF_Font* font,SDL_Renderer* g_screen , int x, int y){
    BaseObject *g_text = new BaseObject();
    bool ret = g_text->LoadText( text, font, g_screen, x, y);
    if( ret){
        text_objects.push_back( g_text );
    }
    return ret;
}

void Menu::drawBackground( SDL_Renderer *g_screen){
    g_background->RenderMap( g_screen, NULL);
}

void Menu::drawButton( SDL_Renderer *g_screen){
    for( unsigned int i = 0; i < text_objects.size(); i++){
        text_objects[i]->Render( g_screen, NULL);
    }
}

SDL_Rect Menu::getRect( int index){
    if( index < 0 || index >= int(text_objects.size())){
        return {0,0,0,0};
    }
    return text_objects[index]->getRect();
}

bool Menu::addUsername(string text, TTF_Font* font,SDL_Renderer* g_screen , int x, int y){
    // text_input->Free();
    bool ret = username->LoadText( text, font, g_screen, x, y);
    return ret;
}
bool Menu::addPassword(string text, TTF_Font* font,SDL_Renderer* g_screen , int x, int y){
    // text_input->Free();
    bool ret = password->LoadText( text, font, g_screen, x, y);
    return ret;
}

void Menu::drawUsername( SDL_Renderer *g_screen){
    username->Render(g_screen, NULL);
}
void Menu::drawPassword( SDL_Renderer *g_screen){
    password->Render(g_screen, NULL);
}

void Menu::Free(){
    g_background->Free();
    username->Free();
    password->Free();
    for( unsigned int i = 0; i < text_objects.size(); i++ ){
        text_objects[i]->Free();
    }
}