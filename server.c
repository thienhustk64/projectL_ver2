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
#define CLIENT_TIMEOUT 4
#define MAX_CLIENT 100 //Max clients can connect to server
int validRoom = 0;
int count = 0;
typedef struct Argument{
	char* message;
    struct sockaddr_in client;
}Argument;
pthread_t tid, tid_2, tid_3;
typedef struct Room{
    char *room_id;
    int *player_id;
    int max_client;
    int *playerRole;
	int client_count;
    int isStart;
    int isNight;
    int w;
    int v;
    int currentRole;
    int current_wolf;
}Room;

typedef struct Client{
	struct sockaddr_in client;
	char *name;
	int id;
    char *password;
	Room *room;
	int host;
    int heal;
}Client;

// local variable
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
    // if((pingfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET){
    //     printf("[-]Could not create socket : %d\n" , WSAGetLastError());
    //     return -1;
    // }

    ZeroMemory(&server_addr, sizeof(server_addr));
    // ZeroMemory(&ping_addr, sizeof(ping_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(5500);

    // ping_addr.sin_family = AF_INET;
	// ping_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
	// ping_addr.sin_port = htons(PORT + 1);

    // if(bind(pingfd ,(SOCKADDR *)&ping_addr , sizeof(ping_addr)) == SOCKET_ERROR){
	// 	printf("[-]Bind failed with error code : %d\n" , WSAGetLastError());
	// 	return ERROR_RETURN;
	// }
	
	if(bind(sockfd ,(SOCKADDR *)&server_addr , sizeof(server_addr)) == SOCKET_ERROR){
		printf("[-]Bind failed with error code : %d\n" , WSAGetLastError());
		return -1;
	}

    return 1;
}

// void sendToClient(SOCKET sockfd, struct sockaddr_in client_addr, char *buffer){
//     int length = (int)sizeof(client_addr);
//     sendto(sockfd, buffer, strlen(buffer), 0, (SOCKADDR *)&client_addr, length);
// }

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

void addClient(int index, char **token, struct sockaddr_in client){
 
    client_list[index].id = index + 1;
    // strcpy(client_list[index].name, client_name);
    strcpy( client_list[index].name, token[1] );
    strcpy( client_list[index].password, token[2] );
    client_list[index].client = client;
    printf("[+]%s with id %d from ip %s is joined server!\n", client_list[index].name, client_list[index].id, inet_ntoa(client_list[index].client.sin_addr));
   
}

Argument *getArument( char *message, struct sockaddr_in client){
    Argument *arg = calloc(1, sizeof(Argument));

    arg->message = calloc(100, sizeof(char));
    arg->client = client;
    memset(arg->message, 0, sizeof(*arg->message));
    strcpy(arg->message, message);
    return arg;
}


void addPlayer(Room *room, int index){
    int i;
    for(i = 0; i < room->max_client; i++){
        if(room->player_id[i] == 0){
            room->player_id[i] = client_list[index].id;
            room->client_count++;
            client_list[index].room = room;
            printf("[+]'%s' with id '%d' is joined room '%s'!\n", client_list[index].name, client_list[index].id, room->room_id);
            break;
        }
    }
}

void GetRoom(int index){
    int i, j;
    char *buffer = calloc(MAX_MESSAGE, sizeof(char));
    sprintf(token[0], "%d",client_list[index].room->client_count);
    sprintf(token[1], "%d",client_list[index].room->max_client);
    buffer = GetMess(token, 2, ROOM_INFO);
    sendToClient(sockfd, client_list[index].client, buffer);
    cleanToken(token , 2);
    j = 0;
    for(i = 0; i < client_list[index].room->max_client; i++){
        if(client_list[index].room->player_id[i] != 0){
            sprintf(token[j++], "%d", client_list[index].room->player_id[i]);
            strcpy(token[j++], client_list[client_list[index].room->player_id[i] - 1].name);
        }
        
    }
    memset(buffer, 0, sizeof(*buffer));
    buffer = GetMess(token, j, ROOM_INFO);
    cleanToken(token ,j);
    sendToClient(sockfd, client_list[index].client, buffer);
    free(buffer);
}

int checkRoom(char *roomName){
    int i;
    for(i = 0; i < MAX_ROOM; i++){
        if(strcmp(room_list[i].room_id, roomName) == 0){
            return i;
        }
    }
    return 0;
}

