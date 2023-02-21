#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "Player.h"
#include "Input.h"
#include "Collision.h"
#include "Screen.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdint.h>
#include<pthread.h>
#include<windef.h>
#include <time.h>
#include "packet.h"
#include "define.h"
//local variable

BaseObject g_background;
static SDL_Window *g_window = NULL;
static SDL_Renderer *g_screen = NULL;
static SDL_Event g_event;
TTF_Font* font = NULL;
Uint32 start;

struct sockaddr_in server_addr;
pthread_t tid;
SOCKET sockfd;
WSADATA wsa;
char **token ;
char *roomName;
CurrentPlayer *currUser;

Menu *menuState = new Menu();
Menu *menuJoin = new Menu();
Menu *menuHost = new Menu();
Menu *menuLogin = new Menu();

char* userName;
char* password;

typedef struct account{
	int id;
	char *username;
	char *password;
}account;

typedef struct _Arg{
	CurrentPlayer *currUser;
    enum pack_type type;
    char *buffer;
}Arg;

typedef struct _Row{
    char *roomName;
    char *playerCount;
    char *maxPlayer;
}Row;

int roomcount;
Row *roomlist;

void GetCurrUser(){
    currUser = (CurrentPlayer*)calloc(1, sizeof(CurrentPlayer));
    currUser->name = (char*)calloc(MAX_NAME, sizeof(char));
    currUser->room = (char*)calloc(MAX_RNAME, sizeof(char));
    currUser->id = -1;
    currUser->isHost = -1;
}

int setupClient(){
	//Initialise winsock
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
		printf("Failed. Error Code : %d\n", WSAGetLastError());
		return -1;
	}
	//Create socket
	if(int(sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR){
		printf("Socket creation failed : %d\n" , WSAGetLastError());
		return -1;
	}
    //Setup address structure
    memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5500);
	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    return 1;
}

void closeClient(){
    closesocket(sockfd);
	WSACleanup();
}

void sendToServer(SOCKET sockfd, struct sockaddr_in server_addr, char *buffer){
    int length = (int)sizeof(server_addr);
    sendto(sockfd, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, length);
}

void ListenToServer(SOCKET sockfd, struct sockaddr_in server_addr, char *buffer){
    int buffer_size, length = sizeof(server_addr);
    buffer_size = recvfrom(sockfd, buffer, MAX_MESSAGE, 0, (SOCKADDR *)&server_addr, &length);
    buffer[buffer_size] = '\0';
}

void register_Acc(){
	char *buffer = (char *)malloc(sizeof(char) * MAX);
    strcpy( token[0], userName);
    strcpy( token[1], password);
    // copy user và pass vào token
	buffer = GetMess(token,2,REGISTER_PACK);
	cleanToken(token,2);
	sendToServer(sockfd,server_addr,buffer);
}

void host_game(CurrentPlayer *currUser){
    char **token = makeCleanToken();
    char *buffer = (char*)calloc(MAX_MESSAGE ,sizeof(char));
    roomName = (char*)calloc(MAX_RNAME, sizeof(char));
    // strcpy(token[0], currUser->id);
    sprintf(token[0],"%d",currUser->id);
    strcpy(roomName , token[0]);
    memset(buffer, 0, sizeof(*buffer));
    buffer = GetMess(token, 1, HOST_GAME);
    sendToServer(sockfd, server_addr, buffer);
    memset(buffer, 0, sizeof(*buffer));
    ListenToServer(sockfd, server_addr, buffer);
    memset(token, 0, sizeof(*token[0]));;
    token = GetToken(buffer, 2);
    enum pack_type type = (enum pack_type)atoi(token[0]);
    if(type == SUCCEED_PACK){
        printf("[+]Room's status -> Creating -> %s\n", token[1]);
        strcpy(currUser->room, roomName);
        currUser->isHost = 1;
        memset(token, 0, sizeof(*token[0]));;
        
        return;
    }else if(type == ERROR_PACK){
        printf("[-]Room's status -> Creating -> %s\n", token[1]);
    }else{
        return;
    }
}

