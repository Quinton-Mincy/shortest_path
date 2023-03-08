#include "my_mouse.h"
Node* bfs(int n, int m, char matrix[n][m], MazeConfig* config, Node nodes[n][m]){
    //if entry point is not changed, then their is no valid entry point
    Point entry = {-1,-1};
    find_start(n,m,matrix,&entry,config->entrance);
    if(entry.i == -1){
        printf("No entry point detected.\n");
        return NULL;
    }
    Node* curr_node = NULL;//just a poniter that points to locations in the nodes matrix :D
    search(n,m,&curr_node, config,matrix,nodes,&entry);

    return curr_node;
}
void find_start(int n, int m, char matrix[n][m],Point* entry, char entrance){
    int i = 0;
    int j = 0;
    while(j<m){//checking only first row for now
        if(matrix[i][j] == entrance){
            entry->i = i;
            entry->j = j;
            return;
        }
        j++;
    }
}
bool is_valid(int i, int j, int n, int m){
    return ( (i>-1 && j>-1) && (i<n && j<m) );
}
void search(int n, int m, Node** curr_node, MazeConfig* config, char matrix[n][m], Node nodes[n][m], Point *entry){
    /*keeps track of information needed to use the Node array as a queue, and performs operations on 2D Node array(nodes), but does not encapsulate nodes*/
    Queue q;
    init_queue(&q,n,m);
    /*create and queue source node*/
    Node src = {0,*entry,NULL};
    q.enqueue(n,m,nodes,src,&q);
    /*matrix of visited/unvisited nodes*/
    bool visited[n][m];
    set_visit(n,m,visited);
    visited[entry->i][entry->j] = true;
    /*used below to check neigbhors of curr_node*/
    int row_mod[] = {-1,0,1,0};
    int column_mod[] = {0,-1,0,1};
    while(!q.empty(q)){//if queue is empty before exit is found, their is no exit
        /*pop most recently added node*/
        *curr_node = q.dequeue(n,m,nodes,&q);
        Point curr_point = (*curr_node)->pt;
        /*curr node is the exit node*/
        if(matrix[curr_point.i][curr_point.j] == config->exit){
            return;
        }
        /*check curr_node neighbors*/
        for(int i = 0;i<4;i++){
            /*checks neighbors up,left,down,and right*/
            int row = curr_point.i+row_mod[i];
            int column = curr_point.j+column_mod[i];
            /*guard clauses*/
            if(!is_valid(row,column,n,m)) continue;//node is inbounds
            if( (matrix[row][column] != config->empty) && (matrix[row][column] != config->exit) ) continue;//code is either an empty character or the exot character
            if(visited[row][column]) continue;//node has not been visited yet
            /*mark neighbor as visited, add it to the queue*/
            visited[row][column] = true;
            Node neighbor = {(*curr_node)->dist+1,{row,column},(*curr_node)};//each neighbor is one space away from the curr node, and curr node set to prev of neighbor
            q.enqueue(n,m,nodes,neighbor,&q);
        }
    }
    *curr_node = NULL;//no path present
}
void set_visit(int n,int m,bool visited[n][m]){
    int i = 0;
    while(i < n){
        memset(visited[i],false,sizeof(bool)*m);
        i++;
    }
}