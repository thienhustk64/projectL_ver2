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

typedef struct Argument{
    int index;
	char* message;
    struct sockaddr_in client;
}Argument;

typedef struct Room{
    char *room_id;
    int *player_id;
    int max_client;
    int *playerRole;
	int client_count;
    int isStart;
    int currentRole;
}Room;

typedef struct Client{
	struct sockaddr_in client;
	char *name;
	int id;
    char *password;
    int rank_point;
	Room *room;
	int host;
    int heal;
}Client;

Client *client_list;
pthread_t tid, tid_2, tid_3;
SOCKET sockfd;
SOCKET pingfd;
struct sockaddr_in server_addr, client_addr, ping_addr;
Client *client_list;
Room *room_list;
char **token;

int setupServer(){ 
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
		printf("[-]Failed. Error Code : %d\n", WSAGetLastError());
		return -1;
	}

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET){
		printf("[-]Could not create socket : %d\n" , WSAGetLastError());
        return -1;
	}

    ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(5500);

	if(bind(sockfd ,(SOCKADDR *)&server_addr , sizeof(server_addr)) == SOCKET_ERROR){
		printf("[-]Bind failed with error code : %d\n" , WSAGetLastError());
		return -1;
	}

    return 1;
}

struct sockaddr_in ListenToClient(SOCKET sockfd, struct sockaddr_in client_addr, char *buffer){
    int buffer_size, length = sizeof(client_addr);
    buffer_size = recvfrom(sockfd, buffer, 100, 0, (SOCKADDR *)&client_addr, &length);
    buffer[buffer_size] = '\0';
    return client_addr;
}
void sendToClient(SOCKET sockfd, struct sockaddr_in client_addr, char *buffer){
    int length = (int)sizeof(client_addr);
    sendto(sockfd, buffer, strlen(buffer), 0, (SOCKADDR *)&client_addr, length);
}
void closeServer(){
    closesocket(sockfd);
	WSACleanup();
}

Argument *getArument(int index, char *message, struct sockaddr_in client){
    Argument *arg = calloc(1, sizeof(Argument));
    arg->message = calloc(100, sizeof(char));
    arg->index = index;
    arg->client = client;
    memset(arg->message, 0, sizeof(*arg->message));
    strcpy(arg->message, message);
    return arg;
}

void addClient(int index, char **token, struct sockaddr_in client, int rank_point){
 
    client_list[index].id = index + 1;
    strcpy( client_list[index].name, token[1] );
    strcpy( client_list[index].password, token[2] );
    client_list[index].client = client;
    client_list[index].rank_point = 0;
    printf("[+]%s with id %d from ip %s is joined server!\n", client_list[index].name, client_list[index].id, inet_ntoa(client_list[index].client.sin_addr));
   
}

void* clientHandle(void *argument){
    pthread_detach(pthread_self());
    int i, j = -1;
    int index ;
    int curr_id;
    int *list = calloc(MAX_ROOM, sizeof(int));
    char *tmp, *tmp2;
    char *buffer = calloc(MAX_MESSAGE, sizeof(char));
    Argument *arg = (Argument *) argument;
    enum mess_type type = GetType(arg->message);
    printf("[+]Client %d\n", (int)type);
   
    switch (type){
    case REGISTER_GAME:
            token = GetToken(arg->message, 3);
            FILE *pt = fopen("account.txt", "a+");
            fprintf(pt,"\n%d",arg->index + 1);
            fprintf(pt," %s",token[1]);
            fprintf(pt," %s",token[2]);
            fprintf(pt," %d", 0);
            cleanToken(token, 3);
            sprintf(token[1], "%d", client_list[i].id);
            strcpy(token[0], "OK");
            buffer = MakeMessage(token, 2, LOGIN_SUCCESS);
            cleanToken(token,2);
            sendToClient(sockfd, arg->client, buffer);
            fclose(pt);
            break; 
        break;
    
    case LOGIN_GAME:
        token = GetToken(arg->message, 3);
        if(arg->index != -1){
            printf("SUCCESS login: Client ID =  %d\n",client_list[arg->index].id);
            cleanToken(token,3);
            sprintf(token[1],"%d",client_list[arg->index].id);
            strcpy(token[0], "OK");
            buffer = MakeMessage(token, 2, LOGIN_SUCCESS);
            cleanToken(token,2);
            sendToClient(sockfd,arg->client,buffer);
        }
        else if (arg->index == -1){
            printf("FAIL login!");
            cleanToken(token, 3);
            sprintf(token[0], "Login fail!");
            buffer = MakeMessage(token, 1, LOGIN_FAIL);
            cleanToken(token, 1);
            sendToClient(sockfd, arg->client, buffer);
        }  
        break;
    }

}

