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
        int current;
        int current_delay;
        bool is_combo;
        bool is_continual;
        bool inverted;
        void updateCollisions();

    public:
        Animation(bool is_combo, bool is_continual);
        Animation(const Animation &a);
        ~Animation();

        int getWidth( int index);
        string getPath( int index);
        int getHeight( int index);
        bool isCombo();
        bool isContinual();
        void addFrame(string file_name, float w, float h, int delay);
        void update(float x, float y, float s, bool flip);
        bool nextFrame(bool &changed);
        void restart();
        void draw(float red, float green, float blue, float alpha);
        void draw(float alpha);
        bool draw( SDL_Renderer* screen, int x_pos, int y_pos, float scale);
};


#endif
