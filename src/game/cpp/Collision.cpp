#include "Collision.h"

using namespace std;

Collision::Collision(){}

Collision::~Collision(){}

void Collision::initCollision( int x_pos, int y_pos, int width, int height){
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    this->height = height;
    this->width = width;
}

int Collision::getXmin(){
    return x_pos;
}

int Collision::getYmin(){
    return y_pos;
}

int Collision::getXmax(){
    return x_pos + width;
}

int Collision::getYmax(){
    return y_pos + height;
}

int Collision::getArea(){
    return width*height;
}

bool Collision::checkCollision( Collision temp){
    int xA = max(x_pos, temp.getXmin());
    int yA = max(y_pos, temp.getYmin());
    int xB = min(x_pos + width, temp.getXmax());
    int yB = min(y_pos + height, temp.getYmax());

    int interArea = max( 0, xB - xA) * max( 0, yB - yA);
    // printf("%d %d %d %d = %d\n", xA, yA, xB, yB, interArea);
    if( interArea > 0){
        return true;
    }
    return false;
}