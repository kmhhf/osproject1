#ifndef QUEUE_H_
#define QUEUE_H_

struct node
{
    char* directory;
    struct node* next;
};

struct queue
{
    struct node* first;
    struct node* last;
};

void createQueue(struct queue* que);

void enqueue(struct queue* que, char* directory);

char* dequeue(struct queue* que);

#endif