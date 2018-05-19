#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

const char *getFilenameExt(const char *filename);

int main(int argc, char *argv[])
{
	FILE * json;
	char * fname = argv[1];
	// Check for valid file name
		char * filename = argv[1];
		json = fopen(filename,"r");
		if (json==NULL){
			printf("%sInvalid file name %s%s\n",KRED,filename,KNRM);
			return;
		}
		fclose(json);
		// Check for valid extension
		const char * ext = getFilenameExt(filename);
		if (strcmp(ext,"json")){
			printf("%sInvalid file extension %s%s\n",KRED,ext,KNRM);
			return;
		}
	if (strncmp())
	printf("Opening file: %s\n",fname);
	
	return 0;
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