#include <netinet/in.h>

#ifndef MY_SOCKET_H
#define MY_SOCKET_H

struct socket {
    int descriptor;
    struct sockaddr_in addr;
};

void setupClientSocket(struct socket *clientSock, char ip[], int port,char logName[]);
void setupServerSocket(struct socket *serverSock, int port, int backLog, char logName[]);

#endif //MY_SOCKET_H
