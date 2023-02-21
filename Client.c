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
void send_pack(){
    int buffer_size, i;
	enum pack_type type;
	char *buffer = calloc(100, sizeof(char));
	
	char **token = makeCleanToken();
    int k;
	int length;
	
	strcpy(token[0],"Thanh");
	strcpy(token[1],"deptrai");
	do{
		memset(buffer, 0, sizeof(*buffer));
	    buffer = GetMess(token, 2, VOTE);
		// ZeroMemory(token, sizeof(token));
		// for(i = 0; i < 10; i++){
		// 	memset(token[i], '\0', 1000);
		// }
        sleep(3);
      
		// buffer_size = recvfrom(sockfd, buffer, 1000, 0, (SOCKADDR *) &server_addr, &length);
		// buffer[buffer_size] = '\0';

        sendToServer(sockfd,server_addr,buffer);
	
		memset(buffer,0,1000);
		length = sizeof(server_addr);
		buffer_size = recvfrom(sockfd,buffer,1000,0,(SOCKADDR *)&server_addr,&length);

		buffer[buffer_size] = '\0';
		printf("%s \n",buffer);
	
	
	}while(1);
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
            if(strcmp(token[1],"OK") == 0){
                printf("id cua ban la %d",atoi(token[2]));
                currUser->id = atoi(token[2]);
            // host_game(currUser);
            // send_pack();
            }
            else{
                printf("%s Please input again \n",token[1]);
                login_to_server();
            }
		}
        
	 }

}
// void Login_To_Server(char *player_name){
//     int buffer_size, i;
//     int length = (int)sizeof(server_addr);
//     char *message = calloc(MAX_OF_CHAR_MESSAGE, sizeof(char));
//     char *buffer = calloc(MAX_OF_CHAR_MESSAGE, sizeof(char));
// 	char **token = calloc(10, sizeof(char *));
// 	for(i = 0; i < 10; i++){
// 		token[i] = calloc(MAX_OF_CHAR_MESSAGE, sizeof(char));
// 	}
//     if(setupClient() == SUCCEED_RETURN){
// 		ZeroMemory(token, sizeof(token));
// 		for(i = 0;i < 10; i++){
// 			memset(token[i], '\0', MAX_OF_CHAR_MESSAGE);
// 		}
//         pthread_t tid;
//         message = GetMessage_Login(player_name);
//         sendto(sockfd, message, strlen(message), 0, (SOCKADDR *)&server_addr, length);
// 		buffer_size = recvfrom(sockfd, buffer, MAX_OF_CHAR_MESSAGE, 0, (SOCKADDR *)&server_addr, &length);
// 		buffer[buffer_size] = '\0';
// 		enum pack_type type = GetMessageType(buffer);
// 		if(type == SUCCEED_PACK){
// 			token = GetToken(buffer, 2);
// 			printf("Server: %s\n", token[1]);
// 		}else if(type == ERROR_PACK){
// 			token = GetToken(buffer, 2);
// 			printf("Server: %s\n", token[1]);
// 		}
//         //pthread_create(&tid, NULL, (void *(*)(void *))Listen_To_Server, buffer);
// 	}
// }

void client_process(){

    closeClient();
}
int main(){
	token = makeCleanToken();
    
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
