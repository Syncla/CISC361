#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define buffSize 255
const char *getFilenameExt(const char *filename);
void readLine(FILE * fp,char*buff);
int main(int argc, char *argv[])
{
	int M=0;
	int S=0;
	int Q=0;
	int time = 0;
	char buff[buffSize];
	FILE * inp;
	char * fname = argv[1];
	// Check for valid file name
	char * filename = argv[1];
	inp = fopen(filename,"r");
	if (inp==NULL){
		printf("%sInvalid file name %s%s\n",KRED,filename,KNRM);
		return;
	}
	
	// Check for valid extension
	const char * ext = getFilenameExt(filename);
	if (strcmp(ext,"txt")){
		printf("%sInvalid file extension %s%s\n",KRED,ext,KNRM);
		return;
	}

	printf("Opening file: %s\n",fname);
	printf("Getting configuration\n");
	readLine(inp,&buff);
	if (strncmp(buff,"C",1)){
		printf("%sNot a valid configuration line %s%s\n",KRED,KNRM,buff);
		return;
	}
	else{
		printf("Configuration is %s",buff);
		// Split configuration based on spaces
		char * token = strtok(buff," ");
		int idx = 0;
		while(token !=NULL){
			if (idx==1){
				time = atoi(token);
			}
			else if(idx==2){
				char id[2];
				id[1] = '\0';
				strncpy(id,token,1);
				if (!strncmp(id,"M",1)){
					char num[10];
					strcpy(num,token);
					char * tmp = strtok(num,"=");
					M=atoi(strtok(NULL,"="));
				}
			}
			else if(idx==3){
				char id[2];
				id[1] = '\0';
				strncpy(id,token,1);
				if (!strncmp(id,"S",1)){
					char num[10];
					strcpy(num,token);
					char * tmp = strtok(num,"=");
					S=atoi(strtok(NULL,"="));
				}
			}
			else if(idx==4){
				char id[2];
				id[1] = '\0';
				strncpy(id,token,1);
				if (!strncmp(id,"Q",1)){
					char num[10];
					strcpy(num,token);
					char * tmp = strtok(num,"=");
					Q=atoi(strtok(NULL,"="));
				}
			}
			if (idx>5){
				break;
			}
			printf("%d: %s\n",idx,token);
			token = strtok(NULL," ");
			idx++;
		}
		printf("Start Time:%d M=%d S=%d Q=%d\n",time,M,S,Q);
	}

	readLine(inp,&buff);
	printf("%s",buff);	
	fclose(inp);
	return 0;
}
void readLine(FILE * fp,char*buff){
	fgets(buff,255,(FILE*)fp);
}
const char *getFilenameExt(const char *filename) {
	/*
		This function consumes a file name and extracts the file extension
		parameters:
			filename - a char array containing the filename
		returns:
			ext - a char array containing the file extension
	*/
	const char *ext = strrchr(filename, '.');
	// If there is no file extension, return an empty string
	if (!ext || ext == filename) {
		return "";
	}
	// Return the file exension
	return ext + 1;
}