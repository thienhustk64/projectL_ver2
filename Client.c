#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>


//local variable
struct sockaddr_in server_addr;
SOCKET sockfd;
WSADATA wsa;
typedef struct data{
	char str[100];
	char str2[100];
}data;
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
void Listen_To_Server(){
	// pthread_detach(pthread_self());
	int buffer_size, i;
	// enum pack_type type;
	char *buffer = calloc(1000, sizeof(char));
	// char **token = calloc(10, sizeof(char *));
	// for(i = 0; i < 10; i++){
	// 	token[i] = calloc(1000, sizeof(char));
	// }
    int k;
	int length = sizeof(server_addr);
	data tmp ;
	strcpy(tmp.str,"Thanh");
	strcpy(tmp.str2,"deptrai");
	do{
		memset(buffer, 0, sizeof(*buffer));
		strcat(buffer,tmp.str);
		strcat(buffer,"|");
		strcat(buffer,tmp.str2);
		// ZeroMemory(token, sizeof(token));
		// for(i = 0; i < 10; i++){
		// 	memset(token[i], '\0', 1000);
		// }
        scanf("%d",&k);
        
		// buffer_size = recvfrom(sockfd, buffer, 1000, 0, (SOCKADDR *) &server_addr, &length);
		// buffer[buffer_size] = '\0';
        sendToServer(sockfd,server_addr,buffer);
		// type = GetMessageType(buffer);

		// switch (type){
		// case PUBLIC_MESSAGE_PACK:
		// 	break;
		// default:
		// 	printf("Can't recognize message!\n");
		// 	return;
		// 	break;
		// }
	}while(k != 0 );
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
	// char *name_client = calloc(MAX_OF_NAME, sizeof(char));
	// char *guess_client = calloc( MAX_OF_NAME, sizeof(char));
    // int length = sizeof(server_addr);
	// char *buffer = calloc(MAX_OF_CHAR_MESSAGE, sizeof(char));
	// char *message = calloc(MAX_OF_CHAR_MESSAGE, sizeof(char));
	// pthread_t tid[2];
	// Packet *pack = calloc(1, sizeof(Packet));
	// int buffer_size;
	// if(setupClient() == SUCCEED_RETURN){
	// 	name_client = Login_To_Server();
	// 	strcpy(pack->message, name_client);
	// 	pack->type = LOGIN_PACK;
	// 	message = GetPackMessage(pack);
	// 	sendto(sockfd, message, strlen(message), 0, (SOCKADDR *)&server_addr, length);
	// 	buffer_size = recvfrom(sockfd, buffer, MAX_OF_CHAR_MESSAGE, 0, (SOCKADDR *) &server_addr, &length);
	// 	buffer[buffer_size] = '\0';
	// 	printf("%s\n", buffer);
	// 	//Connect to server
	// 	pthread_create(&tid[0], NULL, (void * (*)(void*))Listen_To_Server, name_client);
	// 	while(1){
	// 		memset(message, '\0', MAX_OF_CHAR_MESSAGE);
	// 		ZeroMemory(pack , sizeof(pack));
	// 		//printf("%s : ", name_client);
	// 		fflush(stdin);
	// 		scanf("%[^\n]%*c", pack->message);
	// 		if(strlen(pack->message) == 0){
	// 			printf("Exit client\n");
	// 			pack->type = EXIT_PACK;
	// 			message = GetPackMessage(pack);
	// 			sendto(sockfd, message, strlen(message), 0, (SOCKADDR *) &server_addr, length);
	// 			break;
	// 		}else{
	// 			pack->type = PUBLIC_MESSAGE_PACK;
	// 			message = GetPackMessage(pack);
	// 			sendto(sockfd, message, strlen(message) , 0, (SOCKADDR *) &server_addr, length);
	// 			memset(buffer, '\0', MAX_OF_CHAR_MESSAGE);
	// 		}
	// 	}
	// }
    closeClient();
}
int main(){
	setupClient();
    Listen_To_Server();
	return 0;
}