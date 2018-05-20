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
    struct node *next;
    struct node *prev;

}; //node struct

struct LL
{
    struct node *head;
    struct node *tail;
};

// FUNCTION DECLARATIONS
struct LL *pushSJF(struct LL *, const int, const int, const int, const int, const int, const int); //ADDS NODE TO THE LIST
struct LL *pushFIFO(struct LL *, const int, const int, const int, const int, const int, const int);

struct LL *pop(struct LL *); //REMOVES FIRST IN FROM THE LIST

struct LL *list_new(void);         //CREATES NEW LINKED LIST
struct LL *list_free(struct LL *); //

void cpyNode(struct node *, struct node *);

void printLL(const struct LL *);     //PRINTS ENTIRE LINKED LIST
void printNode(const struct node *); //PRINTS A NODE
#endif