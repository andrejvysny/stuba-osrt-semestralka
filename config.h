#ifndef OSRT_SEMESTRALKA_CONFIG_H
#define OSRT_SEMESTRALKA_CONFIG_H

#define SERVER_PORT 7777

#define SERVER_IP "127.0.0.1"

#define SERVER_BACKLOG 5

#define SHM_RUNTIME_KEY 1225

#define CHAR_BUFFER_SIZE 1024

struct runtime {
    pid_t server_pid;
    pid_t client1_pid;
    pid_t client2_pid;
    pid_t client3_pid;
    pid_t client4_pid;
    int clients_up;
};

#define INPUT_FILE "input.txt"

#endif //OSRT_SEMESTRALKA_CONFIG_H
