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
#include"packet.h"
#include"define.h"
//local variable
struct sockaddr_in server_addr;
pthread_t tid;
SOCKET sockfd;
WSADATA wsa;
char **token ;
char *roomName;

CurrentPlayer *currUser;

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
    currUser = calloc(1, sizeof(CurrentPlayer));
    currUser->name = calloc(MAX_NAME, sizeof(char));
    currUser->room = calloc(MAX_RNAME, sizeof(char));
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
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR){
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
    int length = sizeof(server_addr);
    int buff_size = 0;
    int c;
    printf("User: ");
    fflush(stdin);
    scanf("%[^\n]%*c", token[0]);
    printf("Password: ");
    fflush(stdin);
    scanf("%[^\n]%*c", token[1]);
	buffer = GetMess(token,2,REGISTER_PACK);
	makeCleanToken(token,2);
	sendToServer(sockfd,server_addr,buffer);
}
void host_game(CurrentPlayer *currUser){
    
    char **token = makeCleanToken();
    char *buffer = calloc(MAX_MESSAGE ,sizeof(char));
    roomName = calloc(MAX_RNAME, sizeof(char));
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
    char *buffer = calloc(MAX_MESSAGE ,sizeof(char));

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
        roomlist[i].roomName = calloc(MAX_RNAME , sizeof(char));
        roomlist[i].playerCount = calloc(4 , sizeof(char));
        roomlist[i].maxPlayer = calloc(4 , sizeof(char));
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
    char *buffer = calloc(MAX_MESSAGE ,sizeof(char));
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
    int length = sizeof(server_addr);
    int buff_size = 0;
    int c;
    printf("User: ");
    fflush(stdin);
    scanf("%[^\n]%*c", token[0]);
    printf("Password: ");
    fflush(stdin);
    scanf("%[^\n]%*c", token[1]);
	buffer = GetMess(token,2,LOGIN_PACK);
	makeCleanToken(token,2);
	sendToServer(sockfd,server_addr,buffer);
}

void *handleMess(void *argument){
    int choose = 0;
    int row;
	pthread_detach(pthread_self());
    int id;
	Arg *arg = (Arg *)argument;
	// printf("[+]Server: %s!\n", arg->buffer);
	 while(1){ 
        memset(arg->buffer, 0, sizeof(*(arg->buffer)));
        ListenToServer(sockfd, server_addr, arg->buffer);
		printf("%s\n", arg->buffer);
        arg->type = GetType(arg->buffer);
		if(arg->type == LOGIN_PACK){
			token = GetToken(arg->buffer,3);
			printf("id cua ban la %d\n",atoi(token[2]));
            currUser->id = atoi(token[2]);
            printf("chon :\n");
            scanf("%d", &choose);
            if (choose == 1)
            {
                host_game(currUser);
            }
            if (choose == 2)
            {
                GetRoomList(roomlist, token);
                fflush(stdin);
                printf("Chon join phong: \n");
                scanf("%d", &row);
                if (row == 0)
                {
                    join_room(row);
                }
                
                
            }
            
            
            
            
		}
        
	 }

}


void client_process(){

    closeClient();
}
int main(){
	token = makeCleanToken();
    roomlist = calloc(roomcount, sizeof(Row));
    GetCurrUser();
	setupClient();
	// pthread_create(&tid, NULL, &Listen_To_Server, NULL);
	
	login_to_server();
    
	// register_Acc();
    
	Arg *arg = calloc(1,sizeof(Arg));
	arg->buffer = calloc(100,sizeof(char));
	pthread_t pid;
	pthread_create(&pid,NULL,handleMess,(void*)arg);

  
	while(1);
    
	return 0;
}
