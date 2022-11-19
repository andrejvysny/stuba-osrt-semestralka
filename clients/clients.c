#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/shm.h>

#include "../logger/logger.h"
#include "../socket/socket.h"
#include "../server/server.h"
#include "../config.h"
#include "clients.h"

int connectAndGetFile(){
    char name[10] = "TEST";

    logMessage(name,"Running",COLOR_GREEN);

    char buffer[CHAR_BUFFER_SIZE] = { 0 };
    struct socket sock;
    sock = setupClientSocket(SERVER_IP, SERVER_PORT);
    connectToServer(sock);
    logMessage(name,"Connected to server",COLOR_GREEN);

    buffer[0] = (char) SERVER_ACTION_GET_FILE;
    send(sock.descriptor, buffer, strlen(buffer), 0);

    recv(sock.descriptor, buffer, CHAR_BUFFER_SIZE, 0);
    logMessage(name, buffer, COLOR_WHITE);

    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}

void runClient1(){
    char name[10] = "CLIENT_1";

    logMessage(name,"Running",COLOR_GREEN);

    struct runtime *shm_runtime_ptr;
    shm_runtime_ptr = getRuntimeData();

    printf("\n\tCLIENT_1: my pid: %d\n", shm_runtime_ptr->client1_pid);


    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}

void runClient2(){
    char name[10] = "CLIENT_2";

    logMessage(name,"Running",COLOR_GREEN);

    struct runtime *shm_runtime_ptr;
    shm_runtime_ptr = getRuntimeData();

    printf("\n\tCLIENT_2: my pid: %d\n", shm_runtime_ptr->client2_pid);



    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}

void runClient3(){
    char name[10] = "CLIENT_3";

    logMessage(name,"Running",COLOR_GREEN);

    struct runtime *shm_runtime_ptr;
    shm_runtime_ptr = getRuntimeData();

    printf("\n\tCLIENT_3: my pid: %d\n", shm_runtime_ptr->client3_pid);

    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}

void runClient4(){
    char name[10] = "CLIENT_4";

    logMessage(name,"Running",COLOR_GREEN);

    struct runtime *shm_runtime_ptr;
    shm_runtime_ptr = getRuntimeData();

    printf("\n\tCLIENT_4: my pid: %d\n", shm_runtime_ptr->client4_pid);

    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}