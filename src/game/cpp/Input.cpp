#include "Input.h"

Input::Input(){
    keys[U] = SDL_SCANCODE_W;
    keys[D] = SDL_SCANCODE_S;
    keys[B] = SDL_SCANCODE_A;
    keys[F] = SDL_SCANCODE_D;
    keys[P] = SDL_SCANCODE_J;
    keys[K] = SDL_SCANCODE_K;
    keys[S] = SDL_SCANCODE_U;

    trigger[0] = "U";
    trigger[1] = "D";
    trigger[2] = "B";
    trigger[3] = "F";
    trigger[4] = "P";
    trigger[5] = "K";
    trigger[6] = "S";

    for( int i = 0; i < KEY_MAX; i++){
        keyPress[i] = false;
    }
}

Input::~Input(){

}

bool Input::getEvent( SDL_Event *windowEvent, bool inverted){
    bool done = false;
    while( SDL_PollEvent( windowEvent)){
        switch( windowEvent->type){
            case SDL_WINDOWEVENT_CLOSE:
                done = true;
                break;
            case SDL_KEYDOWN:{
                switch ( windowEvent->key.keysym.sym){
                    case SDLK_ESCAPE:
                        done = true;
                        break;
                    default:
                        break;
                }
            }
            break;
            case SDL_QUIT:
                done = true;
                break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    if( state[keys[U]]){
        keyPress[U] = true;
    }

    if( state[keys[D]]){
        keyPress[D] = true;
    }

    if( state[keys[B]]){
        if( inverted){
            keyPress[F] = true;
        }else{
            keyPress[B] = true;
        }
    }
    
    if( state[keys[F]]){
        if( inverted){
            keyPress[B] = true;
        }else{
            keyPress[F] = true;
        }
    }

    if( state[keys[P]]){
        keyPress[P] = true;
    }

    if( state[keys[K]]){
        keyPress[K] = true;
    }

    if( state[keys[S]]){
        keyPress[S] = true;
    }

    return done;
}

std::string Input::getTrigger(){
    std::string rs = "";
    for( int i = 0; i < KEY_MAX; i++){
        if( keyPress[i]){
            rs.append(trigger[i]);
        }
    }
    if( rs.compare("") == 0){
        rs = "NONE";
    }
    for( int i = 0; i < KEY_MAX; i++){
        keyPress[i] = false;
    }

    return rs;
}