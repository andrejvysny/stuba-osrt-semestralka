#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socket.h"
#include "../logger/logger.h"

void setupClientSocket(struct socket *clientSock, char ip[], int port, char logName[]){

    clientSock->descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSock->descriptor < 0){
        logMessage(logName, "Error while socket init.", COLOR_RED);
        exit(EXIT_FAILURE);
    }

    memset(&clientSock->addr, 0, sizeof(clientSock->addr));
    clientSock->addr.sin_family = AF_INET;
    clientSock->addr.sin_port = htons(port);
    clientSock->addr.sin_addr.s_addr = inet_addr(ip);


    if(connect(clientSock->descriptor, (struct sockaddr*)&clientSock->addr, sizeof(clientSock->addr)) < 0){
        logMessage(logName, "Error in connection.", COLOR_RED);
        close(clientSock->descriptor);
        exit(EXIT_FAILURE);
    }else{
        logMessage(logName, "Connected to server.", COLOR_GREEN);
    }

}


void setupServerSocket(struct socket *serverSock, int port, int backLog, char logName[]){

    serverSock->descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSock->descriptor < 0){
        logMessage(logName, "Error in connection.", COLOR_RED);
        exit(EXIT_FAILURE);
    }

    memset(&serverSock->addr, 0, sizeof(serverSock->addr));
    serverSock->addr.sin_family = AF_INET;
    serverSock->addr.sin_port = htons(port);
    serverSock->addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSock->descriptor, (struct sockaddr*)&serverSock->addr, sizeof(serverSock->addr)) < 0){
        logMessage(logName, "Error in binding.", COLOR_RED);
        exit(EXIT_FAILURE);
    }

    if(listen(serverSock->descriptor, backLog) != 0){
        logMessage(logName, "Error in binding (listen).", COLOR_RED);
    }

    logMessage(logName, "Listening...",COLOR_INFO);

}
