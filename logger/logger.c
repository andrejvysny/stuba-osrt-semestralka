#include <stdio.h>
#include "logger.h"

void logMessage(char service[], char message[], int code){

    if(code < 0){
        printf("\033[1;31m\t%s:\033[0m\t%s\n",service, message);
    }else if(code == 1){
        printf("\033[0;33m\t%s:\033[0m\t%s\n",service, message);
    }else if(code == 2){
        printf("\033[0;36m\t%s:\033[0m\t%s\n",service, message);
    }else if(code == 0){
        printf("\033[0;32m\t%s:\033[0m\t%s\n",service, message);
    }else{
        printf("\t%s:\t%s\n",service, message);
    }
}

void debugBuffer(char buffer[], int size){

    int i =0;
    while (buffer[i] != '\0' && i <= size){
        if(buffer[i]=='\0'){
            printf("Position: %2d  Char: \\",i);
            printf("0\n");
        }
        else if(buffer[i]=='\n'){
            printf("Position: %2d  Char: \\",i);
            printf("n\n");
        }
        else if(buffer[i]=='\t'){
            printf("Position: %2d  Char: \\",i);
            printf("t\n");
        }
        else{
            printf("Position: %2d  Char: %c\n",i,buffer[i]);
        }
        i++;
    }
    printf("\n\n");
}