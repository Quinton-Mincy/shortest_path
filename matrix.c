#include "my_mouse.h"
void init_matrix(int n, int m, char* maze, char matrix[n][m]){
    int i = 0;
    int j = 0;
    int k = 0;
    while(i < n){
        while(j < m){
            matrix[i][j] = maze[k];
            j++;
            k++;
        }
        j=0;
        k++;//skip newlines
        i++;
    }
}
char* pop_matrix(char **maze, int row_size){
    char* row = strndup(*maze,row_size+1);
    (*maze) += (row_size+1);
    return row;
}