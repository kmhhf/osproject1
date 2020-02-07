#include "queue.h"
#include <stddef.h>
#include <stdlib.h>

void createQueue(struct queue* que)
{
    que->first = NULL;
    que->last = NULL;
}

void enqueue(struct queue* que, char* directory)
{
    struct node* temp;
    temp = malloc(sizeof(struct node));
    temp->directory = directory;
    temp->next = NULL;

    if (que->first == NULL)
    {
        que->first = temp;
        que->last = temp;
    }

    else
    {
        que->last->next = temp;
        que->last = temp;
    }
}

char* dequeue(struct queue* que)
{
    struct node* temp;
    char* dir = que->first->directory;
    temp = que->first;
    que->first = que->first->next;
    free(temp);
    return(dir);
}