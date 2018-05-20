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


// FUNCTION DECLARATIONS
struct LL* push( struct LL*,const int,const int,const int,const int,const int, const int);			//ADDS NODE TO THE LIST
struct LL* pop(struct LL* );					//REMOVES FIRST IN FROM THE LIST

struct LL* list_new(void);						//CREATES NEW LINKED LIST
struct LL* list_free(struct LL*);				//

void printLL (const struct LL*);				//PRINTS ENTIRE LINKED LIST
void printNode(const struct node*);				//PRINTS A NODE

int main(void){
	struct LL* myList = NULL;

myList = list_new();
push(myList,1,1,4,1,1,1);
push(myList,2,1,4,1,1,1);
push(myList,3,1,4,1,1,1);
push(myList,4,1,2,1,1,1);
push(myList,5,1,3,1,1,1);
push(myList,6,1,6,1,1,1);
push(myList,7,1,1,1,1,1);

printLL(myList);

pop(myList);

printLL(myList);

return 0;
}//main


//FUNCTIONS DEFINITIONS
struct LL* push(struct LL* l, const int id,const int at,const int mm,const int ser, const int rt, const int pri){
	struct node* n = malloc(1* sizeof(*n));

	if( n == NULL ){
  		fprintf(stderr, "IN %s, %s: malloc() failed\n", __FILE__, "list_add");
	  	return l; 
	}//IF

	n->jobID = id;
    n->arrivalTime = at;
    n->mainMemory = mm;
    n->serial = ser;
    n->runTime = rt;
    n->priority = pri;
    
	n->next = NULL;


	if( l == NULL){
  		printf("Queue not initialized\n");
  		free(n);
  		return l;
	}//IF
	
	else if( NULL == l->head && NULL == l->tail ){			
        printf("Empty list, adding first node:%d\n",n->jobID);
  		l->head = l->tail = n;
  		return l;
	}//ELSE IF
	
	else if( NULL == l->head || NULL == l->tail ){
  		fprintf(stderr, "There is something seriously wrong with your assignment of head/tail to the list\n");
  		free(n);
  		return NULL;
	}//ELSE IF
	
	else{
        printf("adding node:%d\n",n->jobID);
        if (n->mainMemory <= l->head->mainMemory){
            n->next = l->head;
            l->head->prev = n;
            l->head = n;
        }//if 
        else{
            struct node* currNode;
            currNode = l->head; 
            printf("currNode created for J=%d\n",n->jobID);
            while(currNode->next != NULL && currNode->mainMemory < n->mainMemory){
                currNode = currNode->next;
            }//while
            
            if(currNode == l->tail){
                n->prev = currNode->prev->next;
                n->prev->next = n;
                l->tail = n;
                n->next = NULL;
            }
            
            else{
                n->next = currNode->prev->next;
                n->prev = currNode->prev;
                currNode->prev->next = n;


                printf("n: ");
                printNode(n);
                printf("curr: ");
                printNode(currNode);

            }//ELSE
        }
	}//ELSE
    printLL(l);
	return l;

}//PUSH


struct LL* pop(struct LL* l ){
	struct node* h = NULL;
  	struct node* n = NULL;
 
  	if( NULL == l ){
    	printf("List is empty\n");
      	return l;
    }
  	
  	else if( NULL == l->head && NULL == l->tail ){
      	printf("Well, List is empty\n");
      	return l;
    }
  
  	else if( NULL == l->head || NULL == l->tail ){
      	printf("There is something seriously wrong with your list\n");
      	printf("One of the head/tail is empty while other is not \n");
      	return l;
    }
 
  	h = l->head;
  	n = h->next;
  	free(h);
  	l->head = n;
  	if( NULL == l->head )  l->tail = l->head;   /* The element tail was pointing to is free(), so we need an update */
 
  	return l;
}//POP

struct LL* list_free( struct LL* l ){
	while( l->head ){
      	pop(l);
    }//WHILE
 
  	return l;
}//LIST_FREE
 
struct LL* list_new(void){
	struct LL* l = malloc( 1 * sizeof(*l));
 
	if( NULL == l ){
      	fprintf(stderr, "LINE: %d, malloc() failed\n", __LINE__);
    }//IF
 
  	l->head = l->tail = NULL;
   
  	return l;
}//LIST_NEW
 
 
void printLL( const struct LL* l ){
	struct node* n = NULL;
 
	if( l ){
      	for( n = l->head; n; n = n->next ){
      		printNode(n);
    	}//FOR
    }//IF
 
  printf("------------------\n");
}//PRINTLL
 
 
void printNode(const struct node* n ){
  	if( n ) {
    	printf("J = %d M = %d\n", n->jobID,n->mainMemory);
    }//IF
  	else{
      	printf("Can not print NULL struct \n");
    }//ELSE
}//PRINTNODE