void GetRoomList( Row *roomlist, char **token){ // Join Game
    int i, j;
    char *buffer = (char*)calloc(MAX_MESSAGE ,sizeof(char));

    memset(buffer, 0, sizeof(*buffer));
    buffer = GetMess(token, 0, JOIN_GAME);
    sendToServer(sockfd, server_addr, buffer);

    memset(buffer, 0, sizeof(*buffer));
    ListenToServer(sockfd, server_addr, buffer);

    memset(token, 0, sizeof(*token[0]));
    token = GetToken(buffer, 2);
    
    enum pack_type type = (enum pack_type)atoi(token[0]);
    if(type == JOIN_GAME){
        roomcount = atoi(token[1]);
        printf("[+]Room founds: %d rooms!\n", roomcount);
    }else{
        printf("[-]Can't get room list from server!\n");
        return;
    }
    for(i = 0; i < roomcount; i++){
        roomlist[i].roomName = (char*)calloc(MAX_RNAME , sizeof(char));
        roomlist[i].playerCount = (char*)calloc(4 , sizeof(char));
        roomlist[i].maxPlayer = (char*)calloc(4 , sizeof(char));
    }

    memset(buffer, 0, sizeof(*buffer));
    ListenToServer(sockfd, server_addr, buffer);
    memset(token, 0, sizeof(*token[0]));
    token = GetToken(buffer, roomcount * 3 + 1);
    printf("token = %s!\n", token[1]);
    j = 1;
    for(i = 0; i < roomcount; i++){
        strcpy(roomlist[i].roomName, token[j++]);
        strcpy(roomlist[i].maxPlayer, token[j++]);
        strcpy(roomlist[i].playerCount, token[j++]);
        roomlist[i].roomName[strlen(roomlist[i].roomName)] = '\0';
        printf("roomName:%s\n", roomlist[i].roomName);
        roomlist[i].maxPlayer[strlen(roomlist[i].maxPlayer)] = '\0';
        roomlist[i].playerCount[strlen(roomlist[i].playerCount)] = '\0';
    }
    
    for(i = 0; i < roomcount *3 + 1; i++){
        memset(token[i], 0, sizeof(*token[i]));
    }
    free(buffer);
}

void join_room(int row){
    enum pack_type type;
    char *buffer = (char*)calloc(MAX_MESSAGE ,sizeof(char));
    printf("row = %d and room = %s and player = %s!\n", row, roomlist[row].roomName, currUser->name);
    memset(buffer, 0, sizeof(*buffer));
    strcpy(token[0], currUser->name);
    strcpy(token[1], roomlist[row].roomName);
    printf("a\n");
    sprintf(token[2],"%d",currUser->id);
    printf("b\n");
    printf("token = %s and %s!\n", token[0], token[1]);
    buffer = GetMess(token, 3, JOIN_ROOM);
    printf ("%s\n", buffer);
    sendToServer(sockfd, server_addr, buffer);
    memset(buffer, 0, sizeof(*buffer));
    ListenToServer(sockfd, server_addr, buffer);
    memset(token, 0, sizeof(*token[0]));
    token = GetToken(buffer, 2);
    type = (enum pack_type)atoi(token[0]);
    if(type == SUCCEED_PACK){
        printf("[+]Joining Room -> %s\n", token[1]);
        strcpy(currUser->room, roomlist[row].roomName);
        memset(token, 0, sizeof(*token[0]));
        // WaitingRoom(sockfd, server_addr, renderer, window, currUser);
        return;
    }else if(type == ERROR_PACK){
        printf("[-]Joining Room -> %s\n", token[1]);
            for(int i = 0; i < 2; i++){
                    memset(token[i], 0 ,sizeof(*token[i]));
                }
        }    
}

void login_to_server(){
    char *buffer = (char *)malloc(sizeof(char) * MAX);
    // int length = sizeof(server_addr);
    // int buff_size = 0;
    // int c;
    strcpy( token[0], userName);
    strcpy( token[1], password);
	buffer = GetMess(token,2,LOGIN_PACK);
	cleanToken(token,2);
	sendToServer(sockfd,server_addr,buffer);
}

void client_process(){

    closeClient();
}

bool InitData(){
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if( ret < 0)
        return false;
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    // Tao window
    g_window = SDL_CreateWindow( "Project L", 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SCREEN_WIDTH, SCREEN_HEIGHT, 
                                SDL_WINDOW_SHOWN);
    if( g_window == NULL){
        success = false;
    }else{
        g_screen = SDL_CreateRenderer( g_window, -1, SDL_RENDERER_ACCELERATED);
        if( g_screen == NULL){
            success = false;
        }else{
            SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if( !(IMG_Init( imgFlags) && imgFlags)){
                success = false;
            }
        }
    }
    if (TTF_Init() < 0){
        SDL_Log("%s", TTF_GetError());
        success = false;

    }
    font = TTF_OpenFont("font/arial.ttf", 30);
    if ( !font ) {
        SDL_Log("%s", TTF_GetError());
        success = false;
    }
    return success;
}

bool LoadBackground( string path){
    bool ret = g_background.LoadImg( path, g_screen);
    return ret;
}

