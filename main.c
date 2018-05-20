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
// CONSTANT VARIABLE DECLARATIONS
#define buffSize 255 // Buffer to hold each readline
#define configSize 5 // How many tokens are in each config line

// Variable to indicate if the program should run in debug mode
int debug = 0;

// Function declarations
const char *getFilenameExt(const char *filename);
void readLine(FILE *fp, char buff[255]);
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
	readLine(inp, buff);
	printf("%c\n", buff[0]);
	fclose(inp);
	return EXIT;
}
void clearBuff(char buff[255])
{
	for (int i = 0;i<255;i++){
		buff[i]=NULL;
	}
}
void readLine(FILE *fp, char buff[255])
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