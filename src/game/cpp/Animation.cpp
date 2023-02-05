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

void Animation::addFrame( string file_name, float w, float h, int delay){
    paths.push_back( file_name);
    widths.push_back( w);
    heights.push_back( h);
    delays.push_back( delay);
}

void Animation::setXPos( int x_pos){
    this->x_pos = x_pos;
}

int Animation::getHeight( int index){
    return heights[index];
}

int Animation::getWidth( int index){
    return widths[index];
}

int Animation::getXFinish(){
    return x_finish;
}

int Animation::getYFinish(){
    return y_finish;
}

string Animation::getPath( int index){
    return paths[index];
}

bool Animation::draw( SDL_Renderer* screen, int x_pos_character, int y_pos_character, float scale){
    SDL_Surface* load_surface = IMG_Load( paths[current].c_str());
    SDL_Texture* new_texture = SDL_CreateTextureFromSurface( screen, load_surface);
    int width = int( widths[current]*scale);
    int height = int( heights[current]*scale);
    x_finish = x_pos_character + x_pos + x_initial*current;
    y_finish = y_pos_character + y_pos;
    SDL_Rect renderquad = { x_pos_character + x_pos + x_initial*current, y_pos_character + y_pos, width, height};
    SDL_RenderCopy( screen, new_texture, NULL, &renderquad);
    // current_delay++;
    // if( current_delay >= delays[current]){
    //     current_delay = 0;
    //     current++;
    // }
    current++;
    if( current >= int( paths.size())){
        // printf("%s\t%d\n",paths[current-1].c_str(), current);
        current = 0;
        return true;
    }
    return false;
}