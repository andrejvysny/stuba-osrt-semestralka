#include <stdio.h>

#include "clients.h"

float getAverageMatrix(int matrix[100][100]){

    float count = 0;
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            count += (int) matrix[i][j];
        }
    }
    return count / (100*100);
}


int getSumMatrix(int matrix[100][100]){

    int count = 0;
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            count += (int) matrix[i][j];
        }
    }
    return count;
}

int getMinMatrix(int matrix[100][100]){
    int min = matrix[0][0];
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            if(matrix[i][j] < min){
                min = matrix[i][j];
            }
        }
    }
    return min;
}

int getMaxMatrix(int matrix[100][100]){
    int max = matrix[0][0];
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            if(matrix[i][j] > max){
                max = matrix[i][j];
            }
        }
    }
    return max;
}