#include "BaseObject.h"

BaseObject::BaseObject(){
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

BaseObject::~BaseObject(){
    Free();
}

bool BaseObject::LoadImg( std::string path, SDL_Renderer* screen){
    SDL_Texture* new_texture = NULL;
    SDL_Surface* load_surface = IMG_Load( path.c_str());
    if( load_surface != NULL){
        SDL_SetColorKey( load_surface, SDL_TRUE, SDL_MapRGB( load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface( screen, load_surface);
        if( new_texture != NULL){
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
        }

        SDL_FreeSurface( load_surface);
    }
    p_object_ = new_texture;
    return p_object_ != NULL;
}

bool BaseObject::LoadText( std::string text, TTF_Font *font, SDL_Renderer* screen, int x, int y){
    SDL_Color fg = { 255, 255, 255};
    SDL_Texture *texture = NULL;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), fg);
    if(surface !=NULL){
        texture = SDL_CreateTextureFromSurface(screen, surface);
        if( texture != NULL){
            rect_.x = x;
            rect_.y = y;
            rect_.w = surface->w;
            rect_.h = surface->h;
        }
        SDL_FreeSurface( surface);
    }
    p_object_ = texture;
    return p_object_ != NULL;
}

void BaseObject::Render( SDL_Renderer* des, const SDL_Rect* clip){
    SDL_Rect renderquad = { rect_.x, rect_.y, rect_.w, rect_.h};
    SDL_RenderCopy( des, p_object_, clip, &renderquad);
}

void BaseObject::RenderMap( SDL_Renderer* des, const SDL_Rect* clip){
    SDL_Rect renderquad = { rect_.x, rect_.y, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy( des, p_object_, clip, &renderquad);
}

void BaseObject::Free(){
    if( p_object_ != NULL){
        SDL_DestroyTexture( p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}
SDL_Rect BaseObject::getRect(){
    return rect_;
}
