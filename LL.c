//Vinay Vazir & Jason Reynolds

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LL.h"

/*
int main(void){
	struct LL* myListFIFO = NULL;
    struct LL* myListSJF = NULL;

    myListFIFO = list_new();

    pushFIFO(myListFIFO,1,1,1,1,1,1);
    pushFIFO(myListFIFO,2,2,2,2,2,1);
    pushFIFO(myListFIFO,3,3,3,3,3,1);
    pushFIFO(myListFIFO,4,4,4,4,4,1);
    pushFIFO(myListFIFO,5,4,4,4,4,1);
    pushFIFO(myListFIFO,6,4,4,4,4,1);

    printLL(myListFIFO);

    pop(myListFIFO);

    printLL(myListFIFO);

    printf("FIFO test complete, SJF test begin:\n");

    myListSJF = list_new();
    pushSJF(myListSJF,1,1,4,1,1,1);
    pushSJF(myListSJF,2,1,4,1,1,1);
    pushSJF(myListSJF,3,1,4,1,1,1);
    pushSJF(myListSJF,4,1,2,1,1,1);
    pushSJF(myListSJF,5,1,3,1,1,1);
    pushSJF(myListSJF,6,1,6,1,1,1);
    pushSJF(myListSJF,7,1,1,1,1,1);

    printLL(myListSJF);

    pop(myListSJF);

    printLL(myListSJF);
    popByID(myListSJF,3);
    printLL(myListSJF);
    popByID(myListSJF,5);
    printLL(myListSJF);

    
    printLL(myListFIFO);
    popByID(myListFIFO,3);    
    printLL(myListFIFO);
    
    printLL(myListFIFO);
    popByID(myListFIFO,5);    
    printLL(myListFIFO);

return 0;

}//main
*/

//FUNCTIONS DEFINITIONS
struct LL *pushSJF(struct LL * l,const int id,const int at ,const int mm,const int ser,const int rt,const int pri,const int devicesAssigned,const int timeLeft,const int timeFinished,const int complete,const int devicesRequested)
{
	struct node *n = malloc(1 * sizeof(*n));

	if (n == NULL)
	{
		//fprintf(stderr, "IN %s, %s: malloc() failed\n", __FILE__, "list_add");
		return l;
	} //IF

	n->jobID = id;
	n->arrivalTime = at;
	n->mainMemory = mm;
	n->serial = ser;
	n->runTime = rt;
	n->priority = pri;
	n->devicesAssigned = devicesAssigned;
	n->devicesRequested = devicesRequested;
	n->timeLeft = timeLeft;
	n->timeFinished = timeFinished;
	n->complete = complete;
	n->next = NULL;
	n->prev = NULL;
	if (l == NULL)
	{
		//printf("Queue not initialized\n");
		free(n);
		return l;
	} //IF

	else if (NULL == l->head && NULL == l->tail)
	{
		//printf("Empty list, adding first node:%d\n",n->jobID);
		l->head = l->tail = n;
		return l;
	} //ELSE IF

	else if (NULL == l->head || NULL == l->tail)
	{
		fprintf(stderr, "ERROR: Something wrong with your assignment of head/tail to the list\n");
		free(n);
		return NULL;
	} //ELSE IF

	else
	{
		//printf("adding node:%d\n",n->jobID);
		if (n->timeLeft <= l->head->timeLeft)
		{
			n->next = l->head;
			l->head->prev = n;
			l->head = n;
		} //if
		else
		{
			struct node *currNode;
			currNode = l->head;
			//printf("currNode created for J=%d\n",n->jobID);
			while (currNode->next != NULL && currNode->timeLeft < n->timeLeft)
			{
				currNode = currNode->next;
			} //while

			if (currNode == l->tail)
			{
				n->prev = currNode->prev->next;
				n->prev->next = n;
				l->tail = n;
				n->next = NULL;
			}

			else
			{
				n->next = currNode->prev->next;
				n->prev = currNode->prev;
                n->prev->next = n;
                n->next->prev = n;

			} //ELSE
		}
	} //ELSE
	  //printLL(l);
	l->size++;
	return l;

} //PUSHSJF

