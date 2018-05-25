// Normal includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
// Threading includes
#include <unistd.h>
#include <pthread.h>

// Custom includes
#include "LL.h"

// COLOR DEFINITIONS
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

// ERROR CODES
#define INVALIDFILENAME -1
#define INVALIDEXT -2
#define INVALIDCONFIG -3
#define EXIT 0
// Function codes
#define OK 1		 // Function succeded
#define EOFREACHED 0 // Function failed
// Booleans
#define true 1
#define false 0

// CONSTANT VARIABLE DECLARATIONS
#define buffSize 255 // Buffer to hold each readline
#define configSize 5 // How many tokens are in each config line
// How many parameters to parse in
#define jobsize 7	 // Parse 7 parameters on a job arrival
#define requestsize 4 // Parse 4 parameters on a device request
#define releasesize 4 // Parse 4 parameters on a device release
#define displaysize 2 // Parse 2 parameters on a display
#define DEF -1		  // Default integer value
#define cmpSize 1
// Variable to indicate if the program should run in debug mode
// If in debug mode, a verbose output will be displayed so its easier to see what went wrong
int debug = 0;

// Function declarations

int readLine(FILE *fp, char buff[255]);
void clearBuff(char buff[255]);
int min(int x, int y);
int max(int x, int y);
float turn = 0;
int aT = DEF;
int id = DEF;
int pri = DEF;
int mem = DEF;
int dev = DEF;
int run = DEF;
int operation = DEF;
int processLine(char buff[255], int *aT, int *id, int *pri, int *mem, int *dev, int *run, int *operation);
// Main
int main(int argc, char *argv[])
{
	// Constant value declerations
	int M = 0; // Amount of main memory available
	int S = 0; // Number of devices
	int Q = 0; // Size of time quantum
	int T = 0; // Start time

	// Variable value declerations
	int interrupt = 0; // Time quantum interrupt
	int devLeft = 0;   // How many devices are left
	int memLeft = 0;   // How much memory is left

	int operation = 0; // What is going on
	// 0 : nothing, increment to next time slice
	// 1 : new job
	// 2 : request for device
	// 3 : release device
	// 4 : printout
	struct node getSize;
	int nodeSize = sizeof(getSize);
	// Buffer to read lines into
	char buff[buffSize];
	// Current running node
	struct node *running = NULL; //malloc(sizeof(struct node));
	// Queues
	struct LL *hQ1 = list_new();	  // SJF Hold queue
	struct LL *hQ2 = list_new();	  // FIFO Hold queue
	struct LL *complete = list_new(); // List of complete tasks
	struct LL *ready = list_new();	// Ready queue
	struct LL *wait = list_new();
	struct LL *all = list_new();
	struct LL *submit = list_new();
	// File stuff
	FILE *inp;				  // File handle
	char *filename = argv[1]; // Filename

	// Check if verbose output is on
	if (argc == 3)
	{
		if (!strcmp("-v\0", argv[2]))
		{
			debug = 1;
		}
	}

	if (debug)
		printf("Opening file: %s\n", filename);

	inp = fopen(filename, "r");
	// Check for valid file name
	if (inp == NULL)
	{
		printf("%sInvalid file name %s%s\n", KRED, filename, KNRM);
		return INVALIDFILENAME;
	}

	// Get configuration
	if (debug)
		printf("Getting configuration\n");
	readLine(inp, buff);
	if (strncmp(buff, "C", 1))
	{
		printf("%sNot a valid configuration line %s%s\n", KRED, KNRM, buff);
		return INVALIDCONFIG;
	}
	else
	{
		printf("%s\n", buff);
		if (debug)
			printf("Configuration is: %s\n", buff); // What was read from the file
		// Split configuration based on spaces
		char *tokens[configSize];
		char *token = strtok(buff, " ");
		int idx = 0;
		while (token)
		{
			tokens[idx] = malloc(sizeof(char) * strlen(token));
			strncpy(tokens[idx++], token, strlen(token));
			token = strtok(NULL, " ");
		}
		for (int i = 0; i < configSize; i++)
		{
			token = strtok(tokens[i], "=");
			while (token)
			{
				if (!strncmp(token, "M\0", 2))
				{
					M = atoi(strtok(NULL, "="));
					memLeft = M;
				}
				else if (!strncmp(token, "S\0", 2))
				{
					S = atoi(strtok(NULL, "="));
					devLeft = S;
				}
				else if (!strncmp(token, "Q\0", 2))
				{
					Q = atoi(strtok(NULL, "="));
				}
				else if (!strncmp(token, "C\0", 2))
				{
					T = atoi(tokens[++i]);
				}
				token = strtok(NULL, "=");
			}
		}
		// What was parsed
		if (debug)
			printf("Start Time:%s%d%s | M=%s%d%s | S=%s%d%s | Q=%s%d%s\n", KGRN, T, KNRM, KGRN, M, KNRM, KGRN, S, KNRM, KGRN, Q, KNRM);
	}

	int eventStart = T;
	int eventEnd = T;

	int quantumEnd = T + Q;
	int quantumStart = T;
	int processed = 0;
	int workTime = Q;
	//processLine(buff,&aT,&id,&pri,&mem,&dev,&run,&operation);
	int status = OK;
	eventEnd = T + Q;
	eventStart = T;
	quantumStart = T;
	quantumEnd = T + Q;

	// Process Events
	while (true)
	{

		// First line has special case
		if (aT == DEF)
		{
			printf("Starting\n");
			status = readLine(inp, buff);
			operation = processLine(buff, &aT, &id, &pri, &mem, &dev, &run, &operation);
		}

		if (aT < eventEnd)
		{
			eventEnd = aT;
		}
		// Work up until the event
		workTime = eventEnd - eventStart;

		//printf("%d %d %d\n",eventStart,eventEnd,workTime);
		// While you can work
		if (running != NULL || ready->head != NULL)
		{

			while (workTime > 0)
			{
				// if there is a job running
				if (running != NULL)
				{
					// TODO FINISH REMOVAL OF NODES IN QUEUES
					// Have enough devices to run
					if (running->devicesAssigned == running->serial)
					{
						// Not enough time to finish
						if (running->timeLeft > workTime && running->complete == 0)
						{
							running->timeLeft -= workTime;
							workTime = 0;
							if (eventEnd == quantumEnd)
							{
								printf("Swapping job: %d with", running->jobID);
								pushNodeFIFO(ready, running);
								cpyNode(running, ready->head);
								pop(ready);
								printf(" %d\n", running->jobID);
							}
						}
						// job finished
						else if (running->timeLeft <= workTime && running->complete == 0)
						{
							//printf("JOB: %d is done\n",running->jobID);
							// compute leftover time
							// New quantum
							printf("Job finished on quantum %d to %d\n", quantumStart, T);
							printf("%d %d\n",T,running->timeLeft);
							T = eventStart + running->timeLeft;
							//quantumStart = T;
							quantumEnd = T;
							printf("%d %d %d %d %d\n",running->jobID,quantumStart,quantumEnd,eventStart,eventEnd);
							//eventEnd = quantumEnd;
							workTime = 0;
							// Finish the job
							running->timeFinished = eventStart + running->timeLeft;
							//eventEnd = T+Q;
							printf("Job finished at time : %d\n", running->timeFinished);
							running->timeLeft = 0;
							running->complete = 1;
							// deallocate resources
							devLeft += running->devicesAssigned;
							memLeft += running->mainMemory;
							running->devicesAssigned = 0;
							pushNodeFIFO(complete, running);

							// If possible, transfer jobs to ready queue
							// Check if a task in wait queue can be put in the ready queue
							struct node *tmp = wait->head;
							while (tmp != NULL)
							{
								if (tmp->devicesRequested <= devLeft)
								{
									devLeft -= tmp->devicesRequested;
									tmp->devicesAssigned += tmp->devicesRequested;
									tmp->devicesRequested = 0;
									pushNodeFIFO(ready, tmp);
									int idToRemove = tmp->jobID;
									tmp = tmp->next;
									// TODO REMOVE TMP BY ID FROM WAIT
								}
								else
								{
									tmp = tmp->next;
								}
							}
							// Check if a task in the hold queue can be put in the ready queue
							// Check queue 1 first
							tmp = hQ1->head;
							while (tmp != NULL)
							{
								if (tmp->serial <= devLeft && tmp->mainMemory < memLeft)
								{
									pushNodeFIFO(ready, tmp);
									int idToRemove = tmp->jobID;
									tmp = tmp->next;
									// TODO REMOVE TMP BY ID FROM hQ1
								}
								else
								{
									tmp = tmp->next;
								}
							}
							tmp = hQ2->head;
							while (tmp != NULL)
							{
								if (tmp->serial <= devLeft && tmp->mainMemory < memLeft)
								{
									pushNodeFIFO(ready, tmp);
									int idToRemove = tmp->jobID;
									tmp = tmp->next;
									// TODO REMOVE TMP BY ID FROM hQ1
								}
								else
								{
									tmp = tmp->next;
								}
							}
							// Free running
							
							free(running);
							running = NULL;
							/*
							// Grab the next job from the ready queue
							if (ready->head != NULL)
							{
								cpyNode(running, ready->head);
								pop(ready);
							}
							else
							{
								// Nothing on the queue
								workTime = 0;
								free(running);
								running = NULL;
							}
							*/
							workTime = 0;
						}
						if (running)
							printf("%d\n",running->timeLeft);
					}
					else
					{
						// Does not have any devices to work with
						workTime = 0;
					}
				}
				else
				{

					// No job is running, grab head of ready
					if (ready->head != NULL)
					{
						running = malloc(nodeSize);
						cpyNode(running, ready->head);
						pop(ready);
					}
					else
					{
						// Nothing to do, continue
						workTime = 0;
					}
				}
			}
		}

		// Update the time
		//printf("here %d %d\n",quantumEnd,eventEnd);
		T = min(quantumEnd, eventEnd);

		// Hit the event
		if (aT == T)
		{

			// new job
			if (operation == 1)
			{

				pushFIFO(submit, id, aT, mem, dev, run, pri, 0, run, (-1), 0, 0);
				if (mem < M && dev < S)
				{
					if (mem < memLeft)
					{
						// There is enough memory to run
						pushFIFO(ready, id, aT, mem, dev, run, pri, 0, run, (-1), 0, 0);
						memLeft -= mem;
					}
					else
					{
						// Not enough memory, put it on hold
						if (pri == 1)
						{
							pushSJF(hQ1, id, aT, mem, dev, run, pri, 0, run, (-1), 0, 0);
						}
						else
						{
							pushFIFO(hQ2, id, aT, mem, dev, run, pri, 0, run, (-1), 0, 0);
						}
					}
					// Put it in list of all jobs
					pushFIFO(all, id, aT, mem, dev, run, pri, 0, run, (-1), 0, 0);
					if (running == NULL)
					{
						// Nothing was running, start a new quantum
						quantumStart = aT;
						quantumEnd = aT + Q;
						eventEnd = quantumEnd;
						eventStart = aT;
					}
				}

				// Not the end of the quantum, continue processing
				eventStart = aT;
				eventEnd = quantumEnd;
			}
			// Request for device
			else if (operation == 2)
			{

				if (id == running->jobID)
				{
					if (running->devicesAssigned + dev < running->serial)
					{
						// Assign devices to running process
						running->devicesAssigned += dev;
						devLeft -= dev;
						// Move running process to end of ready queue

						pushNodeFIFO(ready, running);
						// Pop of top of ready queue to put on running

						cpyNode(running, ready->head);
						pop(ready);
					}
					else
					{
						// Not enough devices left, put on device queue
						pushNodeFIFO(wait, running);
						// Pop of top of ready queue to put on running
						if (ready->head != NULL)
						{
							cpyNode(running, ready->head);
							pop(ready);
						}
						else
						{
							free(running);
							running = NULL;
						}
					}
				}
				// Interrupt, new quantum
				quantumStart = T;
				quantumEnd = T + Q;
				eventStart = T;
				eventEnd = T + Q;
			}
			// Release for device
			else if (operation == 3)
			{
				if (running!=NULL){
				if (id == running->jobID)
				{
					if (running->devicesAssigned - dev > 0)
					{
						// Assign devices to running process
						running->devicesAssigned -= dev;
						devLeft += dev;
						// Move running process to end of ready queue
						pushNodeFIFO(ready, running);
						// Pop of top of ready queue to put on running
						printf("here\n");
						if (ready->head != NULL)
						{
							cpyNode(running, ready->head);
							pop(ready);
						}
						else
						{
							free(running);
							running = NULL;
						}
						int max = wait->size;
						int c = 0;
						struct node *tmp = malloc(nodeSize);
						printf("here\n");
						while (wait->head && c < max)
						{
							// Pop off devices on wait queue if possible
							cpyNode(tmp, wait->head);
							pop(wait);
							// if (needed - assigned - available < 0), put on ready
							if (tmp->serial - tmp->devicesAssigned - devLeft)
							{
								pushNodeFIFO(ready, tmp);
							}
							else
							{
								// Put it back on wait queue
								pushNodeFIFO(wait, tmp);
								// avoid double counting
								c++;
								// if c>max, that means that we have scanned all the items on the wait queue
							}
						}
						
						free(tmp);
					}
					else
					{
						
						// Not enough devices to unassing, put on wait
						pushNodeFIFO(wait, running);
						// Pop of top of ready queue to put on running
						if (ready->head != NULL)
						{
							cpyNode(running, ready->head);
							pop(ready);
						}
						else
						{
							free(running);
							running = NULL;
						}
					}
				}
				}
				// Interrupt, new quantum
				quantumStart = T;
				quantumEnd = T + Q;
				eventStart = T;
				eventEnd = T + Q;
				printf("Here\n");
			}
			// Display
			else if (operation == 4)
			{
				FILE *out;
				char *name = malloc(sizeof(char) * (strlen(filename) - 3));
				char *outName = malloc(sizeof(char) * (strlen(name) + 4));
				strncpy(name, filename, strlen(filename) - 3);
				name[strlen(name) - 1] = '\0';
				snprintf(outName, strlen(name) + 15, "%s_D%d.json", name, aT);
				out = fopen(outName, "w");
				fprintf(out, "{\n");
				fprintf(out, "\t\"current_time\":%d,\n", T);
				fprintf(out, "\t\"total_memory\":%d,\n", M);
				int memavai = M - getAssignedMemory(ready);
				int devavai = S - getAssignedDevices(ready);
				if (running != NULL)
				{
					memavai -= running->mainMemory;
					devavai -= running->devicesAssigned;
				}

				fprintf(out, "\t\"available_memory\":%d,\n", memavai);
				fprintf(out, "\t\"total_devices\":%d,\n", S);
				fprintf(out, "\t\"available_devices\":%d,\n", devavai);
				fprintf(out, "\t\"quantum\":%d,\n", Q);
				fprintf(out, "\t\"turnaround\":%d,\n", turn);
				fprintf(out, "\t\"weighted_turnaround\":%d,\n", turn);
				fprintf(out, "\t\"readyq\":[\n");
				printJobIDs(out, ready);
				fprintf(out, "\t],\n");
				fprintf(out, "\t\"running\":");
				if (running != NULL)
				{
					fprintf(out, "%d,\n", running->jobID);
				}
				else
				{
					fprintf(out, "-1,\n");
				}
				fprintf(out, "\t\"submitq\":[");
				printJobIDs(out, submit);
				fprintf(out, "\t],\n");

				fprintf(out, "\t\"completeq\":[\n");
				printJobIDs(out, complete);
				fprintf(out, "\t],\n");

				fprintf(out, "\t\"waitq\":[\n");
				printJobIDs(out, wait);
				fprintf(out, "\t],\n");

				fprintf(out, "\t\"hQ1\":[\n");
				printJobIDs(out, hQ1);
				fprintf(out, "\t],\n");
				
				fprintf(out, "\t\"hQ2\":[\n");
				printJobIDs(out, hQ2);
				fprintf(out, "\t],\n");
				
				fprintf(out, "\t\"job\":[\n");
				printDetail(out, all);
				fprintf(out, "\t]\n");

				fprintf(out, "}\0");

				free(outName);
				free(name);
				eventStart = aT;
				eventEnd = quantumEnd;
			}
			// Read new line to get the next event
			status = readLine(inp, buff);
			// If EOF
			if (status != EOFREACHED)
			{
				operation = processLine(buff, &aT, &id, &pri, &mem, &dev, &run, &operation);
			}
			else
				break;
		}
		else
		{
			// Haven't hit the event yet, continue as normal
			quantumStart = T;
			quantumEnd = T + Q;
			eventEnd = quantumEnd;
			eventStart = quantumStart;
		}
	}

	if (running != NULL)
	{
		printf("Currently running\n");
		printNode(running);
	}
	else
	{
		printf("Not running anything\n");
	}
	printf("ready: \n");
	printLL(ready);
	printf("wait: \n");
	printLL(wait);
	printf("hQ1: \n");
	printLL(hQ1);
	printf("hQ2: \n");
	printLL(hQ2);
	printf("complete: \n");
	printLL(complete);
	printf("all \n");
	printLL(all);
	list_free(ready);
	list_free(wait);
	list_free(hQ1);
	list_free(hQ2);
	list_free(complete);
	list_free(all);
	fclose(inp);
	return EXIT;
}
void clearBuff(char buff[255])
{
	for (int i = 0; i < 255; i++)
	{
		buff[i] = '\0';
	}
}
int readLine(FILE *fp, char buff[255])
{
	clearBuff(buff);
	if (fgets(buff, 255, (FILE *)fp))
	{
		//Strip newline character
		char *nL = strrchr(buff, '\n'); //Get pos of newline
		if (nL)
		{
			*nL = '\0'; //If there is a newline, replace it with null char
		}
	}
	else
	{
		return EOFREACHED;
	}
	return OK;
}
int min(int x, int y)
{
	if (x < y)
	{
		return x;
	}
	return y;
}
int max(int x, int y)
{
	if (x > y)
	{
		return x;
	}
	return y;
}
int processLine(char buff[255], int *aT, int *id, int *pri, int *mem, int *dev, int *run, int *operation)
{
	// Handle job arrival (Quantum is not interrupted)
	if (!strncmp("A", buff, cmpSize))
	{
		if (debug)
			printf("New job has arrived\n");

		char *tokens[jobsize];
		char *token = strtok(buff, " ");
		int idx = 0;
		while (token)
		{
			tokens[idx] = malloc(sizeof(char) * strlen(token));
			strncpy(tokens[idx++], token, strlen(token));
			token = strtok(NULL, " ");
		}
		for (int i = 0; i < jobsize; i++)
		{
			token = strtok(tokens[i], "=");
			while (token)
			{
				if (!strncmp(token, "J\0", cmpSize))
				{
					*id = atoi(strtok(NULL, "="));
				}
				else if (!strncmp(token, "M\0", cmpSize))
				{
					*mem = atoi(strtok(NULL, "="));
				}
				else if (!strncmp(token, "S\0", cmpSize))
				{
					*dev = atoi(strtok(NULL, "="));
				}
				else if (!strncmp(token, "R\0", cmpSize))
				{
					*run = atoi(strtok(NULL, "="));
				}
				else if (!strncmp(token, "P\0", cmpSize))
				{
					*pri = atoi(strtok(NULL, "="));
				}
				else if (!strncmp(token, "A\0", cmpSize))
				{
					*aT = atoi(tokens[++i]);
				}
				token = strtok(NULL, "=");
			}
		}
		if (debug)
			printf("Arrival Time: %s%d%s | Job ID: %s%d%s | Memory usage: %s%d%s | Device usage: %s%d%s | Run time: %s%d%s | Priority: %s%d%s\n", KGRN, *aT, KNRM, KGRN, *id, KNRM, KGRN, *mem, KNRM, KGRN, *dev, KNRM, KGRN, *run, KNRM, KGRN, *pri, KNRM);
		*operation = 1;
	}
	// Handle request for devices (Interrupt quantum)
	else if (!strncmp("Q", buff, cmpSize))
	{
		if (debug)
			printf("Device request has arrived\n");
		char *tokens[requestsize];
		char *token = strtok(buff, " ");
		int idx = 0;
		while (token)
		{
			tokens[idx] = malloc(sizeof(char) * strlen(token));
			strncpy(tokens[idx++], token, strlen(token));
			token = strtok(NULL, " ");
		}
		for (int i = 0; i < requestsize; i++)
		{
			token = strtok(tokens[i], "=");
			while (token)
			{
				if (!strncmp(token, "J\0", cmpSize))
				{
					*id = atoi(strtok(NULL, "="));
				}

				else if (!strncmp(token, "D\0", cmpSize))
				{
					*dev = atoi(strtok(NULL, "="));
				}
				else if (!strncmp(token, "Q\0", cmpSize))
				{
					*aT = atoi(tokens[++i]);
				}
				token = strtok(NULL, "=");
			}
		}
		if (debug)
			printf("Arrival Time: %s%d%s | Job ID: %s%d%s | Devices requested: %s%d%s\n", KGRN, *aT, KNRM, KGRN, *id, KNRM, KGRN, *dev, KNRM);

		*operation = 2;
	}
	// Handle release for devices
	else if (!strncmp("L", buff, cmpSize))
	{
		if (debug)
			printf("Device release has arrived\n");
		char *tokens[releasesize];
		char *token = strtok(buff, " ");
		int idx = 0;
		while (token)
		{
			tokens[idx] = malloc(sizeof(char) * strlen(token));
			strncpy(tokens[idx++], token, strlen(token));
			token = strtok(NULL, " ");
		}
		for (int i = 0; i < releasesize; i++)
		{
			token = strtok(tokens[i], "=");
			while (token)
			{
				if (!strncmp(token, "J\0", cmpSize))
				{
					*id = atoi(strtok(NULL, "="));
				}

				else if (!strncmp(token, "D\0", cmpSize))
				{
					*dev = atoi(strtok(NULL, "="));
				}
				else if (!strncmp(token, "L\0", cmpSize))
				{
					*aT = atoi(tokens[++i]);
				}
				token = strtok(NULL, "=");
			}
		}
		if (debug)
			printf("Arrival Time: %s%d%s | Job ID: %s%d%s | Devices requested: %s%d%s\n", KGRN, *aT, KNRM, KGRN, *id, KNRM, KGRN, *dev, KNRM);
		*operation = 3;
	}
	// Handle display
	else if (!strncmp("D", buff, cmpSize))
	{
		if (debug)
			printf("Display has arrived\n");
		char *tokens[displaysize];
		char *token = strtok(buff, " ");
		int idx = 0;
		while (token)
		{
			tokens[idx] = (char *)malloc(sizeof(char) * strlen(token));
			strncpy(tokens[idx++], token, strlen(token));
			token = strtok(NULL, " ");
		}
		for (int i = 0; i < displaysize; i++)
		{

			token = strtok(tokens[i], "=");
			while (token)
			{
				if (!strncmp(token, "D\0", cmpSize))
				{
					*aT = atoi(tokens[++i]);
				}
				token = strtok(NULL, "=");
			}
		}
		if (debug)
			printf("Arrival Time: %s%d%s\n", KGRN, *aT, KNRM);
		*operation = 4;
	}
	return *operation;
}