void startServer(){
    int i; 
    int index;
    int check = 0;
    enum mess_type type;
    char *buffer = calloc(100, sizeof(char));
    while(1){
        memset(buffer, 0, sizeof(*buffer));
        ZeroMemory(&client_addr, sizeof(client_addr));
        client_addr = ListenToClient(sockfd, client_addr, buffer);
        printf("%s\n", buffer);
        type = GetType(buffer);
        if (type == LOGIN_GAME){
            token = GetToken(buffer, 3);
            for(i = 0 ; i < MAX_CLIENT ; i ++){
                if(strcmp(client_list[i].name, token[1]) == 0 && strcmp(client_list[i].password,token[2]) == 0 ){
                    check = 1;
                    index = i;
                    break;
                }else{
                    check = -1;
                }
            }

            if (check == -1){
                cleanToken(token, 3);
                Argument *arg = getArument(-1, buffer, client_addr);
                pthread_create(&tid, NULL, &clientHandle, (void *)arg);
            }else if (check == 1){
                cleanToken(token, 3);
                Argument *arg = getArument(index, buffer, client_addr);
                pthread_create(&tid, NULL, &clientHandle, (void *)arg);
            }
               
        }else if (type == REGISTER_GAME){
            token = GetToken(buffer, 3);
            for (i = 0; i < MAX_CLIENT; i++){
                if (client_list[i].id == -1){
                    addClient(i, token, client_addr , 0);
                    cleanToken(token, 3);
                    Argument *arg = getArument(i, buffer, client_addr);
                    pthread_create(&tid, NULL, &clientHandle, (void *)arg);
                    break;
                }
                
            }
            
        }
    }
}

void readFile(Client *client_list){
    FILE *fp = fopen("account.txt","r");
    if(fp == NULL){
        printf("file not found\n");
    }
    else{
        int i = 0;
        char c = fgetc(fp);
        while (c != EOF){
            if(c != '\n' && c != '\t' && c != ' ' && c != EOF){
                fseek(fp, -1, SEEK_CUR);
                fscanf(fp, "%d", &client_list[i].id);
                fscanf(fp, "%s", client_list[i].name);
                fscanf(fp, "%s",client_list[i].password);
                fscanf(fp, "%d", &client_list[i].rank_point);
                i++;   
            }
            c = fgetc(fp);
        }
        fclose(fp);
    }
}

int check_rank(int rank_point){
    if (rank_point >= 0 && rank_point < 200)
    {
        return IRON;
    }else if (rank_point >= 200 && rank_point < 400)
    {
        return BRONZE;
    }else if (rank_point >= 400 && rank_point < 600)
    {
        return SILVER;
    }else if (rank_point >= 600 && rank_point < 800)
    {
        return GOLD;
    }else if (rank_point >= 800 && rank_point < 1000)
    {
        return PLATINUM;
    }else if (rank_point >= 1000)
    {
        return DIAMOND;
    }  
    return -1;
}

int main(){

    token = makeToken();
    client_list = (Client*)malloc(sizeof(Client)*MAX_CLIENT); 
  
    for(int i = 0 ; i < MAX_CLIENT ; i++){
        client_list[i].id = -1;
        ZeroMemory(&client_list[i].client, sizeof(client_list[i].client));
        client_list[i].name = (char*)malloc(sizeof(char)*MAX);
        client_list[i].password = (char*)malloc(sizeof(char)*MAX);
    }
    readFile(client_list);
    printf("%s\n",client_list[1].name);
    printf("%d\n",client_list[1].rank_point);
    // room_list = calloc(MAX_ROOM, sizeof(Room));
	// for(int i = 0; i < MAX_ROOM; i++){
	// 	room_list[i].room_id = calloc(MAX_NAME, sizeof(char));
	// 	room_list[i].client_count = 0;
	// }

    if(setupServer() == 1){
        printf("[+]Server is now online!\n");
        startServer();
    }
}