struct LL *pushNodeSJF(struct LL *l, struct node *n){
    
	 return pushSJF(l,n->jobID,n->arrivalTime,n->mainMemory,n->serial,n->runTime,n->priority,n->devicesAssigned,n->timeLeft,n->timeFinished,n->complete,n->devicesRequested);
    
} //ADDS NODE TO THE LIST

struct LL *pushFIFO(struct LL * l,const int id,const int at ,const int mm,const int ser,const int rt,const int pri,const int devicesAssigned,const int timeLeft,const int timeFinished,const int complete,const int devicesRequested)
{
	struct node *n = malloc(1 * sizeof(*n));

	if (n == NULL)
	{
		//fprintf(stderr, "IN %s, %s: malloc() failed\n", __FILE__, "list_add");
		return l;
	} //IF

	n->jobID = id;
	n->arrivalTime = at;
	n->mainMemory = mm;
	n->serial = ser;
	n->runTime = rt;
	n->priority = pri;
	n->devicesAssigned = devicesAssigned;
	n->devicesRequested = devicesRequested;
	n->timeLeft = timeLeft;
	n->timeFinished = timeFinished;
	n->complete = complete;
	n->next = NULL;
	n->prev = NULL;

	if (l == NULL)
	{
		//printf("Queue not initialized\n");
		free(n);
		return l;
	} //IF

	else if (NULL == l->head && NULL == l->tail)
	{ /* printf("Empty list, adding p->num: %d\n\n", p->num);  */
		l->head = l->tail = n;
		return l;
	} //ELSE IF

	else if (NULL == l->head || NULL == l->tail)
	{
		fprintf(stderr, "ERROR: Something wrong with your assignment of head/tail to the list\n");
		free(n);
		return NULL;
	} //ELSE IF

	else
	{ /* printf("List not empty, adding element to tail\n"); */
		l->tail->next = n;
        n->prev = l->tail;
		l->tail = n;
	} //ELSE
	l->size++;
	return l;

} //PUSHFIFO

struct LL *pushNodeFIFO(struct LL *l, struct node *n){
    return pushFIFO(l,n->jobID,n->arrivalTime,n->mainMemory,n->serial,n->runTime,n->priority,n->devicesAssigned,n->timeLeft,n->timeFinished,n->complete,n->devicesRequested);
}//pushNodeFIFO


struct LL *pop(struct LL *l)
{
	struct node *h = NULL;
	struct node *n = NULL;
	
	if (NULL == l)
	{
		//printf("List is empty\n");
		return l;
	}

	else if (NULL == l->head && NULL == l->tail)
	{
		//printf("Well, List is empty\n");
		return l;
	}

	else if (NULL == l->head || NULL == l->tail)
	{
		printf("ERROR: Something wrong with your assignment of head/tail to the list\n");
		
		return l;
	}

	h = l->head;
	n = h->next;
	free(h);
	l->head = n;
	if (NULL == l->head)
		l->tail = l->head; /* The element tail was pointing to is free(), so we need an update */
	l->size--;
	return l;
} //POP

struct LL *list_free(struct LL *l)
{
	while (l->head)
	{
		pop(l);
	} //WHILE

	return l;
} //LIST_FREE

struct LL *list_new(void)
{
	struct LL *l = malloc(1 * sizeof(*l));
	
	if (NULL == l)
	{
		//fprintf(stderr, "LINE: %d, malloc() failed\n", __LINE__);
	} //IF

	l->head = l->tail = NULL;
	l->size=0;
	return l;
} //LIST_NEW

void cpyNode(struct node *n2, struct node *n1){
    
	n2->jobID = n1->jobID;
    n2->arrivalTime = n1->arrivalTime;
	n2->mainMemory = n1->mainMemory;
    n2->serial = n1->serial;
    n2->runTime = n1->runTime;
    n2->priority = n1->priority;
    n2->devicesAssigned = n1->devicesAssigned;
    n2->timeLeft = n1->timeLeft;
    n2->timeFinished = n1->timeFinished;
    n2->complete = n1->complete;
    n2->devicesRequested = n1->devicesRequested;
	n2->next = n1->next;
	n2->prev = n1->prev;
	
}
void printLL(const struct LL *l)
{
	struct node *n = NULL;

	if (l){
		for (n = l->head; n; n = n->next){
			printNode(n);
		} //FOR
	}//IF

	printf("------------------\n");
} //PRINTLL

