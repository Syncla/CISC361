#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <stdlib.h>  
#include <stdio.h>  

typedef struct node{
    int jobID;
    int arrivalTime;
    int mainMemory;
    int serial;
    int priority;
    struct node *next;
    struct node *prev;
}node;


#endif