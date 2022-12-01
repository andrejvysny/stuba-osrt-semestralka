#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>

#include "../logger/logger.h"
#include "../socket/socket.h"
#include "../config.h"
#include "server.h"

#define PROCESS_NAME_SERVER "SERVER"

void handleServerAction(int sock_desc, char action, char *buffer);
char getAction(char *buffer);
char *getData(char *buffer);
void appendToFile(char *message);

void signal_kill();



int runServer()
{
    logMessage(PROCESS_NAME_SERVER, "Server Running", COLOR_GREEN);

    struct socket serverSock;

    int tmpFd;
    struct sockaddr_in tmpAddr;
    socklen_t tmpAddrLen;

    size_t readBytes;

    pid_t handlerPid;
    char buffer[CHAR_BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));

    setupServerSocket(&serverSock, SERVER_PORT, 5, PROCESS_NAME_SERVER);

    while(1){
        tmpFd = accept(serverSock.descriptor, (struct sockaddr*) &tmpAddr, &tmpAddrLen);
        if(tmpFd < 0){
            logMessage(PROCESS_NAME_SERVER,"Error while accepting client!",COLOR_RED);
            exit(EXIT_FAILURE);
        }

        if((handlerPid = fork()) == 0){
            close(serverSock.descriptor);

            while(1){
                readBytes = recv(tmpFd, buffer, CHAR_BUFFER_SIZE, 0);

                if(readBytes > 0){
                    handleServerAction(tmpFd, getAction(buffer), buffer);

                }else if (getAction(buffer) == SERVER_ACTION_EXIT || readBytes == 0){
                    logMessage(PROCESS_NAME_SERVER, "Client disconnected.",COLOR_YELLOW);
                    break;
                }else{
                    logMessage(PROCESS_NAME_SERVER, "Error while reading from client!",COLOR_RED);
                    break;
                }
            }

            logMessage(PROCESS_NAME_SERVER, "Stopping client response process", COLOR_YELLOW);
            close(tmpFd);
            exit(EXIT_SUCCESS);
        }

    }

    if(handlerPid != 0){
        logMessage(PROCESS_NAME_SERVER, "Stopping server", COLOR_YELLOW);
        close(serverSock.descriptor);
    }
    return 0;
}




char getAction(char *buffer){
    return (char) buffer[0];
}

char *getData(char *buffer){
    return buffer + sizeof (char);
}

void handleServerAction(int sock_desc, char action, char *buffer){

    switch (action)
    {
        case SERVER_ACTION_GET_FILE:
            strcpy(buffer, INPUT_FILE);
            break;

        case SERVER_ACTION_SAVE_RESULT:
            appendToFile(getData(buffer));
            strcpy(buffer, "DONE");
            break;

        default:
            strcpy(buffer, "Invalid action");
            break;
    }

    send(sock_desc, buffer, strlen(buffer), 0);
    bzero(buffer, CHAR_BUFFER_SIZE);

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

void signal_kill()
{
    signal(SIGUSR1,signal_kill);

    logMessage(PROCESS_NAME_SERVER, "Received SIGUSR1 signal", COLOR_INFO);
    logMessage(PROCESS_NAME_SERVER,"Killing server", COLOR_YELLOW);
    exit(0);
}

void appendToFile(char *message)
{
    logMessage(PROCESS_NAME_SERVER,"Writing to output file!",COLOR_INFO);
    FILE *fr;
    if((fr = fopen(OUTPUT_FILE,"a")) == NULL){
        return 0;
    }
    fprintf(fr, "%s\n", message);
    fclose(fr);
}