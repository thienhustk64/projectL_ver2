#ifndef _animation_h
#define _animation_h

#include <iostream>
#include <string>
#include <vector>
#include "CommonFunc.h"
#include "Collision.h"

using namespace std;

class Animation
{
    private:
        vector<string> paths;
        vector<int> widths;
        vector<int> heights;
        string name;
        vector<int> delays;
        vector<int> y_initials;
        vector<int> x_poss;
        int x_pos;
        int y_pos;
        int x_initial;
        int x_finish;
        int y_finish;
        int current;
        int current_delay;
        bool is_combo;
        bool is_continual;
        bool inverted;
        string trigger;
        Collision defense;

    public:
        Animation(bool is_combo, bool is_continual);
        Animation(bool is_combo, bool is_continual, int x_pos, int y_pos, int x_initial);
        Animation(const Animation &a);
        ~Animation();

        void setTrigger( string trigger);
        void reset();
        string getTrigger();
        int getWidth();
        int getXFinish();
        int getYFinish();
        int getXPos();
        int getYInitial();
        Collision getCollision();
        string getPath();
        int getHeight();
        void setXPos( int x_pos);
        void addFrame(string file_name, float w, float h, int delay, int y_initial, int x_pos);
        bool draw( SDL_Renderer* screen, string name, TTF_Font* font, int x_pos_character, int y_pos_character, float scale, bool inverted);
        bool checkCollision( Collision temp);
};


#endif
