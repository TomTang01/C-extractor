#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define MAX_LINE_LEN 1024 // including null terminator
#define USAGE_FORMAT_STR "ERR: expected usage is %s -c <in_cols> [-d delimiter] [-s] col1 ...\n"

int main(int argc, char **argv) {
	int in_cols = 0; // number of columns in input
	int out_cols = 0; // number of columns to output

	bool c_flag = false; // keep track of whether or not we received the required c flag
	bool stats_flag = false; // keep track of whether -s flag is passed in or not

	char delim = ','; // keep track of the delimiter, by default it is a comma

	// local variables to store the statistics
	unsigned int lines = 0; // number of lines in input
	size_t max_len = 0; // longest field in input
	size_t output_size = 0; // total output printed in characters

	// TODO: process named command line options using getopt and store the information in the
	// appropriate local variables
	int c;
	/* TODO: fill out this skeleton code calling getopt */
	while ((c = getopt(argc, argv, "c:d:s")) != -1) {
		switch (c) {
		case 's':
			stats_flag = 1;
			break;
		case 'c':
			c_flag = 1;
			in_cols = atoi(optarg);
			break;
		case 'd':
			delim = *optarg;
			break;
		/*
		TODO: fill in remaining cases (c flag, d flag)
		- You will want to use the optarg variable set by getopt, to access option args
		- You will want to use atoi to convert strings to integer types
		*/
		default:
			fprintf(stderr, USAGE_FORMAT_STR, *argv);
			return EXIT_FAILURE;
		}
	}

	// TODO: uncomment following block & check to make sure c argument was provided
	
	if (c_flag == false) {
		// we did not receive the required c option, print usage and terminate
		fprintf(stderr, USAGE_FORMAT_STR, *argv);
		return EXIT_FAILURE;
	}

	/* pointers to arrays which will be allocated dynamically by you */
	char **colptrs = NULL; /* pointer to array of pointers to the start of
	                         each field in input buffer */
	int *target_cols = NULL; /* pointer to array of output indices */

	// allocate arrays dynamically
	// TODO: uncomment the block below & fill in mallocs
	
	colptrs = malloc(sizeof(*colptrs) * (in_cols)); // should be based on in_cols
	if (colptrs == NULL) { // always check if malloc failed
		return EXIT_FAILURE;
	}
	out_cols = (argc - optind); // should be based on argc, optind
	target_cols = malloc(sizeof(*target_cols) * out_cols);
	if (target_cols == NULL) { // always check if malloc failed
		free(colptrs);
		return EXIT_FAILURE;
	}

	// TODO: process other command line arguments (list of output columns)
	// Fill these into target_cols!
	for(int i = optind; i < argc; i++) {
		*(target_cols+i-optind) = atoi(*(argv+i));
	}
	char **colP = colptrs;
	int *colT = target_cols;
	char line[MAX_LINE_LEN]; // read buffer for processing lines in input
	// TODO: using fgets, loop over input from stdin
	// steps: read a line, parse the line as described in writeup, print target columns for this
	// line, repeat for next line, then repeat until no more lines
	char *lineptr;
	size_t wordLen = 0;
	int numOfWords = 0;
	while ((lineptr = fgets(line, MAX_LINE_LEN, stdin)) != NULL) {
		*colP = lineptr;
		numOfWords = 1;
		while(*lineptr != '\0') {
			if(*lineptr == delim) {
				*lineptr = '\0';
				*(colP+numOfWords) = lineptr + 1;
				if(wordLen > max_len) {
					max_len = wordLen;
				}
				wordLen = -1;
				numOfWords++;
			}
			else if(*lineptr == '\n') {
				*lineptr = '\0';
				if(wordLen > max_len) {
					max_len = wordLen;
				}
				wordLen = -1;
			}
				lineptr += 1;
				wordLen += 1;
		}
		int charCount = 0;
		for(int i=0; i<out_cols; i++) {
			if(i != 0) {
				printf("%c",delim);
				output_size++;
			}
			while(*(*(colP+*(colT+i)-1)+charCount) !='\0') {
				printf("%c",*(*(colP+*(colT+i)-1)+charCount));
				charCount++;
				output_size++;
			}
			charCount = 0;

		}
		printf("\n");
		output_size++;
		lines++;
	}
	// TODO: print out the statistics if -s flag is set
	if(stats_flag == 1) {
		printf(
			"Number of lines: %d\nLongest field: %zu characters\nOutput size: %zu characters\n",
			lines, max_len, output_size
		);
	}

	// free the memory for arrays allocated with malloc
	// TODO: uncomment these lines when you uncomment the mallocs above
	free(colptrs);
	free(target_cols);
	return EXIT_SUCCESS;
}