void close_game( Menu *menuLogin){
    g_background.Free();
    menuLogin->Free();
    SDL_DestroyRenderer( g_screen);
    g_screen = NULL;
    SDL_DestroyWindow( g_window);
    g_window = NULL;
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

void drawGame( int health1, int health2){
    SDL_Rect rect1 = {10, 20, 400, 50};
    SDL_Rect rect2 = {550, 20, 400, 50};
    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
    SDL_RenderDrawRect(g_screen, &rect1);
    SDL_RenderDrawRect(g_screen, &rect2);
    rect1.w = max( 0, health1*4);
    rect2.w = max( 0, health2*4);
    SDL_SetRenderDrawColor(g_screen, 235, 7, 60, 0);
    SDL_RenderFillRect(g_screen, &rect1);
    SDL_RenderFillRect(g_screen, &rect2);

}

bool inGame( Player *player1, Player *player2, Input keyboard){
    std::string triggerAction;
    std::string triggerCollision;
    start = SDL_GetTicks();
    // Xem tk player có out
    bool is_quit = false;
    bool is_collision = false;
    is_quit = keyboard.getEvent( &g_event, player2->isInverted());
    // Trigger client
    triggerAction = keyboard.getTrigger();
    // Gửi lên cho server
    player2->updateState( triggerAction);

    SDL_SetRenderDrawColor( g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    SDL_RenderClear( g_screen);
    is_collision = player1->checkCollision( player2->getCollision());
    // trigger of collision
    triggerCollision = player2->getTrigger();
    if( is_collision){
        player1->updateState( triggerCollision);
        player2->checkSkill( player1->isHurt());
    }
    g_background.RenderMap( g_screen, NULL);

    player2->DoPlayer(is_collision);
    player2->draw( g_screen, font);

    player1->DoPlayer(is_collision);
    player1->draw( g_screen, font);
    drawGame( player1->getHealth(), player2->getHealth());
    SDL_RenderPresent( g_screen);
    if(1000/FPS > SDL_GetTicks()-start) {
        SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
    }
    return is_quit;
}

void Login(){
    State select = LOGIN;
    bool is_quit = false;
    while( !is_quit){
        switch(select){
            case EXIT:
                is_quit = true;
                select = EXIT;
                break;
            case LOGIN:
                select = showLogin( menuLogin, g_screen, g_event, font, userName, password);
                break;
            case SIGNIN:
                login_to_server();
                select = CHECKLOGIN;
                break;
            case SIGNUP:
                register_Acc();
                select = CHECKLOGIN;
                break;
            case CHECKLOGIN:{
                Arg *arg = (Arg*)calloc(1,sizeof(Arg));
                arg->buffer = (char*)calloc(100,sizeof(char));
                while(1){ 
                    memset(arg->buffer, 0, sizeof(*(arg->buffer)));
                    ListenToServer(sockfd, server_addr, arg->buffer);
                    printf("%s\n", arg->buffer);
                    arg->type = GetType(arg->buffer);
                    if(arg->type == LOGIN_PACK){
                        token = GetToken(arg->buffer,3);
                        printf("id cua ban la %d\n",atoi(token[2]));
                        currUser->id = atoi(token[2]);
                        if( strcmp(token[1], "OK") == 0){
                            printf("OK");
                            select = EXIT;
                            break;
                        }else{
                            select = LOGIN;
                        }
                    }
                }
                break;
            }
                
            default:
                select = LOGIN;
                // is_quit = true;
                break;
        }
    }
}

int main( int argc, char *argv[]){
    if( InitData() == false){
        return -1;
    }
    if( initLogin( menuLogin, font, g_screen) == false){
        return -1;
    }
    if( initMenu( menuState, font, g_screen) == false){
        printf("menu");
        return -1;
    }
    if( initJoin( menuJoin, font, g_screen) == false){
        printf("join");
        return -1;
    }
    if( initHost( menuHost, font, g_screen) == false){
        printf("host");
        return -1;
    }
    if( LoadBackground( "data//background.png") == false){
        return -1;
    }

    userName = (char*)calloc(30, sizeof(char));
    password = (char*)calloc(30, sizeof(char));
    // Init Client

    token = makeCleanToken();
    roomlist = (Row*)calloc(roomcount, sizeof(Row));
    GetCurrUser();
	setupClient();

    Login();

    State select = MENU;
    State prev_select = MENU;
    Input keyboard;
    Player player1;
    player1.initialize( "Player1", "xml/ryu.xml", "xml/fireball.xml", "xml/fireball_collision.xml", true, 400, 200);
    Player player2;
    player2.initialize( "Player2", "xml/ryu.xml", "xml/fireball.xml", "xml/fireball_collision.xml", false, 800, 200);
    
    bool is_quit = false;
    while( !is_quit){
        switch(select){
            case EXIT:
                is_quit = true;
                break;
            case MENU:
                select = showMenu( menuState, g_screen, g_event);
                if( select != BACK){
                    prev_select = MENU;
                }
                break;
            case HOST:
                host_game( currUser);
                select = showHost( menuHost, g_screen, g_event);
                if( select != BACK){
                    prev_select = HOST;
                }
                break;
            case JOIN:
                join_room(1);
                select = showJoin( menuJoin, g_screen, g_event);
                if( select != BACK){
                    prev_select = JOIN;
                }
                break;
            case BACK:
                select = prev_select;
                break;
            case INGAME:
                is_quit = inGame( &player1, &player2, keyboard);
                break;
            default:
                select = MENU;
                break;
        }
    }


    menuState->Free();
    menuHost->Free();
    menuJoin->Free();
    close_game( menuLogin);
    closeClient();
    printf("Hello");
    return 0;
}