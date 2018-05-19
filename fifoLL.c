#include "fifoLL.h"
#include "node.h"
#include<stdio.h>
#include<stdlib.h>
void push(void *newData){
    if(head==NULL){
        head = (node*)newData;
        tail = head;
    }
    tail->next = (node*)newData;
    tail = tail->next;
}
struct node * pop(){
    node * tmp = head;
    head = tmp->next;
    return tmp; 
}
int main(){
    node * tmp = malloc(sizeof(node));
    tmp->arrivalTime=0;
    tmp->jobID=1;
    
    printf("Hello world\n");
}