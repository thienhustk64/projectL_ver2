#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>
#include <string>
#include <typeinfo>
#include <sstream>
#include <iostream>
#include <map>
#include "tinyxml2.h"
#include "Animation.h"
#include "CommonFunc.h"
#include "ultis.h"

class Player{
    private:
        int x_pos;
        int y_pos;
        float alpha;
        float scale;
        int x_pos_skill;
        int y_pos_skill;
        int x_pos_skill_finish;
        int y_pos_skill_finish;

        float y_initial;
        float x_initial;
        float alpha_initial;
        float scale_initial;

        std::string type;
        bool inverted;
        bool is_hurt;
        bool is_skill;
        bool is_skill_finish;
        int index;
        int current;

        std::string current_state;
        std::string previous_state;

        std::map<std::string, std::map<std::string, std::string> > states;
        std::map<std::string, std::string> next_state;
        std::map<std::string, Animation*> animations;

        Player *opponent;
        
        void setCurrentState(std::string state);
        void checkCollisions(float action_x_vel);

    public:
        Player();
        Player(const Player &p);
        ~Player();

        bool operator< (const Player& p) const;
        float getXpos();
        void moveXpos(float value);
        bool isInverted();
        bool isHurt();

        std::map<std::string, bool> state;
        bool initialize(std::string character, std::string skill, std::string skill_finish, bool player_one, float x_pos, float y_pos);
        void updateState( std::string trigger);
        void draw( SDL_Renderer* screen);
    };

#endif