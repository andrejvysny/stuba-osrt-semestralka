#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>

#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>

#include "server/server.h"
#include "logger/logger.h"
#include "clients/clients.h"
#include "config.h"


int main() {

    struct runtime runtime_d;

    if ((runtime_d.server_pid = fork()) < 0) {
        logMessage("RUNNER", "Error while creating server process", COLOR_RED);
        exit(EXIT_FAILURE);
    }

    if (runtime_d.server_pid != 0) {
        if ((runtime_d.client1_pid = fork()) < 0) {
            logMessage("RUNNER", "Error while creating client 1 process", COLOR_RED);
            exit(EXIT_FAILURE);
        }
    }

    if (runtime_d.server_pid != 0 && runtime_d.client1_pid != 0) {
        if ((runtime_d.client2_pid = fork()) < 0) {
            logMessage("RUNNER", "Error while creating client 2 process", COLOR_RED);
            exit(EXIT_FAILURE);
        }
    }

    if (runtime_d.server_pid != 0 && runtime_d.client1_pid != 0 && runtime_d.client2_pid != 0) {
        if ((runtime_d.client3_pid = fork()) < 0) {
            logMessage("RUNNER", "Error while creating client 3 process", COLOR_RED);
            exit(EXIT_FAILURE);
        }
    }

    if (runtime_d.server_pid != 0 && runtime_d.client1_pid != 0 && runtime_d.client2_pid != 0 && runtime_d.client3_pid != 0) {
        if ((runtime_d.client4_pid = fork()) < 0) {
            logMessage("RUNNER", "Error while creating client 4 process", COLOR_RED);
            exit(EXIT_FAILURE);
        }
    }

    if (runtime_d.server_pid != 0 && runtime_d.client1_pid != 0 && runtime_d.client2_pid != 0 && runtime_d.client3_pid != 0 && runtime_d.client4_pid != 0) {

        int shm_id;
        key_t mem_key;
        struct runtime *shm_runtime_ptr;

        if ((mem_key = ftok(SHM_RUNTIME_LOC, SHM_RUNTIME_KEY)) == (key_t) -1) {
            perror("\nIPC error: ftok\n"); exit(1);
        }

        shm_id = shmget(mem_key, sizeof(struct runtime), IPC_CREAT | 0666);
        shm_runtime_ptr = (struct runtime*) shmat(shm_id, NULL, 0);

        shm_runtime_ptr->server_pid = runtime_d.server_pid;
        shm_runtime_ptr->client1_pid = runtime_d.client1_pid;
        shm_runtime_ptr->client2_pid = runtime_d.client2_pid;
        shm_runtime_ptr->client3_pid = runtime_d.client3_pid;
        shm_runtime_ptr->client4_pid = runtime_d.client4_pid;
        shm_runtime_ptr->clients_up = 0;
    }

    sleep(1);

    // SERVER
    if (runtime_d.server_pid == 0) {
        if (prctl(PR_SET_NAME, (unsigned long) "server") < 0)
            logMessage("RUNNER", "ERROR while naming process server", COLOR_RED);
        logMessage("RUNNER", "Server process created!", COLOR_GREEN);
        runServer();

    }

    sleep(2);

    //CLIENTS
    if (runtime_d.client1_pid == 0 && runtime_d.server_pid != 0 && runtime_d.client2_pid != 0 && runtime_d.client3_pid != 0 && runtime_d.client4_pid != 0) {
        if (prctl(PR_SET_NAME, (unsigned long) "client_1") < 0)
            logMessage("RUNNER", "ERROR while naming process client 1", COLOR_RED);
        logMessage("RUNNER", "Client 1 process created!", COLOR_GREEN);
        runClient1();
    }

    if (runtime_d.client2_pid == 0 && runtime_d.server_pid != 0 && runtime_d.client1_pid != 0 && runtime_d.client3_pid != 0 && runtime_d.client4_pid != 0) {
        if (prctl(PR_SET_NAME, (unsigned long) "client_2") < 0)
            logMessage("RUNNER", "ERROR while naming process client 2", COLOR_RED);
        logMessage("RUNNER", "Client 2 process created!", COLOR_GREEN);
        runClient2();
    }

    if (runtime_d.client3_pid == 0 && runtime_d.server_pid != 0 && runtime_d.client2_pid != 0 && runtime_d.client1_pid != 0 && runtime_d.client4_pid != 0) {
        if (prctl(PR_SET_NAME, (unsigned long) "client_3") < 0)
            logMessage("RUNNER", "ERROR while naming process client 3", COLOR_RED);
        logMessage("RUNNER", "Client 3 process created!", COLOR_GREEN);
        runClient3();
    }

    if (runtime_d.client4_pid == 0 && runtime_d.server_pid != 0 && runtime_d.client2_pid != 0 && runtime_d.client3_pid != 0 && runtime_d.client1_pid != 0) {
        if (prctl(PR_SET_NAME, (unsigned long) "client_4") < 0)
            logMessage("RUNNER", "ERROR while naming process client 4", COLOR_RED);
        logMessage("RUNNER", "Client 4 process created!", COLOR_GREEN);
        runClient4();
    }

    return 0;
}








