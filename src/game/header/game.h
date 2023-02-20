#ifndef GAME_H
#define GAME_H

#include "lib.h"
#include "menu_state.h"
using namespace std;
class game
{
private:
    stack<State*>* states;
    SDL_Window* window;

    SDL_Renderer* renderer = NULL;

    const int SCREEN_WIDTH = 1040;
    const int SCREEN_HEIGHT = 770;

    void initWindow();
    void initStates();
public:
    game();
    ~game();

    //Update
    void update();
    //Render
    void render();
    void renderWindow();
};



#endif