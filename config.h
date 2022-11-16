//
// Created by andrejvysny on 15.11.2022.
//

#ifndef OSRT_SEMESTRALKA_CONFIG_H
#define OSRT_SEMESTRALKA_CONFIG_H

#define SERVER_PORT 7777

#define SERVER_IP "127.0.0.1"

#define SERVER_BACKLOG 5

struct process_pids {
    pid_t server_pid;
    pid_t client1_pid;
    pid_t client2_pid;
    pid_t client3_pid;
    pid_t client4_pid;
};

#define SHM_PIDS_KEY 1224
#endif //OSRT_SEMESTRALKA_CONFIG_H
