#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../logger/logger.h"
#include "socket.h"

int generateSocket(){
    // vytvorenie socketu
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc < 0)
    {
        logMessage("SYSTEM","Cannot create socket!", -1);
        exit(1);
    }
    return sock_desc;
}

struct socket setupClientSocket(char ip[], int port){

    struct socket socket;
    socket.descriptor = generateSocket();

    // nastavenie socketu
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(ip);
    client.sin_port = htons(port);

    socket.addr = client;

    return socket;
}

void connectToServer(struct socket config){

    if (connect(config.descriptor, (struct sockaddr*)&config.addr, sizeof(config.addr)) < 0)
    {
        logMessage("CLIENT_1","Cannot connect to server!",-1);
        close(config.descriptor);
    }
}