#include "my_mouse.h"
void printmm(int n, int m, char matrix[n][m]){
    int i = 0;
    int j = 0;
    while(i< n){
        while(j<m){
            printf("%c ",matrix[i][j]);
            j++;
        }
        printf("\n");
        j = 0;
        i++;
    }
}
int show_results(int n, int m, Node* dist, char matrix[n][m], char *config){
        if(dist==NULL){
        printf("Path does not exist.\n");
        return EXIT_FAILURE;
    }else{
        int steps = dist->dist;
        printf("%s\n",config);
        // printf("Shortest Path is: %d\n",dist->dist);
        dist = dist->prev;
        while( (dist->pt.i != 0) && (dist->pt.j != 0)){
            // printf("{%d,%d}<==",dist->pt.i,dist->pt.j);
            matrix[dist->pt.i][dist->pt.j] = 'o';
            dist = dist->prev;
            // if((dist->pt.i == 0) || (dist->pt.j == 0)){
            //     // printf("{%d,%d}\n",dist->pt.i,dist->pt.j);
            //     matrix[dist->pt.i][dist->pt.j] = 'o';
            // }
        }
    printmm(n,m,matrix);
    printf("%d STEPS!\n",steps);
    }
    return EXIT_SUCCESS;
}