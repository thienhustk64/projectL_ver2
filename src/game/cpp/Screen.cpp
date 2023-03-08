#include "Screen.h"
bool check_mouse_pos(SDL_Rect rect){
    int x, y;
    SDL_GetMouseState(&x, &y);
    bool value = true;
    if(x < rect.x){
        value = false;
    }
    else if(x > rect.x + rect.w){
        value = false;
    }
    else if(y < rect.y){
        value = false;
    }
    else if(y > rect.y + rect.h){
        value = false;
    }
    return value;
}

State showMenu( Menu *menuState, SDL_Renderer *g_screen, SDL_Event g_event){
    State select = NOCHANGE;
    while( select == NOCHANGE){
        while( SDL_PollEvent( &g_event)){
            switch( g_event.type){
                case SDL_WINDOWEVENT_CLOSE:
                    select = EXIT;
                    break;
                case SDL_MOUSEBUTTONDOWN:{
                    if(check_mouse_pos(menuState->getRect(1))){
                        select = HOST;
                    }else if(check_mouse_pos(menuState->getRect(2))){
                        select = JOIN;
                    }else if(check_mouse_pos(menuState->getRect(3))){
                        select = EXIT;
                    }
                    break;
                }
                case SDL_QUIT:
                    select = EXIT;
                    break;
            }
        }
        SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear( g_screen);
        //menuState->drawBackground
        menuState->drawBackground( g_screen );
        if( check_mouse_pos( menuState->getRect(1))){
            SDL_Rect box = menuState->getRect(1);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_RenderFillRect(g_screen, &box);
        }else if(check_mouse_pos( menuState->getRect(2))){
            SDL_Rect box = menuState->getRect(2);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_RenderFillRect(g_screen, &box);
        }else if(check_mouse_pos( menuState->getRect(3))){
            SDL_Rect box = menuState->getRect(3);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_RenderFillRect(g_screen, &box);
        }
        menuState->drawButton( g_screen );
        SDL_RenderPresent( g_screen);
    }
    return select;
}

bool initMenu( Menu *menuState, TTF_Font* font, SDL_Renderer *g_screen){
    if( menuState->initBackground( "data//background.png", g_screen) == false){
        return false;
    }
    if( menuState->addText( "GAME 2v2", font, g_screen, 400, 100) == false){
        return false;
    }
    if( menuState->addText( "HOST GAME", font, g_screen, 400, 300) == false){
        return false;
    }
    if( menuState->addText( "JOIN GAME", font, g_screen, 400, 200) == false){
        return false;
    }
    if( menuState->addText( "EXIT", font, g_screen, 450, 400) == false){
        return false;
    }
    return true;
}

State showJoin( Menu *menuJoin, SDL_Renderer *g_screen, SDL_Event g_event){
    State select = NOCHANGE;
    while( select == NOCHANGE){
        while( SDL_PollEvent( &g_event)){
            switch( g_event.type){
                case SDL_WINDOWEVENT_CLOSE:
                    select = EXIT;
                    break;
                case SDL_MOUSEBUTTONDOWN:{
                    if(check_mouse_pos(menuJoin->getRect(1))){
                        select = INGAME;
                    }else if(check_mouse_pos(menuJoin->getRect(3))){
                        select = BACK;
                    }
                    break;
                }
                case SDL_QUIT:
                    select = EXIT;
                    break;
            }
        }
        SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear( g_screen);
        //menuJoin->drawBackground
        menuJoin->drawBackground( g_screen );
        if( check_mouse_pos( menuJoin->getRect(1))){
            SDL_Rect box = menuJoin->getRect(1);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_RenderFillRect(g_screen, &box);
        }else if(check_mouse_pos( menuJoin->getRect(3))){
            SDL_Rect box = menuJoin->getRect(3);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_RenderFillRect(g_screen, &box);
        }
        menuJoin->drawButton( g_screen );
        SDL_RenderPresent( g_screen);
    }
    return select;
}

bool initJoin( Menu *menuJoin, TTF_Font* font, SDL_Renderer *g_screen){
    if( menuJoin->initBackground( "data//background.png", g_screen) == false){
        return false;
    }
    if( menuJoin->addText( "MENU JOIN", font, g_screen, 50, 25) == false){
        return false;
    }
    if( menuJoin->addText( "JOIN ", font, g_screen, 150, 500) == false){
        return false;
    }
    // text input
    if( menuJoin->addText( "Name", font, g_screen, 150, 100) == false){
        return false;
    }
    if( menuJoin->addText( "Back", font, g_screen, 700, 500) == false){
        return false;
    }
    return true;
}

