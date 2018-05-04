#include "node.h"

node * init(int jobID, int arrivalTime, int mainMemory, int serial,int priority)
{
   node * n =(node *)malloc(sizeof(node));
    n->jobID=jobID;
    n->arrivalTime=arrivalTime;
    n->mainMemory=mainMemory;
    n->serial=serial;
    n->priority;
    return n;
}