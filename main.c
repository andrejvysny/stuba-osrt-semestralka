#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>


#include "clients/clients.h"
#include "logger/logger.h"
#include "server/server.h"
#include "config.h"


int main() {

/*
    int shmId;
    struct process_pids *pids;

    shmId = shmget((key_t)SHM_PIDS_KEY, sizeof(struct process_pids), IPC_CREAT | 0666);

    if(shmId < 0){
        logMessage("RUNNER", "Shared memory failed!",-1);
        exit(EXIT_FAILURE);
    }

    pids = shmat(shmId, NULL, 0);
*/

    int pipe_fd[2];

    pipe(pipe_fd);

    struct process_pids pids;


    if ((pids.server_pid = fork()) < 0) {
        logMessage("RUNNER", "Error while creating server process", -1);
        exit(1);
    }

    if (pids.server_pid != 0) {
        if ((pids.client1_pid = fork()) < 0) {
            logMessage("RUNNER", "Error while creating client 1 process", -1);
            exit(1);
        }
    }

    if (pids.server_pid != 0 &&  pids.client1_pid != 0) {
        if ((pids.client2_pid = fork()) < 0) {
            logMessage("RUNNER", "Error while creating client 2 process", -1);
            exit(1);
        }
    }


    if (pids.server_pid != 0 &&  pids.client1_pid != 0 && pids.client2_pid != 0) {
        if ((pids.client3_pid = fork()) < 0) {
            logMessage("RUNNER", "Error while creating client 3 process", -1);
            exit(1);
        }
    }


    // SERVER
    if (pids.server_pid == 0) {
        if (prctl(PR_SET_NAME, (unsigned long) "server") < 0)
            logMessage("RUNNER", "ERROR while naming process server",-1);
        logMessage("RUNNER", "Server process created!", 0);
        runServer();
    }


    //RUNNER CODE
    if (pids.server_pid != 0 && pids.client1_pid != 0 && pids.client2_pid != 0 && pids.client3_pid != 0 &&pids.client4_pid != 0){
        int pid = pids.server_pid;
        logMessage("RUNNER", "Runner sending PID to PIPE",10);
        write(pipe_fd[1], &pid, sizeof(pid));
    }

    sleep(1);

    if (pids.client1_pid == 0 && pids.server_pid != 0) {
        if (prctl(PR_SET_NAME, (unsigned long) "client_1") < 0)
            logMessage("RUNNER", "ERROR while naming process client 1",-1);

        logMessage("CLIENT_1", "CLIENT 1 receiving",0);
        int pid_buffer;
        printf("Init read\n\n");
        int rec = (int) read(pipe_fd[0],&pid_buffer,sizeof (pid_buffer));
        printf("RUNNER CLIENT_1 RECEIVED: %d .... ID: %d", rec, pid_buffer);

        /*
        logMessage("RUNNER", "Client 1 process created!", 0);
        runClient1();

        logMessage("RUNNER", "Sending signal to kill server!", 0);

        kill(pid_buffer, SIGKILL);
*/
    }

    /*
    if (pids.client2_pid == 0 && pids.server_pid != 0 && pids.client1_pid != 0) {
        if (prctl(PR_SET_NAME, (unsigned long) "client_2") < 0)
            logMessage("RUNNER", "ERROR while naming process client 2",-1);
        logMessage("RUNNER", "Client 2 process created!", 0);
        runClient2();
    }

    if (pids.client3_pid == 0 && pids.server_pid != 0 && pids.client1_pid != 0 && pids.client2_pid != 0) {
        if (prctl(PR_SET_NAME, (unsigned long) "client_3") < 0)
            logMessage("RUNNER", "ERROR while naming process client 3",-1);
        logMessage("RUNNER", "Client 3 process created!", 0);
        runClient3();
    }


    close(pipe_fd[0]);
    close(pipe_fd[1]);*/

    exit(0);

    return 0;
}




