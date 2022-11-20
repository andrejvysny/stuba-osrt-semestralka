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

//int connectAndGetFile(){
//    char name[10] = "TEST";
//
//    logMessage(name,"Running",COLOR_GREEN);
//
//    char buffer[CHAR_BUFFER_SIZE] = { 0 };
//    struct socket sock;
//    sock = setupClientSocket(SERVER_IP, SERVER_PORT);
//    connectToServer(sock);
//    logMessage(name,"Connected to server",COLOR_GREEN);
//
//    buffer[0] = (char) SERVER_ACTION_GET_FILE;
//    buffer[1] = '\0';
//    send(sock.descriptor, buffer, strlen(buffer), 0);
//
//    recv(sock.descriptor, buffer, CHAR_BUFFER_SIZE, 0);
//
//    debugBuffer(buffer,20);
//
//    logMessage(name, buffer, COLOR_WHITE);
//
//    logMessage(name, "Exiting", COLOR_YELLOW);
//    return 0;
//}


void loadData(char *file, int matrix[100][100]){

    FILE *fr;
    if((fr = fopen(file,"r")) == NULL){
        return 0;
    }
    int r=0,c=0;
    while (fscanf(fr, "%d",&matrix[r][c]) != EOF) {
        c++;
        if(c==100&& r<99){
            r++;
            c=0;
        }
        if(c==100&&r==99){
            break;
        }
    }
    fclose(fr);
}


void runClient1(){
    char name[10] = "CLIENT_1";
    logMessage(name,"Running",COLOR_GREEN);

    char buffer[CHAR_BUFFER_SIZE] = { 0 };
    struct socket sock;
    sock = setupClientSocket(SERVER_IP, SERVER_PORT);
    connectToServer(sock);
    logMessage(name,"Connected to server",COLOR_GREEN);

    buffer[0] = (char) SERVER_ACTION_GET_FILE;
    buffer[1] = '\0';
    send(sock.descriptor, buffer, strlen(buffer), 0);
    recv(sock.descriptor, buffer, CHAR_BUFFER_SIZE, 0);

    int matrix[100][100];
    loadData(buffer, matrix);
    float avg = getAverageMatrix(matrix);
    close(sock.descriptor);

    printf("\tCLIENT_1: Avg Matrix: %f\n",avg);

    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}

void runClient2(){
    char name[10] = "CLIENT_2";
    logMessage(name,"Running",COLOR_GREEN);

    char buffer[CHAR_BUFFER_SIZE] = { 0 };
    struct socket sock;
    sock = setupClientSocket(SERVER_IP, SERVER_PORT);
    connectToServer(sock);
    logMessage(name,"Connected to server",COLOR_GREEN);

    buffer[0] = (char) SERVER_ACTION_GET_FILE;
    buffer[1] = '\0';
    send(sock.descriptor, buffer, strlen(buffer), 0);
    recv(sock.descriptor, buffer, CHAR_BUFFER_SIZE, 0);

    int matrix[100][100];
    loadData(buffer, matrix);
    int sum = getSumMatrix(matrix);
    close(sock.descriptor);

    printf("\tCLIENT_2: Sum Matrix: %d\n",sum);

    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}

void runClient3(){
    char name[10] = "CLIENT_3";
    logMessage(name,"Running",COLOR_GREEN);

    char buffer[CHAR_BUFFER_SIZE] = { 0 };
    struct socket sock;
    sock = setupClientSocket(SERVER_IP, SERVER_PORT);
    connectToServer(sock);
    logMessage(name,"Connected to server",COLOR_GREEN);

    buffer[0] = (char) SERVER_ACTION_GET_FILE;
    buffer[1] = '\0';
    send(sock.descriptor, buffer, strlen(buffer), 0);
    recv(sock.descriptor, buffer, CHAR_BUFFER_SIZE, 0);

    int matrix[100][100];
    loadData(buffer, matrix);
    int min = getMinMatrix(matrix);
    close(sock.descriptor);

    printf("\tCLIENT_3: Min Matrix: %d\n",min);

    // struct runtime *shm_runtime_ptr;
    //shm_runtime_ptr = getRuntimeData();

    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}

void runClient4(){
    char name[10] = "CLIENT_4";
    logMessage(name,"Running",COLOR_GREEN);

    char buffer[CHAR_BUFFER_SIZE] = { 0 };
    struct socket sock;
    sock = setupClientSocket(SERVER_IP, SERVER_PORT);
    connectToServer(sock);
    logMessage(name,"Connected to server",COLOR_GREEN);

    buffer[0] = (char) SERVER_ACTION_GET_FILE;
    buffer[1] = '\0';
    send(sock.descriptor, buffer, strlen(buffer), 0);
    recv(sock.descriptor, buffer, CHAR_BUFFER_SIZE, 0);

    int matrix[100][100];
    loadData(buffer, matrix);
    int max = getMaxMatrix(matrix);
    close(sock.descriptor);

    printf("\tCLIENT_4: Max Matrix: %d\n",max);

    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}