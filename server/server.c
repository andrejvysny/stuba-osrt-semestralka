#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "../logger/logger.h"
#include "../socket/socket.h"
#include "../config.h"
#include "server.h"

void setupServerSocket(struct socket *sock);
void handleServerAction(int action, char *buffer);
int getAction(char *buffer);
char *getData(char *buffer);

int runServer()
{
    char name[7] = "SERVER\0";
    logMessage(name, "Server Running", COLOR_GREEN);

    struct socket sock;
    setupServerSocket(&sock);
    char buffer[CHAR_BUFFER_SIZE] = { 0 };

    int temp_sock_desc;
    ssize_t success_read;

    while(1){
        logMessage(name, "listening...\n", COLOR_INFO);

        temp_sock_desc = accept(sock.descriptor, (struct sockaddr*)&sock.addr, (socklen_t*)&sock.addr);
        if (temp_sock_desc < 0)
        {
            logMessage(name, "Accept failed!",COLOR_RED);
            close(sock.descriptor);
            exit(EXIT_FAILURE);
        }

        success_read = recv(temp_sock_desc, buffer, CHAR_BUFFER_SIZE, 0);
        if(success_read == 0){
            logMessage(name, "Client disconnected.",COLOR_RED);
            close(sock.descriptor);
            close(temp_sock_desc);
            exit(EXIT_FAILURE);
        }
        if(success_read == -1){
            logMessage(name, "Cannot read from client!",COLOR_RED);
            close(sock.descriptor);
            close(temp_sock_desc);
            exit(EXIT_FAILURE);
        }

        if(strcmp(buffer,"exit")){
            break;
        }

        handleServerAction(getAction(buffer), buffer);
        send(temp_sock_desc, buffer,strlen(buffer), 0);
        close(temp_sock_desc);
    }

    logMessage(name, "Stopping server", COLOR_YELLOW);
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

int getAction(char *buffer){
    return (int) buffer[0];
}

char *getData(char *buffer){
    return buffer;
}

void handleServerAction(int action, char *buffer){

    switch (action)
    {
        case  SERVER_ACTION_GET_FILE:
            strcpy(buffer, INPUT_FILE);
            break;

        default:
            strcpy(buffer, "Invalid action");
            break;
    }

}

struct runtime* getRuntimeData(){

    int shm_id;
    key_t mem_key;
    struct runtime *shm_runtime_ptr;

    if ((mem_key = ftok(SHM_RUNTIME_LOC, SHM_RUNTIME_KEY)) == (key_t) -1) {
        logMessage("SHM","IPC error: ftok",-1);
        exit(1);
    }

    shm_id = shmget(mem_key, sizeof(struct runtime), 0666);
    shm_runtime_ptr = (struct runtime*) shmat(shm_id, NULL, 0);

    return shm_runtime_ptr;
}
