#ifndef COLLISION_H
#define COLLISION_H

#include <iostream>
#include<algorithm>
#include "CommonFunc.h"

using namespace std;

class Collision{
    private:
        int x_pos;
        int y_pos;
        int height;
        int width;
    public:
        Collision();
        ~Collision();

        void initCollision( int x_pos, int y_pos, int width, int height);

        int getXmin();
        int getXmax();
        int getYmin();
        int getYmax();
        int getArea();
        bool checkCollision( Collision temp);
};
#endif
