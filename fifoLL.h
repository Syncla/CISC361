#ifndef FIFOLL_H
#define FIFOLL_H
#include "node.h"
void push(void *newData);
struct node * pop();
struct node* head;
struct node* tail;

#endif