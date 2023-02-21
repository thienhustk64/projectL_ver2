#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    char str[100];
    strcpy(str, "1||1|2");
    char *token ;
    token = strtok(str,"|");
    while(token != NULL){
        printf("%s\n",token);
        token = strtok(NULL,"|");
    }
}