State showHost( Menu *menuHost, SDL_Renderer *g_screen, SDL_Event g_event){
    State select = NOCHANGE;
    while( select == NOCHANGE){
        while( SDL_PollEvent( &g_event)){
            switch( g_event.type){
                case SDL_WINDOWEVENT_CLOSE:
                    select = EXIT;
                    break;
                case SDL_MOUSEBUTTONDOWN:{
                    if(check_mouse_pos(menuHost->getRect(1))){
                        select = INGAME;
                    }else if(check_mouse_pos(menuHost->getRect(3))){
                        select = BACK;
                    }
                    break;
                }
                case SDL_QUIT:
                    select = EXIT;
                    break;
            }
        }
        SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear( g_screen);
        //menuHost->drawBackground
        menuHost->drawBackground( g_screen );
        if( check_mouse_pos( menuHost->getRect(1))){
            SDL_Rect box = menuHost->getRect(1);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_RenderFillRect(g_screen, &box);
        }else if(check_mouse_pos( menuHost->getRect(3))){
            SDL_Rect box = menuHost->getRect(3);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_RenderFillRect(g_screen, &box);
        }
        menuHost->drawButton( g_screen );
        SDL_RenderPresent( g_screen);
    }
    return select;
}

bool initHost( Menu *menuHost, TTF_Font* font, SDL_Renderer *g_screen){
    if( menuHost->initBackground( "data//background.png", g_screen) == false){
        return false;
    }
    if( menuHost->addText( "MENU HOST", font, g_screen, 50, 25) == false){
        return false;
    }
    if( menuHost->addText( "CREATE ", font, g_screen, 150, 500) == false){
        return false;
    }
    if( menuHost->addText( "Name", font, g_screen, 150, 100) == false){
        return false;
    }
    if( menuHost->addText( "Back", font, g_screen, 700, 500) == false){
        return false;
    }
    return true;
}

