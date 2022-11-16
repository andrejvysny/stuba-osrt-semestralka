
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../logger/logger.h"
#include "../socket/socket.h"
#include "../config.h"
#include "server.h"


void setupServerSocket(struct socket *sock);


int runServer()
{
    struct socket sock;
    setupServerSocket(&sock);

    int temp_sock_desc;
    ssize_t success_read;
    char buffer[1024];

    logMessage("SERVER", "server running", 0);

    while(1){
        logMessage("SERVER", "listening...\n", 4);

        temp_sock_desc = accept(sock.descriptor, (struct sockaddr*)&sock.addr, (socklen_t*)&sock.addr);
        if (temp_sock_desc < 0)
        {
            logMessage("SERVER", "Accept failed!",-1);
            close(sock.descriptor);
            exit(EXIT_FAILURE);
        }

        success_read = recv(temp_sock_desc, buffer, 1024, 0);
        if(success_read == 0){
            logMessage("SERVER", "Server killed",10);

            break;
        }

        printf("SERVER RECEIVED: %s\n", buffer);
        sleep(1);

        send(temp_sock_desc, "server response", strlen("server response"), 0);

    }

   // int shmId = shmget((key_t)SHM_PIDS_KEY, sizeof(struct process_pids), IPC_CREAT | 0666);

    //shmctl(shmId, IPC_RMID, NULL);
    close(temp_sock_desc);
    close(sock.descriptor);
    return 0;
}



void setupServerSocket(struct socket *sock){

    sock->descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sock->descriptor == -1)
    {
        printf("cannot create socket!\n");
        exit(EXIT_FAILURE);
    }

    memset(&sock->addr, 0, sizeof(sock->addr));
    sock->addr.sin_family = AF_INET;
    sock->addr.sin_addr.s_addr = INADDR_ANY;
    sock->addr.sin_port = htons(SERVER_PORT);
    if (bind(sock->descriptor, (struct sockaddr*)&sock->addr, sizeof(sock->addr)) < 0)
    {
        logMessage("SERVER", "Server cannot bind socket!", -1);
        close(sock->descriptor);
        exit(EXIT_FAILURE);
    }


    if (listen(sock->descriptor, SERVER_BACKLOG) < 0)
    {
        logMessage("SERVER", "Cannot listen on socket!", -1);
        close(sock->descriptor);
        exit(EXIT_FAILURE);
    }

}