// Kyle Moore
// 02/03/20
// This file creates a queue ADT

#include "queue.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//Create an empty queue by setting both pointers to NULL
void createQueue(struct queue* que)
{
    que->first = NULL;
    que->last = NULL;
}

//Add a node to the queue
void enqueue(struct queue* que, char* directory)
{
    struct node* temp;
    char* dir = malloc(strlen(directory) + 1);  //allocate memory for the name of the directory to be added
    if(dir == NULL)                             //malloc error check
    {
        perror("Error: ");
        exit(EXIT_FAILURE);
    }

    strcpy(dir, directory);
    temp = malloc(sizeof(struct node));         //memeory for the node
    if(temp == NULL)                            //error check
    {
        perror("Error: ");
        exit(EXIT_FAILURE);
    }

    temp->directory = dir;
    temp->next = NULL;

    if (que->first == NULL)                     //add a node to an empty queue
    {
        que->first = temp;
        que->last = temp;
    }

    else                                        //add a node to the end of a queue
    {
        que->last->next = temp;
        que->last = temp;
    }
}

//remove a node from the queue and return the value stored in directory
char* dequeue(struct queue* que)
{
    struct node* temp;
    char* dir = que->first->directory;
    temp = que->first;
    que->first = que->first->next;
    free(temp);
    return(dir);
}
