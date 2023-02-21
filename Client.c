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
void ListenToServer(SOCKET sockfd, struct sockaddr_in server_addr, char *buffer){
    int buffer_size, length = sizeof(server_addr);
    buffer_size = recvfrom(sockfd, buffer, MAX_MESSAGE, 0, (SOCKADDR *)&server_addr, &length);
    buffer[buffer_size] = '\0';
}
void *handleMess(void *argument){
	pthread_detach(pthread_self());
	Arg *arg = (Arg *)argument;
	// printf("[+]Server: %s!\n", arg->buffer);
	 while(1){ 
        memset(arg->buffer, 0, sizeof(*(arg->buffer)));
        ListenToServer(sockfd, server_addr, arg->buffer);
		printf("%s\n", arg->buffer);
        arg->type = GetType(arg->buffer);
		if(arg->type == LOGIN_PACK){
			token = GetToken(arg->buffer,3);
			printf("id cua ban la %d",atoi(token[2]));
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
	setupClient();
	// pthread_create(&tid, NULL, &Listen_To_Server, NULL);
	
	// login_to_server();
	register_Acc();
	Arg *arg = calloc(1,sizeof(Arg));
	arg->buffer = calloc(1000,sizeof(char));
	pthread_t pid;
	pthread_create(&pid,NULL,handleMess,(void*)arg);
	while(1);
	return 0;
}
