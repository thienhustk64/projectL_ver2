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

int validRoom = 0;

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
    int room_rank;
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

void updateClient(int index, struct sockaddr_in client){
    client_list[index].client = client;
    printf("[+]%s with id %d from ip %s is joined server!\n", client_list[index].name, client_list[index].id, inet_ntoa(client_list[index].client.sin_addr));  
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

int *countValidRoom(int *validRoom , int client_index){
    int i;
    int *list = calloc(MAX_ROOM, sizeof(int));
    for(i = 0; i < MAX_ROOM; i++){
        list[i] = -1;
    }
    *validRoom = 0;
    for(i = 0; i < MAX_ROOM; i++){
        if(strlen(room_list[i].room_id) > 0 && room_list[i].client_count < 2 && room_list[i].isStart == -1 && room_list[i].room_rank == check_rank(client_list[client_index].rank_point)){
            list[i] = i;
            *validRoom = *validRoom + 1;
        }
    }
    return list;
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
            sprintf(token[1], "%d", client_list[arg->index].id);
            strcpy(token[0], "OK");
            sprintf(token[2], "%s", client_list[arg->index].name);
            buffer = MakeMessage(token, 3, LOGIN_SUCCESS);
            cleanToken(token,3);
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
            sprintf(token[2], "%s", client_list[arg->index].name);
            buffer = MakeMessage(token, 3, LOGIN_SUCCESS);
            cleanToken(token,3);
            sendToClient(sockfd,arg->client,buffer);
            printf("OK\n");
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

    case HOST_GAME:
        token = GetToken(arg->message, 2);   
        printf("%s\n", arg->message);
        if(checkRoom(token[1]) != 0){
            cleanToken(token, 2);
            strcpy(token[1], "Room name already exists!");
            buffer = MakeMessage(token, 1, ERROR_PACK);
            cleanToken(token, 1);
        }else{
            // printf("id truyen den la %s",token[1]);
            for(i = 0; i < MAX_ROOM; i++){
                if(strlen(room_list[i].room_id) == 0){
                    strcpy(room_list[i].room_id, token[1]);
                    room_list[i].max_client = 2;
                    room_list[i].player_id = calloc(room_list[i].max_client, sizeof(int));
                    room_list[i].player_id[room_list[i].client_count] = client_list[arg->index].id;
                    room_list[i].room_rank = check_rank(client_list[arg->index].rank_point);
                    room_list[i].client_count++;
                    client_list[arg->index].room = &room_list[i];
                    client_list[arg->index].host = 1;
                    break;
                  
                }
            }
            cleanToken(token, 2);
            printf("[+]%s is hosting room '%s' max player is '%d'!\n", client_list[arg->index].name, client_list[arg->index].room->room_id, client_list[arg->index].room->max_client);
            printf("Rank of room : %d\n", client_list[arg->index].room->room_rank);
            strcpy(token[0], "OK!");
            buffer = MakeMessage(token, 1, SUCCEED_PACK);
            cleanToken(token, 1);
            sendToClient(sockfd, arg->client, buffer);
        }
        // GetRoom(arg->index);
        break;
    case FIND_ROOM:
        for(i = 0; i < MAX_ROOM; i++){
            list[i] = -1;
        }
        list = countValidRoom(&validRoom , arg->index);
        printf("So phong : %d\n", validRoom);
        sprintf(token[0], "%d", validRoom);
        buffer = MakeMessage(token, 1, FIND_ROOM);
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
        buffer = MakeMessage(token, j, HOST_GAME);
        cleanToken(token, j);
        sendToClient(sockfd, arg->client, buffer);
        break; 
    case JOIN_ROOM: 
        token = GetToken(arg->message, 2);
        printf(" BUFF : %s\n", arg->message);
        j = checkRoom(token[1]);
        if(j != -1){
            if(room_list[j].client_count < 2 && room_list[j].isStart != 1){
                addPlayer(&room_list[j], arg->index);
                cleanToken(token, 2);
                strcpy(token[0], "OK");
                buffer = MakeMessage(token, 1, SUCCEED_PACK);
                printf("BUFF2 : %s\n", buffer);
                cleanToken(token, 2);
                sendToClient(sockfd, client_list[arg->index].client, buffer);
                printf("Gui OK %s \n", buffer);
                // for(i = 0; i < client_list[index].room->max_client; i++){
                //     if(client_list[index].room->player_id[i] != 0){
                //         GetRoom(client_list[index].room->player_id[i] - 1);
                //     }
                // }
            }
        }else{
            cleanToken(token, 4);
            strcpy(token[0], "Room is already full!");
            buffer = MakeMessage(token, 2, ERROR_PACK);
            cleanToken(token, 2);
            sendToClient(sockfd, client_list[index].client, buffer);
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
                updateClient(index, client_addr);
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
            
        }else{
            for (i = 0; i < MAX_CLIENT; i++){
                if ((client_list[i].client.sin_addr.s_addr == client_addr.sin_addr.s_addr) && (client_list[i].client.sin_port == client_addr.sin_port)){
                    Argument *arg = getArument(i, buffer, client_addr);
                    pthread_create(&tid, NULL, &clientHandle, (void*)arg);
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
    // printf("%s\n",client_list[1].name);
    // printf("%d\n",client_list[1].rank_point);
    room_list = calloc(MAX_ROOM, sizeof(Room));
	for(int i = 0; i < MAX_ROOM; i++){
		room_list[i].room_id = calloc(MAX_NAME, sizeof(char));
		room_list[i].client_count = 0;
        room_list[i].room_rank = -1;
        room_list[i].isStart = -1;
	}

    if(setupServer() == 1){
        printf("[+]Server is now online!\n");
        startServer();
    }
}