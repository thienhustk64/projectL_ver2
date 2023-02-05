#ifndef _input_h
#define _input_h

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "CommonFunc.h"

enum Keys{ 
    U,
    D,
    B,
    F,
    P,
    K,
    S,
    KEY_MAX
};

class Input{
    private:
        std::map<Keys,SDL_Scancode> keys;
        std::map<int,std::string> trigger;
        bool keyPress [KEY_MAX];
        bool quit_key;

    public:
        Input();
        ~Input();
        bool getEvent( SDL_Event *windowEvent);
        std::string getTrigger();
};

#endif
