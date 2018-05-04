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
    node *next;
    node *prev;
}node;

node * init(int jobID, int arrivalTime, int mainMemory, int serial,int priority);

#endif