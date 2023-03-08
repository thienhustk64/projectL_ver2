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
    currUser = (PlayerInfor*)calloc(1, sizeof(PlayerInfor));
    currUser->name = (char*)calloc(MAX_NAME, sizeof(char));
    currUser->room = (char*)calloc(MAX_RNAME, sizeof(char));
    currUser->id = -1;
    currUser->isHost = -1;
}
int setupClient(SOCKET sockfd, struct sockaddr_in server_addr){
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
	makeToken(token,2);
	sendToServer(sockfd,server_addr,buffer);
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
	makeToken(token,2);
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
        printf("%d\n", arg->type);
		if(arg->type == LOGIN_SUCCESS){
			token = GetToken(arg->buffer,3);
			printf("id cua ban la %d\n",atoi(token[2]));
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
    roomlist = calloc(roomcount, sizeof(Row));
    GetCurrUser(currUser);
	setupClient(sockfd, server_addr);
	// pthread_create(&tid, NULL, &Listen_To_Server, NULL);
	
	// login_to_server();
    
	register_Acc();
    
	Arg *arg = calloc(1,sizeof(Arg));
	arg->buffer = calloc(100,sizeof(char));
	pthread_t pid;
	pthread_create(&pid,NULL,handleMess,(void*)arg);

  
	while(1);
    
	return 0;
}