State showLogin( Menu *menuLogin, SDL_Renderer *g_screen, SDL_Event g_event, TTF_Font* font, char* userName, char* password){
    State select = NOCHANGE;
    SDL_Rect userNameRect = {500, 200, 300, 30};
    SDL_Rect passwordRect = {500, 300, 300, 30};
    int tr = 0;
    while( select == NOCHANGE){
        while( select == NOCHANGE){
            while( SDL_PollEvent( &g_event)){
                switch( g_event.type){
                    case SDL_TEXTINPUT:
                            if(strlen(userName) < 15){
                                if(tr == 0){
                                    strcat(userName, g_event.text.text);
                                }
                                if(strlen(userName) >= 0 ){
                                    menuLogin->addUsername( userName, font, g_screen, 500, 200);
                                    menuLogin->addPassword( password, font, g_screen, 500, 300);
                                }
                            }
                            if(strlen(password) < 15){
                                if(tr == 1){
                                    strcat(password, g_event.text.text);
                                }
                                if(strlen(password) >= 0 ){
                                    menuLogin->addUsername( userName, font, g_screen, 500, 200);
                                    menuLogin->addPassword( password, font, g_screen, 500, 300);
                                }
                            }
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        select = EXIT;
                        break;
                    case SDL_KEYDOWN:
                        switch ( g_event.key.keysym.sym){
                            case SDLK_ESCAPE:
                                select = EXIT;
                                break;
                            case SDLK_BACKSPACE:
                                    if(tr == 0){
                                        userName[strlen(userName) - 1] = '\0';
                                    }
                                    if(strlen(userName) >= 0 ){
                                        menuLogin->addUsername( userName, font, g_screen, 500, 200);
                                        menuLogin->addPassword( password, font, g_screen, 500, 300);
                                    }
                                    if(tr == 1){
                                        password[strlen(password) - 1] = '\0';
                                    }
                                    if(strlen(password) >= 0 ){
                                        menuLogin->addUsername( userName, font, g_screen, 500, 200);
                                        menuLogin->addPassword( password, font, g_screen, 500, 300);
                                    }
                                break;
                            default:
                                break;
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if(check_mouse_pos(menuLogin->getRect(3))){
                            select = SIGNIN;
                        }else if(check_mouse_pos(menuLogin->getRect(4))){
                            select = SIGNUP;
                        } else if(check_mouse_pos(menuLogin->getRect(5))){
                            select = EXIT;
                        }else if(check_mouse_pos(userNameRect)){
                            tr = 0;
                        }else if(check_mouse_pos(passwordRect)){
                            tr = 1;
                        }
                        break;
                    case SDL_QUIT:
                        select = EXIT;
                        break;
                }
            }
            SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            SDL_RenderClear( g_screen);
            
            //menuLogin->drawBackground
            menuLogin->drawBackground( g_screen );
            SDL_Rect box;
            SDL_SetRenderDrawColor(g_screen, 0, 255, 255, 255);
            SDL_RenderFillRect(g_screen, &userNameRect);
            SDL_RenderFillRect(g_screen, &passwordRect);
            SDL_SetRenderDrawColor(g_screen, 0, 255, 0, 255);
            if( tr == 0){
                SDL_RenderFillRect(g_screen, &userNameRect);
            }else if( tr == 1){
                SDL_RenderFillRect(g_screen, &passwordRect);
            }
            if( check_mouse_pos( menuLogin->getRect(3))){
                box = menuLogin->getRect(3);
                SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
                SDL_RenderFillRect(g_screen, &box);
            }else if(check_mouse_pos( menuLogin->getRect(4))){
                box = menuLogin->getRect(4);
                SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
                SDL_RenderFillRect(g_screen, &box);
            }else if(check_mouse_pos( menuLogin->getRect(5))){
                box = menuLogin->getRect(5);
                SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
                SDL_RenderFillRect(g_screen, &box);
            }
            menuLogin->drawUsername( g_screen);
            menuLogin->drawPassword( g_screen);
            menuLogin->drawButton( g_screen );
            SDL_RenderPresent( g_screen);
        }
    }
    return select;
}

bool initLogin( Menu* menuLogin, TTF_Font* font, SDL_Renderer *g_screen){
    if( menuLogin->initBackground( "data//background.png", g_screen) == false){
        return false;
    }
    if( menuLogin->addText( "LOGIN", font, g_screen, 400, 100) == false){
        return false;
    }
    if( menuLogin->addText( "USER NAME", font, g_screen, 200, 200) == false){
        return false;
    }
    if( menuLogin->addText( "PASS", font, g_screen, 200, 300) == false){
        return false;
    }
    if( menuLogin->addText( "SIGN IN", font, g_screen, 150, 500) == false){
        return false;
    }
    // text input
    if( menuLogin->addText( "SIGN UP", font, g_screen, 450, 500) == false){
        return false;
    }
    if( menuLogin->addText( "EXIT", font, g_screen, 750, 500) == false){
        return false;
    }
    return true;
}

State showPause( Menu *menuPause, SDL_Renderer *g_screen, SDL_Event g_event){
        State select = NOCHANGE;
    while( select == NOCHANGE){
        while( SDL_PollEvent( &g_event)){
            switch( g_event.type){
                case SDL_WINDOWEVENT_CLOSE:
                    select = EXIT;
                    break;
                case SDL_MOUSEBUTTONDOWN:{
                    if(check_mouse_pos(menuPause->getRect(1))){
                        select = INGAME;
                    }else if(check_mouse_pos(menuPause->getRect(2))){
                        select = MENU;
                    }else if(check_mouse_pos(menuPause->getRect(3))){
                        select = EXIT;
                    }
                    break;
                }
                case SDL_QUIT:
                    select = EXIT;
                    break;
            }
        }
        SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear( g_screen);
        //menuState->drawBackground
        menuPause->drawBackground( g_screen );
        if( check_mouse_pos( menuPause->getRect(1))){
            SDL_Rect box = menuPause->getRect(1);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_RenderFillRect(g_screen, &box);
        }else if(check_mouse_pos( menuPause->getRect(2))){
            SDL_Rect box = menuPause->getRect(2);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_RenderFillRect(g_screen, &box);
        }else if(check_mouse_pos( menuPause->getRect(3))){
            SDL_Rect box = menuPause->getRect(3);
            SDL_SetRenderDrawColor(g_screen, 255, 0, 0, 255);
            SDL_RenderFillRect(g_screen, &box);
        }
        menuPause->drawButton( g_screen );
        SDL_RenderPresent( g_screen);
    }
    return select;
}

bool initPause(  Menu* menuLogin, TTF_Font* font, SDL_Renderer *g_screen){
    if( menuLogin->initBackground( "data//background.png", g_screen) == false){
        return false;
    }
    if( menuLogin->addText( "GAME 2v2", font, g_screen, 390, 100) == false){
        return false;
    }
    if( menuLogin->addText( "RESUME", font, g_screen, 400, 200) == false){
        return false;
    }
    if( menuLogin->addText( "BACK TO MENU", font, g_screen, 360, 300) == false){
        return false;
    }
    if( menuLogin->addText( "EXIT", font, g_screen, 430, 400) == false){
        return false;
    }
    return true;
}