#include "Animation.h"
#include "Player.h"

using namespace std;

Animation::Animation(bool is_combo, bool is_continual){
    this->is_combo     = is_combo;
    this->is_continual = is_continual;
    current       = 0;
    current_delay = 0;
}

Animation::Animation(const Animation &a){
    current       = a.current;
    current_delay = a.current_delay;
    is_combo      = a.is_combo;
    is_continual  = a.is_continual;
    inverted      = a.inverted;
    x_pos         = x_pos;
    y_pos         = y_pos;
    scale         = scale;

    // for(int i = 0; i < a.sprites.size(); i++)    {
    //     sprites.push_back(new Sprite(*a.sprites[i]));
    //     offense.push_back(new Collision(*a.offense[i]));
    //     defense.push_back(new Collision(*a.defense[i]));
    //     action.push_back(new Actions(*a.action[i]));
    // }

    delays = a.delays;
}

Animation::~Animation(){
    // for(int i = 0; i < sprites.size(); i++){
    //     delete sprites[i];
    //     delete offense[i];
    //     delete defense[i];
    //     delete action[i];
    // }
}




