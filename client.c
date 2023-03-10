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
#include"message.h"
#include"define.h"

struct sockaddr_in server_addr;
pthread_t tid1, tid2;
pthread_t tid;
SOCKET sockfd;
WSADATA wsa;
char **token ;
char *roomName;

PlayerInfor *currUser;

typedef struct account{
	int id;
	char *username;
	char *password;
}account;


typedef struct _Arg{
	PlayerInfor *currUser;
    enum mess_type type;
    char *buffer;
}Arg;

typedef struct _Row{
    char *roomName;
    char *playerCount;
    char *maxPlayer;
}Row;
int roomcount;
Row *roomlist;
void GetCurrUser(PlayerInfor *currUser ){
    // currUser = (PlayerInfor*)calloc(1, sizeof(PlayerInfor));
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

void closeClient(SOCKET sockfd){
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

void *sendToPingServer(){
    pthread_detach(pthread_self());
    char *buffer = calloc(4, sizeof(char));
    sprintf(buffer, "%d", currUser->id);
    printf("%s\n", buffer);
    struct sockaddr_in pingServer;
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ZeroMemory(&pingServer, sizeof(pingServer));
    pingServer.sin_family = AF_INET;
	pingServer.sin_port = htons(PORT + 1);
	pingServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP_ADDR);
    while(currUser->id != -1){
        sendToServer(sockfd, pingServer, buffer);
        Sleep(1000);
    }
    return NULL;
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
	buffer = MakeMessage(token,2,REGISTER_GAME);
	cleanToken(token,2);
	sendToServer(sockfd,server_addr,buffer);
    printf("send OK %s\n", buffer);
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
	buffer = MakeMessage(token,2,LOGIN_GAME);
	cleanToken(token,2);
	sendToServer(sockfd,server_addr,buffer);
}

void host_game(PlayerInfor *currUser){   
    char **token = makeToken();
    char *buffer = (char*)calloc(MAX_MESSAGE ,sizeof(char));
    roomName = calloc(MAX_RNAME, sizeof(char));
    sprintf(token[0],"%s",currUser->name);
    strcpy(roomName , token[0]);
    memset(buffer, 0, sizeof(*buffer));
    buffer = MakeMessage(token, 1, HOST_GAME);
    sendToServer(sockfd, server_addr, buffer);
    memset(buffer, 0, sizeof(*buffer));
    ListenToServer(sockfd, server_addr, buffer);
    memset(token, 0, sizeof(*token[0]));
    token = GetToken(buffer, 2);
    enum mess_type type = (enum mess_type)atoi(token[0]);
    if(type == SUCCEED_PACK){
        printf("[+]Room's status -> Creating -> %s\n", token[1]);
        strcpy(currUser->room, roomName);
        currUser->isHost = 1;
        memset(token, 0, sizeof(*token[0]));;
        printf("Ban dang trong room : %s\n",currUser->room );
        
        return;
    }else if(type == ERROR_PACK){
         printf("[-]Room's status -> Creating -> %s\n", token[1]);
    }else{
        return;
    }
}

void GetRoomList( Row *roomlist, char **token){ // Find ROOM
    int i, j;
    char *buffer = calloc(MAX_MESSAGE ,sizeof(char));
    memset(buffer, 0, sizeof(*buffer));
    buffer = MakeMessage(token, 0, FIND_ROOM);
    sendToServer(sockfd, server_addr, buffer);
    memset(buffer, 0, sizeof(*buffer));
    ListenToServer(sockfd, server_addr, buffer);

    memset(token, 0, sizeof(*token[0]));
    token = GetToken(buffer, 2);
    
    enum mess_type type = (enum mess_type)atoi(token[0]);
    if(type == FIND_ROOM){
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
    j = 1;
    for(i = 0; i < roomcount; i++){
        strcpy(roomlist[i].roomName, token[j++]);
        strcpy(roomlist[i].maxPlayer, token[j++]);
        strcpy(roomlist[i].playerCount, token[j++]);
        roomlist[i].roomName[strlen(roomlist[i].roomName)] = '\0';
        roomlist[i].maxPlayer[strlen(roomlist[i].maxPlayer)] = '\0';
        roomlist[i].playerCount[strlen(roomlist[i].playerCount)] = '\0';
    }

    for (i = 0; i < roomcount; i++){
        printf("%d roomName:%s max %s players , now: %s\n",i , roomlist[i].roomName, roomlist[i].maxPlayer, roomlist[i].playerCount);
    }
    
    for(i = 0; i < roomcount *3 + 1; i++){
        memset(token[i], 0, sizeof(*token[i]));
    }
    free(buffer);
}

void join_room(int row, Row *roomlist, PlayerInfor *currUser){
    printf("OK\n");
    enum mess_type type;
    char *buffer = calloc(MAX_MESSAGE ,sizeof(char));
    printf("OK\n");
    printf("row = %d and room = %s and player = %d!\n", row, roomlist[row].roomName, currUser->id);
    memset(buffer, 0, sizeof(*buffer));
    strcpy(token[0], roomlist[row].roomName);
    printf("token = %s!\n", token[0]);
    buffer = MakeMessage(token, 2, JOIN_ROOM);
    cleanToken(token,1);
    printf ("%s\n", buffer);
    sendToServer(sockfd, server_addr, buffer);
    memset(buffer, 0, sizeof(*buffer));
    ListenToServer(sockfd, server_addr, buffer);
    printf("a\n");
    memset(token, 0, sizeof(*token[0]));
    token = GetToken(buffer, 2);
    
    type = (enum mess_type)atoi(token[0]);
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
        printf("%d\n", arg->type);
		if(arg->type == LOGIN_SUCCESS){
            GetCurrUser(arg->currUser);
			token = GetToken(arg->buffer,4);
			printf("id cua %s la %d\n", token[3],atoi(token[2]));
            arg->currUser->id = atoi(token[2]);
            currUser->id = arg->currUser->id;
            strcpy(arg->currUser->name, token[3]);
            cleanToken(token, 4);
            printf("chon :\n");
            scanf("%d", &choose);
            if (choose == 1)
            {   printf("ID hien tai la %d\n", currUser->id);
                pthread_create(&tid1, NULL, sendToPingServer, NULL);
                host_game(arg->currUser);
                break;
            }else if (choose == 2)
            {   
                pthread_create(&tid2, NULL, sendToPingServer, NULL);
                GetRoomList(roomlist, token);
                fflush(stdin);
                printf("Chon join phong: \n");
                scanf("%d", &row);
                if (row == 0)
                {   
                    printf("Row = %d \n", row);
                    join_room(row,  roomlist , arg->currUser);
                }
            }
            

        }
        else if (arg->type == LOGIN_FAIL)
        {   
            printf("Login fail ! Try again !\n");
            login_to_server();
        }
        
        
	 }

}

int main(){
	token = makeToken();
    roomlist = calloc(20, sizeof(Row));
    
	setupClient();
	// pthread_create(&tid, NULL, &Listen_To_Server, NULL);
	
	login_to_server();
    
	// register_Acc();
    
	Arg *arg = calloc(1,sizeof(Arg));
	arg->buffer = calloc(100,sizeof(char));
    arg->currUser = calloc(1,sizeof(PlayerInfor));

    currUser = (PlayerInfor*)calloc(1, sizeof(PlayerInfor));
    currUser->name = (char*)calloc(MAX_NAME, sizeof(char));
    currUser->room = (char*)calloc(MAX_RNAME, sizeof(char));
    currUser->id = -1;
    currUser->isHost = -1;
	// pthread_t pid;
	// pthread_create(&pid,NULL,handleMess,(void*)arg);
    handleMess(arg);

  
	while(1);
    
	return 0;
}