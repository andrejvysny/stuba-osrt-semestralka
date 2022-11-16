#include <stdio.h>
#include <string.h>
#include "logger.h"

void logMessage(char service[], char message[], int code){

    if(code < 0){

        printf("\033[1;31m\t%s:\033[0m\t%s\n",service, message);
    }

    else if(code == 0){
        printf("\033[0;32m\t%s:\033[0m\t%s\n",service, message);
    }else{
        printf("\t%s:\t%s\n",service, message);
    }
}