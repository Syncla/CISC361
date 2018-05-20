#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
 
 struct node
 {
 	int num;
 	struct node* next;
 };//node struct

 struct LL
 {
 	struct node* head;
 	struct node* tail;
 };

 struct LL* push( struct LL*,const int);
 struct LL* pop(struct LL* );

 struct LL* list_new(void);
 struct LL* list_free(struct LL*);

 void printLL (const struct LL*);
 void printLLuhMent(const struct node*);

 int main(void){
 	struct LL* myList = NULL;

	myList = list_new();
	push(myList,1);
	push(myList,2);
	push(myList,3);
	push(myList,4);

	printLL(myList);

	pop(myList);

	printLL(myList);

	return 0;
 }//main

 struct LL* push(struct LL* s, const int i){
 	struct node* n = malloc(1* sizeof(*p));
 	
 }//push