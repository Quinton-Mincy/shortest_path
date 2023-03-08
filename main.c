#include "my_mouse.h"
int main(int ac, char *av[]){
    /*if ac>2, too many arguments given, if less, file name not specified*/
    if(ac != 2){
        return 1;
    }
    /* Opening given file, read into buffer, halting for errors */
    int fd;
    struct stat sb;
    size_t size;
    if(open_file(av[1],&sb,&size,&fd) == EXIT_FAILURE){
        return EXIT_FAILURE;
    }
    char buffer[size+1];
    if(read_file(buffer,size,fd) == -1){
        handle_error("read");
        return EXIT_FAILURE;
    }
    /*extract first line from given file (configuration of the maze)*/
    MazeConfig parsed_config;
    char config[15] = {'\0'};//max 14 characters in config line
    char* maze;
    int n;
    int m;
    if( (init_config(&parsed_config,&config[0],buffer,&maze,&n,&m)) == EXIT_FAILURE){
        return EXIT_FAILURE;
    }
    /*create 2D charater matrix representation of file*/
    char matrix[n][m];
    init_matrix(n,m,maze,matrix);
    /*find the shortest path, if it exists, and print the solution*/
    Node nodes[n][m];//memory pool containing enough space for all possible nodes
    Node* dist = bfs(n,m,matrix,&parsed_config,nodes);
    return show_results(n,m,dist,matrix,config);//returns EXIT_FAILURE or EXIT_SUCCESS
}
