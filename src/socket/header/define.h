#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERROR_RETURN -1
#define SUCCEED_RETURN 1

#define MAX 50
#define MAX_NAME 20 // max character of player name
#define MAX_RNAME 30
#define MAX_MESSAGE 1000 // max character of message
#define MAX_ROOM 20
#define MAX_CLIENT 100

#define PORT 27015 //defaut port to server listen
#define PPORT 27016 //default port to get ping
#define SERVER_IP_ADDR "127.0.0.1" //the ip address of server
