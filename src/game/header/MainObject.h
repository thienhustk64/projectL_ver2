#ifndef MAIN_OBJECT
#define MAIN_OBJECT

#include "CommonFunc.h"
#include "BaseObject.h"

class MainObject: public BaseObject{
    public:
        MainObject();
        ~MainObject();

        bool LoadImg( std::string path, SDL_Renderer *screen);
        void Show( SDL_Renderer *screen);
        void HandelInputAction( SDL_Event *events, SDL_Renderer *screen);
    
    private:
        float x_val_;
        float y_val_;

        float x_pos_;
        float y_pos_;

        int width_frame_;
        int height_frame_;

        SDL_Rect frame_clip
};

#endif