#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/shm.h>
#include <math.h>
#include <stdlib.h>

#include "../logger/logger.h"
#include "../socket/socket.h"
#include "../server/server.h"
#include "../config.h"
#include "clients.h"

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

void sendUSR1(int pid, char name[]){
    logMessage(name,"Sending SIGUSR1",COLOR_WHITE);
    kill(pid,SIGUSR1);
}


int intToBuffer(int number, char *buffer, int offset){
    int nDigits = floor(log10(abs(number))) + 1;
    if(number < 0) nDigits++;
    gcvt(number, nDigits + 1 , buffer + offset);
}

int floatToBuffer(float number, char *buffer, int offset){
    int nDigits = floor(log10(abs((int)number))) + 1;
    if(number < 0) nDigits++;
    gcvt(number, nDigits + 3 , buffer + offset);
}


void runClient1(){
    char name[10] = "CLIENT_1";
    logMessage(name,"Running",COLOR_GREEN);

    struct runtime *shm_runtime_ptr;
    shm_runtime_ptr = getRuntimeData();
    shm_runtime_ptr->clients_up++;

    char buffer[CHAR_BUFFER_SIZE] = { 0 };
    struct socket sock;

    setupClientSocket(&sock, SERVER_IP,SERVER_PORT, name);

    buffer[0] = (char) SERVER_ACTION_GET_FILE;
    buffer[1] = '\0';
    send(sock.descriptor, buffer, strlen(buffer), 0);
    recv(sock.descriptor, buffer, CHAR_BUFFER_SIZE, 0);

    int matrix[100][100];
    loadData(buffer, matrix);
    float avg = getAverageMatrix(matrix);

    bzero(buffer, CHAR_BUFFER_SIZE);
    buffer[0] = (char) SERVER_ACTION_SAVE_RESULT;
    strcpy(buffer + sizeof (char), "Average:");
    floatToBuffer(avg,buffer,(int) strlen(buffer));
    buffer[(int) strlen(buffer)] = '\0';

    logMessage(name,"Sending AVG to server.", COLOR_INFO);

    send(sock.descriptor, buffer, strlen(buffer), 0);
    recv(sock.descriptor, buffer, CHAR_BUFFER_SIZE, 0);

    close(sock.descriptor);
    shm_runtime_ptr->clients_up--;


    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}

void runClient2(){
    char name[10] = "CLIENT_2";
    logMessage(name,"Running",COLOR_GREEN);

    struct runtime *shm_runtime_ptr;
    shm_runtime_ptr = getRuntimeData();
    shm_runtime_ptr->clients_up++;

    char buffer[CHAR_BUFFER_SIZE] = { 0 };
    struct socket sock;

    setupClientSocket(&sock, SERVER_IP,SERVER_PORT, name);

    buffer[0] = (char) SERVER_ACTION_GET_FILE;
    buffer[1] = '\0';
    send(sock.descriptor, buffer, strlen(buffer), 0);
    recv(sock.descriptor, buffer, CHAR_BUFFER_SIZE, 0);

    int matrix[100][100];
    loadData(buffer, matrix);
    int sum = getSumMatrix(matrix);

    bzero(buffer, CHAR_BUFFER_SIZE);
    buffer[0] = (char) SERVER_ACTION_SAVE_RESULT;
    strcpy(buffer + sizeof (char), "Sum:");
    intToBuffer(sum,buffer,(int) strlen(buffer));
    buffer[(int) strlen(buffer)] = '\0';

    logMessage(name,"Sending SUM to server.", COLOR_INFO);

    send(sock.descriptor, buffer, strlen(buffer), 0);
    recv(sock.descriptor, buffer, CHAR_BUFFER_SIZE, 0);

    close(sock.descriptor);
    shm_runtime_ptr->clients_up--;


    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}

void runClient3(){return 0;
    char name[10] = "CLIENT_3";
    logMessage(name,"Running",COLOR_GREEN);
/*
    struct runtime *shm_runtime_ptr;
    shm_runtime_ptr = getRuntimeData();
    shm_runtime_ptr->clients_up++;

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
*/
    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}

void runClient4(){return 0;
    char name[10] = "CLIENT_4";
    logMessage(name,"Running",COLOR_GREEN);
/*
    struct runtime *shm_runtime_ptr;
    shm_runtime_ptr = getRuntimeData();
    shm_runtime_ptr->clients_up++;

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
*/
    logMessage(name, "Exiting", COLOR_YELLOW);
    return 0;
}