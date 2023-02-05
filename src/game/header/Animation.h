#ifndef _animation_h
#define _animation_h

#include <iostream>
#include <string>
#include <vector>
#include "CommonFunc.h"

using namespace std;

class Animation
{
    private:
        vector<string> paths;
        vector<int> widths;
        vector<int> heights;
        // vector<Collision*> offense;
        // vector<Collision*> defense;
        string name;
        vector<int> delays;
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
        void updateCollisions();

    public:
        Animation(bool is_combo, bool is_continual);
        Animation(bool is_combo, bool is_continual, int x_pos, int y_pos, int x_initial);
        Animation(const Animation &a);
        ~Animation();

        int getWidth( int index);
        int getXFinish();
        int getYFinish();
        string getPath( int index);
        int getHeight( int index);
        bool isCombo();
        bool isContinual();
        void addFrame(string file_name, float w, float h, int delay);
        void setXPos( int x_pos);
        void update(float x, float y, float s, bool flip);
        bool draw( SDL_Renderer* screen, int x_pos_character, int y_pos_character, float scale);
};


#endif
