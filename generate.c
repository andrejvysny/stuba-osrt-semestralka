#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if(argc<4){
        perror("Missing one of input numbers.\n");
        return 0;
    }

    int lower = atoi(argv[1]), upper = atoi(argv[2]), count = atoi(argv[3]);

    srand(time(0));

    FILE *fw;
    if((fw = fopen("input.txt","w")) == NULL){
        perror("Error while creating file");
        exit(0);
    }

    int i;
    for (i = 1; i <= count; i++) {
        int num = (rand() % (upper - lower + 1)) + lower;
        printf("%3d. %5d\n",i, num);
        fprintf(fw, "%d\n",num);
    }

    fclose(fw);
    return 0;
}