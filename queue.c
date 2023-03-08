#include "my_mouse.h"
Node* dequeue(int n, int m, Node node_q[n][m],Queue* q){
    Node* node = NULL;
    if(q->count>0){
        node = &node_q[q->front.i][q->front.j];
        if(q->front.j == m-1){
            q->front.i+=1;
        }
        q->front.j = ((q->front.j)+1)%m;
        q->count--;
    }else{
        printf("Error: Queue Underflow!!\n");
    }
    return node;
}
bool empty(Queue q){
    return q.count == 0;
}
void enqueue(int n, int m, Node node_q[n][m], Node new_node,Queue* q){
    // printf("count: %d, max_size: %d, rear_j: %d, rear_i: %d\n",(*q).count,(*q).max_size,(*q).rear.j,(*q).rear.i);
    if((*q).count < (*q).max_size){
        if((*q).rear.j == m -1){
            (*q).rear.i+=1;
        }
        (*q).rear.j = ((*q).rear.j+=1)%m;
        node_q[(*q).rear.i][(*q).rear.j] = new_node;
        (*q).count+=1;
    }else{
        printf("Error: Queue Overflow!!\n");
    }
}
void init_queue(Queue* new_queue, int rows, int columns){
    new_queue->count = 0;
    new_queue->dequeue = &dequeue;
    new_queue->enqueue = &enqueue;
    new_queue->empty = &empty;
    new_queue->max_size = rows*columns;
    new_queue->front.i = 0;
    new_queue->front.j = 0;
    new_queue->rear.i = 0;
    new_queue->rear.j = -1;
}