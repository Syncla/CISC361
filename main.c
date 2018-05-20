// Normal includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
// Threading includes
#include <unistd.h>
#include <pthread.h>

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
#define OK 1			// Function succeded
#define EOFREACHED 0	// Function failed

// CONSTANT VARIABLE DECLARATIONS
#define buffSize 255 // Buffer to hold each readline
#define configSize 5 // How many tokens are in each config line
// How many parameters to parse in
#define jobsize 7		// Parse 7 parameters on a job arrival
#define requestsize 4	// Parse 4 parameters on a device request
#define releasesize 4	// Parse 4 parameters on a device release
#define displaysize 2	// Parse 2 parameters on a display

// Variable to indicate if the program should run in debug mode
// If in debug mode, a verbose output will be displayed so its easier to see what went wrong
int debug = 0;

// Function declarations
const char *getFilenameExt(const char *filename);
int readLine(FILE *fp, char buff[255]);
void clearBuff(char buff[255]);

// Main
int main(int argc, char *argv[])
{
	int M = 0;			 // Amount of main memory available
	int S = 0;			 // Number of devices
	int Q = 0;			 // Size of time quantum
	int T = 0;			 // Start time
	char buff[buffSize]; // Buffer to read lines into
	FILE *inp;			 // File handle
	char *filename = argv[1];
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

	// Check for valid extension
	const char *ext = getFilenameExt(filename);
	if (strcmp(ext, "txt"))
	{
		printf("%sInvalid file extension %s%s\n", KRED, ext, KNRM);
		return INVALIDEXT;
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
				}
				else if (!strncmp(token, "S\0", 2))
				{
					S = atoi(strtok(NULL, "="));
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
	// Read file
	while (readLine(inp, buff))
	{
		// Handle job arrival
		if (!strncmp("A", buff, 1))
		{
			if (debug)
				printf("New job has arrived\n");
			int aT = NULL;
			int id = NULL;
			int pri = NULL;
			int mem = NULL;
			int dev = NULL;
			int run = NULL;
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
					if (!strncmp(token, "J\0", 2))
					{
						id = atoi(strtok(NULL, "="));
					}
					else if (!strncmp(token, "M\0", 2))
					{
						mem = atoi(strtok(NULL, "="));
					}
					else if (!strncmp(token, "S\0", 2))
					{
						dev = atoi(strtok(NULL, "="));
					}
					else if (!strncmp(token, "R\0", 2))
					{
						run = atoi(strtok(NULL, "="));
					}
					else if (!strncmp(token, "P\0", 2))
					{
						pri = atoi(strtok(NULL, "="));
					}
					else if (!strncmp(token, "A\0", 2))
					{
						aT = atoi(tokens[++i]);
					}
					token = strtok(NULL, "=");
				}
			}
			if (debug)
				printf("Arrival Time: %s%d%s | Job ID: %s%d%s | Memory usage: %s%d%s | Device usage: %s%d%s | Run time: %s%d%s | Priority: %s%d%s\n", KGRN, aT, KNRM, KGRN, id, KNRM, KGRN, mem, KNRM, KGRN, dev, KNRM, KGRN, run, KNRM, KGRN, pri, KNRM);
		}
		// Handle request for devices
		if (!strncmp("Q", buff, 1))
		{
			if (debug)
				printf("Device request has arrived\n");
			int aT;
			int id;
			int dev;
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
					if (!strncmp(token, "J\0", 2))
					{
						id = atoi(strtok(NULL, "="));
					}
					
					else if (!strncmp(token, "D\0", 2))
					{
						dev = atoi(strtok(NULL, "="));
					}
					else if (!strncmp(token, "Q\0", 2))
					{
						aT = atoi(tokens[++i]);
					}
					token = strtok(NULL, "=");
				}
			}
			if (debug)
				printf("Arrival Time: %s%d%s | Job ID: %s%d%s | Devices requested: %s%d%s\n", KGRN, aT, KNRM, KGRN, id, KNRM, KGRN, dev, KNRM);
		}
		// Handle release for devices
		if (!strncmp("L", buff, 1))
		{
			if (debug)
				printf("Device release has arrived\n");
			int aT;
			int id;
			int dev;
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
					if (!strncmp(token, "J\0", 2))
					{
						id = atoi(strtok(NULL, "="));
					}
					
					else if (!strncmp(token, "D\0", 2))
					{
						dev = atoi(strtok(NULL, "="));
					}
					else if (!strncmp(token, "L\0", 2))
					{
						aT = atoi(tokens[++i]);
					}
					token = strtok(NULL, "=");
				}
			}
			if (debug)
				printf("Arrival Time: %s%d%s | Job ID: %s%d%s | Devices requested: %s%d%s\n", KGRN, aT, KNRM, KGRN, id, KNRM, KGRN, dev, KNRM);
		}
		
		// Handle display
		if (!strncmp("D", buff, 1))
		{
			if (debug)
				printf("Display has arrived\n");
			int aT;
			char *tokens[displaysize];
			char *token = strtok(buff, " ");
			int idx = 0;
			while (token)
			{
				tokens[idx] = malloc(sizeof(char) * strlen(token));
				strncpy(tokens[idx++], token, strlen(token));
				token = strtok(NULL, " ");
			}
			for (int i = 0; i < displaysize; i++)
			{
				token = strtok(tokens[i], "=");
				while (token)
				{
					if (!strncmp(token, "D\0", 2))
					{
						aT = atoi(tokens[++i]);
					}
					token = strtok(NULL, "=");
				}
			}
			if (debug)
				printf("Arrival Time: %s%d%s\n", KGRN, aT, KNRM);
		}
		
	}
	fclose(inp);
	return EXIT;
}
void clearBuff(char buff[255])
{
	for (int i = 0; i < 255; i++)
	{
		buff[i] = NULL;
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
const char *getFilenameExt(const char *filename)
{
	/*
		This function consumes a file name and extracts the file extension
		parameters:
			filename - a char array containing the filename
		returns:
			ext - a char array containing the file extension
	*/
	const char *ext = strrchr(filename, '.');
	// If there is no file extension, return an empty string
	if (!ext || ext == filename)
	{
		return "";
	}
	// Return the file exension
	return ext + 1;
}