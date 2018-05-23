#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef LL_H_INCLUDED
#define LL_H_INCLUDED
//STRUCT DEFINITIONS

struct node
{
    int jobID;
    int arrivalTime;
    int mainMemory;
    int serial;
    int runTime;
    int priority;
    int devicesAssigned;
    int timeLeft;
    int timeFinished;
    int complete;
    struct node *next;
    struct node *prev;
}; //node struct

struct LL
{
    int size;
    struct node *head;
    struct node *tail;
};

// FUNCTION DECLARATIONS
struct LL *pushSJF(struct LL *, const int, const int, const int, const int, const int, const int); //ADDS NODE TO THE LIST
struct LL *pushNodeSJF(struct LL *, struct node *); //ADDS NODE TO THE LIST

struct LL *pushFIFO(struct LL *, const int, const int, const int, const int, const int, const int);
struct LL *pushNodeFIFO(struct LL *, struct node *);

struct LL *pop(struct LL *); //REMOVES FIRST IN FROM THE LIST

struct LL *list_new(void);         //CREATES NEW LINKED LIST
struct LL *list_free(struct LL *); //

void cpyNode(struct node * __dst__, struct node * __src__);

void printLL(const struct LL *);     //PRINTS ENTIRE LINKED LIST
void printNode(const struct node *); //PRINTS A NODE
int getAssignedMemory(struct LL *);

int getAssignedDevices(struct LL *);

void printJobIDs(FILE * ,struct LL *);

void printDetail(FILE * ,struct LL *);


#endif