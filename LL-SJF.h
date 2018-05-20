#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

//STRUCT DEFINITIONS
struct node{
	int jobID;
    int arrivalTime;
    int mainMemory;
    int serial;
    int runTime;
    int priority;
    struct node *next;
    struct node *prev;
    
};//node struct

struct LL{
	struct node* head;
	struct node* tail;
};
