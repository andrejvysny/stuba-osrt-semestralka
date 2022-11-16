#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>


#include "clients.h"
#include "../logger/logger.h"
#include "../socket/socket.h"
#include "../config.h"

//TODO: load from shared memory

struct process_pids pids;

void runClient1(){
    logMessage("CLIENT_1","Client 1 Running",0);

    char* hello = "Message from Client 1 ###";
    char buffer[1024] = { 0 };

    struct socket sock;
    sock = setupClientSocket(SERVER_IP, SERVER_PORT);

    connectToServer(sock);


    logMessage("CLIENT_1", "Sending after 10 secs",10);
    sleep(10);
    send(sock.descriptor, hello, strlen(hello), 0);

    sleep(2);

    recv(sock.descriptor, buffer, 1024, 0);

    logMessage("CLIENT_1", buffer, 10);
    logMessage("CLIENT_1", "Killing server!", 10);

    //kill(pids.server_pid, SIGKILL);

    close(sock.descriptor);

    logMessage("CLIENT_1", "ENDING",10);
}


void runClient2(){
    logMessage("CLIENT_2","Client 2 Running",0);

    sleep(10);
    logMessage("CLIENT_1", "ENDING",10);

}

void runClient3(){
    logMessage("CLIENT_3","Client 3 Running",0);

}

void runClient4(){
    logMessage("CLIENT_4","Client 4 Running",0);

}
