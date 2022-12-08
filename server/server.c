#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/prctl.h>
#include <time.h>
#include <pthread.h>

#include "../logger/logger.h"
#include "../socket/socket.h"
#include "../config.h"
#include "server.h"

#define PROCESS_NAME_SERVER "SERVER"

void handleServerAction(int sock_desc, char action, char *buffer);
char getAction(char *buffer);
char *getData(char *buffer);
void appendToFile(char *message);


struct runtime* getRuntimeData();
struct runtime *shm_runtime_ptr;
struct socket serverSock;
int connection = 0;
int second = 0;
timer_t timer;

timer_t createTimer(int signal)
{
    struct sigevent e;
    e.sigev_notify=SIGEV_SIGNAL;
    e.sigev_signo=signal;
    timer_t timer;
    timer_create(CLOCK_REALTIME, &e, &timer);
    return timer;
}

void setTimer(timer_t timer, int s)
{
    struct itimerspec time;
    time.it_value.tv_sec=s;
    time.it_value.tv_nsec=0;
    time.it_interval.tv_sec=0;
    time.it_interval.tv_nsec=0;
    timer_settime(timer,CLOCK_REALTIME,&time,NULL);
}

void signal_timer(){
    second++;
    signal(SIGUSR2,signal_timer);
    printf("\tSERVER: Number of connections in %ds : %d\n",second, connection);
    setTimer(timer,1);
}

int getRuntimeShmId(){
    key_t mem_key;

    if ((mem_key = ftok(SHM_RUNTIME_LOC, SHM_RUNTIME_KEY)) == (key_t) -1) {
        logMessage("SHM","IPC error: ftok",-1);
        exit(EXIT_FAILURE);
    }

    return shmget(mem_key, sizeof(struct runtime), 0666);
}

void stopServer(){
    logMessage("SERVER", "Stopping server",COLOR_YELLOW);

    shmctl(getRuntimeShmId(), IPC_RMID, NULL);
    close(serverSock.descriptor);
    exit(EXIT_SUCCESS);
}

void signal_client_down()
{
    signal(SIGUSR1, signal_client_down);
    shm_runtime_ptr->clients_up--;

    if(shm_runtime_ptr->clients_up == 0){
        logMessage("SERVER", "All clients down",COLOR_YELLOW);
        stopServer();
    }
}

int runServer()
{
    logMessage(PROCESS_NAME_SERVER, "Server Running", COLOR_GREEN);


    timer = createTimer(SIGUSR2);
    setTimer(timer,1);

    signal(SIGUSR1, signal_client_down);
    signal(SIGUSR2,signal_timer);

    shm_runtime_ptr = getRuntimeData();

    int tmpFd;
    struct sockaddr_in tmpAddr;
    socklen_t tmpAddrLen;

    size_t readBytes;

    pid_t handlerPid;
    char buffer[CHAR_BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));

    setupServerSocket(&serverSock, SERVER_PORT, SERVER_BACKLOG, PROCESS_NAME_SERVER);

    while(1){

        tmpFd = accept(serverSock.descriptor, (struct sockaddr*) &tmpAddr, &tmpAddrLen);
        connection++;
        if(tmpFd < 0){
            logMessage(PROCESS_NAME_SERVER,"Error while accepting client!",COLOR_RED);
            exit(EXIT_FAILURE);
        }

        if((handlerPid = fork()) == 0){
            if (prctl(PR_SET_NAME, (unsigned long) "server_handler") < 0)
                logMessage(PROCESS_NAME_SERVER, "ERROR while naming process server handler.", COLOR_RED);

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
        }else{
            if(shm_runtime_ptr->clients_up == 0){
                logMessage(PROCESS_NAME_SERVER, "Stopping server, clients are down.",COLOR_YELLOW);
                break;
            }
        }

    }

    if(handlerPid != 0){
        logMessage(PROCESS_NAME_SERVER, "Stopping server", COLOR_YELLOW);
        close(serverSock.descriptor);
    }
    return 0;
}

void appendToFile(char *message)
{
    FILE *fr;
    if((fr = fopen(OUTPUT_FILE,"a")) == NULL){
        return 0;
    }
    fprintf(fr, "%s\n", message);
    fclose(fr);
}


char getAction(char *buffer){
    return (char) buffer[0];
}

char *getData(char *buffer){
    return buffer + sizeof (char);
}

void handleServerAction(int sock_desc, char action, char *buffer){
    pthread_t tid;
    switch (action)
    {
        case SERVER_ACTION_GET_FILE:
            strcpy(buffer, INPUT_FILE);
            break;

        case SERVER_ACTION_SAVE_RESULT:
            pthread_create(&tid, NULL, (void *) appendToFile, getData(buffer));
            logMessage(PROCESS_NAME_SERVER,"Writing to output file!",COLOR_INFO);
            pthread_join(tid, NULL);

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

    struct runtime *shm_ptr;


    shm_ptr = (struct runtime*) shmat(getRuntimeShmId(), NULL, 0);

    return shm_ptr;
}


