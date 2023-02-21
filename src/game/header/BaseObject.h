#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "CommonFunc.h"

class BaseObject{
    public:
        BaseObject();
        ~BaseObject();
        void SetRect( const int& x, const int& y){
            rect_.x = x;
            rect_.y = y;
        }
        SDL_Rect getRect();
        SDL_Texture* GetObject() const {
            return p_object_;
        }
        
        bool LoadImg( std::string path, SDL_Renderer* screen);
<<<<<<< HEAD
        bool LoadText( std::string text, SDL_Renderer* screen);
=======
        bool LoadText( std::string text, TTF_Font *font, SDL_Renderer* screen, int x, int y);
>>>>>>> 52730ced2ba8a27a0576dff82acdcab7b458e66d
        void Render( SDL_Renderer* des, const SDL_Rect* clip=NULL);
        void RenderMap( SDL_Renderer* des, const SDL_Rect* clip=NULL);
        void Free();

    protected:
        SDL_Texture* p_object_;
        SDL_Rect rect_;

    
};

#endif