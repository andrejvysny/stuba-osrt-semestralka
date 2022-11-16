#include <netinet/in.h>

#ifndef OSRT_SEMESTRALKA_SOCKET_H
#define OSRT_SEMESTRALKA_SOCKET_H

int generateSocket();

struct socket setupClientSocket(char ip[], int port);

void connectToServer(struct socket config);

struct socket {
    int descriptor;
    struct sockaddr_in addr;
};

#endif //OSRT_SEMESTRALKA_SOCKET_H
