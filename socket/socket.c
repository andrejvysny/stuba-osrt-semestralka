#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socket.h"


void setupClientSocket(struct socket *clientSock, char ip[], int port, char logName[]){


    clientSock->descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSock->descriptor < 0){
        printf("[-]Error in connection.\n");
        exit(EXIT_FAILURE);
    }
    printf("[+]Client Socket is created.\n");

    memset(&clientSock->addr, 0, sizeof(clientSock->addr));
    clientSock->addr.sin_family = AF_INET;
    clientSock->addr.sin_port = htons(port);
    clientSock->addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    if(connect(clientSock->descriptor, (struct sockaddr*)&clientSock->addr, sizeof(clientSock->addr)) < 0){
        printf("[-]Error in connection.\n");
        close(clientSock->descriptor);
        exit(EXIT_FAILURE);
    }else{
        printf("[+]Client connected to server.\n");
    }

}



void setupServerSocket(struct socket *serverSock, int port, char logName[]){

    serverSock->descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSock->descriptor < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Server Socket is created.\n");

    memset(&serverSock->addr, 0, sizeof(serverSock->addr));
    serverSock->addr.sin_family = AF_INET;
    serverSock->addr.sin_port = htons(port);
    serverSock->addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSock->descriptor, (struct sockaddr*)&serverSock->addr, sizeof(serverSock->addr)) < 0){
        printf("[-]Error in binding.\n");
        exit(1);
    }
    printf("[+]Bind to port %d\n", 4444);

    if(listen(serverSock->descriptor, 10) == 0){
        printf("[+]Listening....\n");
    }else{
        printf("[-]Error in binding.\n");
    }

}