void printNode(const struct node *n)
{
	if (n)
	{
		printf("Arr = %d Job = %d Mem = %d Ser = %d R = %d P = %d Time Left = %d Time Finished = %d\n", n->arrivalTime, n->jobID, n->mainMemory, n->serial, n->runTime, n->priority,n->timeLeft,n->timeFinished);
	} //IF
	else
	{
		printf("Can not print NULL struct \n");
	} //ELSE
} //PRINTNODE

int getAssignedDevices(struct LL *l){
    int ad = 0;
    if(l){
        for (struct node * n=l->head;n;n=n->next){
            ad += n->devicesAssigned;
        }//for
    }//IF
    
    return ad;
    
}//GETASSIGNEDDEVICES

int getAssignedMemory(struct LL *l){
    int am = 0;
    
    if(l){
        for (struct node *n=l->head;n;n=n->next){
            am += n->mainMemory;
        }//for
    }//IF
    
    return am;
    
}// GETASSIGNEDMEMORY

void printJobIDs(FILE *out, struct LL *l){
    if (l==NULL){
        return;
    }//if
    
    struct node *n = NULL;
    
    fprintf(out, "\n");
	
    if (l){
		for (n = l->head; n; n = n->next){
            if (n != l->tail){
                fprintf(out,"\t\t%d,\n",n->jobID);
            }
            else{
                fprintf(out,"\t\t%d\n",n->jobID);
            }//else
		} //FOR
	}//IF
    
}//printJobIDs


void printDetail(FILE *out, struct LL *l){
        if (l==NULL){
        return;
    }//if
    
    struct node *n = NULL;
    
    fprintf(out, "\n");
	
    if (l){
		for (n = l->head; n; n = n->next){
            if (n != l->tail){
                fprintf(out,"\t{\n");
                fprintf(out,"\t\t\"arrival_time\": %d,\n",n->arrivalTime);
                fprintf(out,"\t\t\"devices_allocated\": %d,\n",n->devicesAssigned);
                fprintf(out,"\t\t\"id\": %d,\n",n->jobID);
                fprintf(out,"\t\t\"remaining_time\": %d\n",n->timeLeft);
                fprintf(out, "\t},\n");
            }
            else{
                fprintf(out,"\t{\n");
                fprintf(out,"\t\t\"arrival_time\": %d,\n",n->arrivalTime);
                fprintf(out,"\t\t\"devices_allocated\": %d,\n",n->devicesAssigned);
                fprintf(out,"\t\t\"id\": %d,\n",n->jobID);
                fprintf(out,"\t\t\"remaining_time\": %d\n",n->timeLeft);
                fprintf(out, "\t}\n");
            }//else
		} //FOR
	}//IF
}//printDetail

struct LL *popByID(struct LL *l,int id){
  if (l==NULL){
        return NULL;
    }//if
    
    struct node *n = NULL;
    
    if (l){
		for (n = l->head; n; n = n->next){
            //printf("nope:%d\n",n->jobID);
            if (n->jobID == id){
                //printf("match found\n");
                if (n == l->head){
                    //printf("HEAD: pop\n");
                    pop(l);
                }//if
                else if (n == l->tail){
                    //printf("TAIL:\n");
                    n->prev->next = NULL;
                    l->tail = n->prev;
                }//esle if
                else{
                    //printf("MIDDLEOUT\n");
                    n->prev->next = n->next;
                    n->next->prev = n->prev;
                }//else
            };
        
        } //FOR
	}//IF 
    
    return l;
}//popbyID

double getAVGTurnaround(struct LL* l){
    if ( l == NULL){
        return 0.0;
    }
    
    int sum;
    int numNodes = 0;
    if(l){
        for (struct node * n=l->head;n;n=n->next){
            sum += n->timeFinished-n->arrivalTime;
            numNodes++;
        }//for
    }//IF
    if (numNodes==0)
		return 0;
    return (sum/numNodes);
}//getAVGTurnaround
