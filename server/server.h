#ifndef OSRT_SEMESTRALKA_SERVER_H
#define OSRT_SEMESTRALKA_SERVER_H

int runServer();
struct runtime* getRuntimeData();

#define SERVER_ACTION_EXIT '0'
#define SERVER_ACTION_GET_FILE '1'
#define SERVER_ACTION_SAVE_RESULT '2'

#endif //OSRT_SEMESTRALKA_SERVER_H