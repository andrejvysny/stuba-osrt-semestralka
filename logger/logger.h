
#ifndef OSRT_SEMESTRALKA_LOGGER_H
#define OSRT_SEMESTRALKA_LOGGER_H

#define COLOR_RED -1
#define COLOR_GREEN 0
#define COLOR_YELLOW 1
#define COLOR_INFO 2
#define COLOR_WHITE 100

void logMessage(char service[], char message[], int code);
void debugBuffer(char buffer[], int size);

#endif //OSRT_SEMESTRALKA_LOGGER_H
