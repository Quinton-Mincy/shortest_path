#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_WIDTH 1001
#define MAX_HEIGHT 1001

typedef struct s_maze_config{
    int width;
    int height;
    char full;
    char empty;
    char path;
    char entrance;
    char exit;
}MazeConfig;

typedef struct s_Point{
    int i;
    int j;
}Point;

typedef struct s_Node{
    int dist;//distance from start node
    struct s_Point pt;
    struct s_Node *prev;
}Node;

typedef struct s_Queue{
    struct s_Point front;
    struct s_Point rear;
    int count;
    int max_size;
    void (*enqueue) (int n, int m, struct s_Node node_q[n][m], struct s_Node new_node,struct s_Queue* this);
    Node* (*dequeue) (int n, int m, struct s_Node node_q[n][m],struct s_Queue* this);
    bool (*empty) (struct s_Queue this);
    struct s_Node **nodes;
}Queue;

/*matrix.c*/
void init_matrix(int n, int m, char* maze, char matrix[n][m]);
char* pop_matrix(char **maze, int row_size);

/*queue.c*/
Node* dequeue(int n, int m, Node node_q[n][m],Queue* q);
void init_queue(Queue* new_queue, int rows, int columns);
bool empty(Queue q);
void enqueue(int n, int m, Node node_q[n][m], Node new_node,Queue* q);

/*maze_config.c*/
void get_chars(MazeConfig** parsed_config, char *config);
char* get_config(char* buffer,char* config);
int get_height(char** config);
int get_width(char** config);
int init_config(MazeConfig *parsed_config, char* config, char* buffer, char** maze, int* n, int* m);
bool is_digit(char c);
int open_file(char* filename, struct stat *sb, size_t* size, int* fd);
int parse_config(MazeConfig *parsed_config,char *config);
int read_file(char* buffer,size_t size, int fd);

/*bfs.c*/
Node* bfs(int n, int m, char matrix[n][m], MazeConfig* config, Node nodes[n][m]);
void find_start(int n, int m, char matrix[n][m],Point* entry, char entrance);
bool is_valid(int i, int j, int n, int m);
void search(int n, int m, Node** curr_node, MazeConfig* config, char matrix[n][m], Node nodes[n][m], Point *entry);
void set_visit(int n,int m,bool visited[n][m]);

/*display.c*/
void printmm(int n, int m, char matrix[n][m]);
int show_results(int n, int m, Node* dist, char matrix[n][m], char *config);

/*error.c*/
void handle_error(char *err_msg);