int *countValidRoom(int *validRoom){
    int i;
    int *list = calloc(MAX_ROOM, sizeof(int));
    for(i = 0; i < MAX_ROOM; i++){
        list[i] = -1;
    }
    *validRoom = 0;
    for(i = 0; i < MAX_ROOM; i++){
        if(strlen(room_list[i].room_id) > 0 && room_list[i].client_count < 2){
            list[i] = i;
            *validRoom = *validRoom + 1;
        }
    }
    return list;
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
    enum pack_type type = GetType(arg->message);
    printf("[+]Client %d\n", (int)type);
   
    switch (type){
    case REGISTER_PACK:
        for(i = 0 ; i  < MAX_CLIENT ;i++){
            if (client_list[i].id == -1){
                int curr_id = i;
                token = GetToken(arg->message, 3);
                addClient(i, token, arg->client);
                FILE *pt = fopen("account.txt", "a+");
                fprintf(pt,"\n%d",i+1);
                fprintf(pt," %s",token[1]);
                fprintf(pt," %s",token[2]);
                cleanToken(token, 3);
                sprintf(token[1], "%d", client_list[i].id);
                strcpy(token[0], "OK");
                buffer = GetMess(token, 2, LOGIN_PACK);
                cleanToken(token,2);
                sendToClient(sockfd, arg->client, buffer);
                fclose(pt);
                break;
            }
        }
        
        break;
    
    case LOGIN_PACK:
  
         token = GetToken(arg->message, 3);
         for(i = 0 ; i  < MAX_CLIENT ;i++){
            
            if(client_list[i].id == -1)
                continue;
            if(strcmp(client_list[i].name, token[1]) == 0 && strcmp(client_list[i].password,token[2]) == 0 ){
                curr_id = i;
                printf("SUCCESS login %d\n",client_list[i].id);
                cleanToken(token,3);
                sprintf(token[1],"%d",client_list[i].id);
                strcpy(token[0], "OK");
                buffer = GetMess(token, 2, LOGIN_PACK);
                cleanToken(token,2);
                sendToClient(sockfd,arg->client,buffer);
            }
         }
         
        break;
    case HOST_GAME:
       
        token = GetToken(arg->message, 2);
        
        if(checkRoom(token[1]) != 0){
            cleanToken(token, 2);
            strcpy(token[1], "Room name already exists!");
            buffer = GetMess(token, 1, ERROR_PACK);
            cleanToken(token, 1);
        }else{
            // printf("id truyen den la %s",token[1]);
            for(i = 0; i < MAX_ROOM; i++){
                if(strlen(room_list[i].room_id) == 0){
                    strcpy(room_list[i].room_id, token[1]);
                    room_list[i].max_client = 2;
                    room_list[i].player_id = calloc(room_list[i].max_client, sizeof(int));
                    room_list[i].player_id[room_list[i].client_count] = client_list[i].id;
                    room_list[i].client_count++;
                    client_list[i].room = &room_list[i];
                    client_list[i].host = 1;
                    index = i;
                    break;
                  
                }
            }
            cleanToken(token, 2);

            printf("[+]%s is hosting room '%s' max player is '%d'!\n", client_list[index].name, client_list[index].room->room_id, client_list[index].room->max_client);
            strcpy(token[0], "OK!");
            buffer = GetMess(token, 1, SUCCEED_PACK);
            cleanToken(token, 1);
            sendToClient(sockfd, arg->client, buffer);
        }
        GetRoom(index);
        break;

    case JOIN_GAME:
        for(i = 0; i < MAX_ROOM; i++){
            list[i] = -1;
        }
        list = countValidRoom(&validRoom);
        printf("So phong : %d\n", validRoom);
        sprintf(token[0], "%d", validRoom);
        buffer = GetMess(token, 1, JOIN_GAME);
        sendToClient(sockfd, arg->client, buffer);
        cleanToken(token, 1);

        j = 0;
        for(i = 0; i < MAX_ROOM; i++){
            if(list[i] > -1){
                strcpy(token[j++], room_list[list[i]].room_id);
                sprintf(token[j++], "%d", room_list[list[i]].max_client);
                sprintf(token[j++], "%d", room_list[list[i]].client_count);
            }
        }
        buffer = GetMess(token, j, HOST_GAME);
        cleanToken(token, j);
        sendToClient(sockfd, arg->client, buffer);
        break;    
    
    case JOIN_ROOM:
        token = GetToken(arg->message, 4);
        printf(" ID join  %s\n", token[2]);
        for (int i = 0; i < MAX_CLIENT; i++)
        {
            if (client_list[i].id == atoi(token[2]))
            {
                index = i;
                break;
            }
        }
        
        j = checkRoom(token[2]);
        if(j != -1){
            if(room_list[j].client_count < room_list[j].max_client && room_list[j].isStart != 1){
                addPlayer(&room_list[j], index);
                cleanToken(token, 4);
                strcpy(token[0], "OK!");
                buffer = GetMess(token, 2, SUCCEED_PACK);
                cleanToken(token, 2);
                sendToClient(sockfd, client_list[index].client, buffer);
                for(i = 0; i < client_list[index].room->max_client; i++){
                    if(client_list[index].room->player_id[i] != 0){
                        GetRoom(client_list[index].room->player_id[i] - 1);
                    }
                }
            }
        }else{
            cleanToken(token, 4);
            strcpy(token[0], "Room is already full!");
            buffer = GetMess(token, 2, ERROR_PACK);
            cleanToken(token, 2);
            sendToClient(sockfd, client_list[index].client, buffer);
        }
        break;

    }
  
}
void startServer(){
    int i;
    enum pack_type type;

  
    char *buffer = calloc(100, sizeof(char));
  
    while(1){
        memset(buffer, 0, sizeof(*buffer));
        ZeroMemory(&client_addr, sizeof(client_addr));
        client_addr = ListenToClient(sockfd, client_addr, buffer);
        Argument *arg = getArument( buffer, client_addr);
        pthread_create(&tid, NULL, &clientHandle, (void *)arg);
        // printf("%s",buffer);
       
        
       
        // type = GetType(buffer);
        // if(type == LOGIN_PACK){
        //     for(i = 0; i < MAX_CLIENT; i++){
        //         if(client_list[i].id == -1){
        //             Argument *arg = getArument(i, buffer, client_addr);
        //             pthread_create(&tid, NULL, &clientHandle, (void *)arg);
        //             break;
        //         }
        //     }
        // }else{
        //     for(i = 0; i < MAX_CLIENT; i++){
        //         if((client_list[i].client.sin_addr.s_addr == client_addr.sin_addr.s_addr) && (client_list[i].client.sin_port == client_addr.sin_port)){
        //             Argument *arg = getArument(i, buffer, client_addr);
        //             pthread_create(&tid, NULL, &clientHandle, (void *)arg);
        //         }
        //     }
        
    }
}
void readFile(){
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
                i++;
               
            }
            c = fgetc(fp);
        }
        fclose(fp);
    }
}
int main(){
     token = makeCleanToken();
    client_list = (Client*)malloc(sizeof(Client)*MAX_CLIENT); 
  
    for(int i = 0 ; i < MAX_CLIENT ; i++){
        client_list[i].id = -1;
        ZeroMemory(&client_list[i].client, sizeof(client_list[i].client));
        client_list[i].name = (char*)malloc(sizeof(char)*MAX);
        client_list[i].password = (char*)malloc(sizeof(char)*MAX);
    }
    readFile();
    // printf("%s",client_list[0].name);
    // int i;
    room_list = calloc(MAX_ROOM, sizeof(Room));
	// client_list = calloc(MAX_CLIENT, sizeof(Client));
   
	for(int i = 0; i < MAX_ROOM; i++){
		room_list[i].room_id = calloc(MAX_NAME, sizeof(char));
		room_list[i].client_count = 0;

	}
	// for(i = 0; i < MAX_CLIENT; i++){
    //     client_list[i].role = -1;
	// 	client_list[i].host = -1;
	// 	client_list[i].id = -1;
	// 	client_list[i].name = calloc(MAX_NAME, sizeof(char));
    //     client_list[i].heal = 1;
    //     client_list[i].vote_count = 0;
    //     client_list[i].target = -1;
    //     ZeroMemory(&client_list[i].client, sizeof(client_list[i].client));
	// 	client_list[i].room = NULL; 
	// }
    // pthread_create(&tid_2, NULL, listenPingServer, NULL);
    if(setupServer() == 1){
        printf("[+]Server is now online!\n");
        startServer();
    }
}