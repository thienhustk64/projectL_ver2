#include "Animation.h"
#include "Player.h"

using namespace std;

Animation::Animation(bool is_combo, bool is_continual){
    this->is_combo     = is_combo;
    this->is_continual = is_continual;
    current       = 0;
    current_delay = 0;
    x_pos = 0;
    y_pos = 0;
    x_initial = 0;
}

Animation::Animation(bool is_combo, bool is_continual, int x_pos, int y_pos, int x_initial){
    this->is_combo     = is_combo;
    this->is_continual = is_continual;
    current       = 0;
    current_delay = 0;
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    this->x_initial = x_initial;
}

Animation::Animation(const Animation &a){
    current       = a.current;
    current_delay = a.current_delay;
    is_combo      = a.is_combo;
    is_continual  = a.is_continual;
    inverted      = a.inverted;
    x_pos = a.x_pos;

    for(unsigned int i = 0; i < a.paths.size(); i++)    {
        paths.push_back( a.paths[i]);
        widths.push_back( a.widths[i]);
        heights.push_back( a.heights[i]);
        delays.push_back( a.delays[i]);
    }
}

Animation::~Animation(){
    // for(int i = 0; i < sprites.size(); i++){
    //     delete sprites[i];
    //     delete offense[i];
    //     delete defense[i];
    //     delete action[i];
    // }
}

void Animation::addFrame( string file_name, float w, float h, int delay, int y_initial){
    paths.push_back( file_name);
    widths.push_back( w);
    heights.push_back( h);
    delays.push_back( delay);
    y_initials.push_back( y_initial);
}

void Animation::setXPos( int x_pos){
    this->x_pos = x_pos;
}

int Animation::getHeight(){
    return heights[current];
}

int Animation::getWidth(){
    return widths[current];
}

int Animation::getXFinish(){
    return x_finish;
}

int Animation::getYFinish(){
    return y_finish;
}

int Animation::getXPos(){
    return x_pos;
}

int Animation::getYInitial(){
    return y_initials[current];
}

string Animation::getPath(){
    return paths[current];
}

bool Animation::draw( SDL_Renderer* screen, int x_pos_character, int y_pos_character, float scale, bool inverted){
    SDL_Surface* load_surface = IMG_Load( paths[current].c_str());
    SDL_Texture* new_texture = SDL_CreateTextureFromSurface( screen, load_surface);
    SDL_FreeSurface( load_surface);
    int width = int( widths[current]*scale);
    int height = int( heights[current]*scale);
    

    
    // SDL_RenderCopy( screen, new_texture, NULL, &renderquad);
    if( inverted){
        x_finish = x_pos_character - int(x_pos*scale) + x_initial*current*-1;
        y_finish = y_pos_character + int(y_pos*scale);
        SDL_Rect renderquad = { x_finish-width, y_finish, width, height};
        SDL_RenderCopyEx( screen, new_texture, NULL, &renderquad, 0, NULL, SDL_FLIP_HORIZONTAL);
    }else{
        x_finish = x_pos_character + int(x_pos*scale) + x_initial*current;
        y_finish = y_pos_character + int(y_pos*scale);
        SDL_Rect renderquad = { x_finish, y_finish, width, height};
        SDL_RenderCopyEx( screen, new_texture, NULL, &renderquad, 0, NULL, SDL_FLIP_NONE);
    }
    SDL_DestroyTexture( new_texture);

    current++;
    if( current >= int( paths.size())){
        current = 0;
        return true;
    }
    return false;
}