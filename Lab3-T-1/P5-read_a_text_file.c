/***********************
   ----- 48450 -- week 4 lab handout 5 ------ 
This is a program for you to read the line of text from a data file data.txt.

**********************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

int main(int argc, char** argv) {
	char c[100];
	FILE* fptr;
	char file_name[100];
	int sig;
	char check[12] = "end_header\n";

	/* Verify the correct number of arguments were passed in */
	if (argc != 2) {
		fprintf(stderr, "USAGE:./main.out data.txt\n");
	}

	strcpy(file_name, argv[1]); //copy a string from the commond line to get the file name


	if ((fptr = fopen(file_name, "r")) == NULL) {
		printf("Error! opening file");
		// Program exits if file pointer returns NULL.
		exit(1);
	}

	// reads text until newline is encountered
	printf("reading from the file:\n");

	// indicate whether the program read to the end of header
	sig = 0;

	while (fgets(c, sizeof(c), fptr) != NULL) {
		//if the program read to the end of header
		//then, read the data region and print it to the console (sig==1)
		if (sig == 1)
			fputs(c, stdout);

		/* check whether this line is the end of header,
		the new line in array c contains "end_header\n"*/
		if ((sig==0) && strcmp(c, check)==0)
		{
		//Yes. The end of header
			sig = 1;
		}			
	}

	fclose(fptr);

	return 0;
}


