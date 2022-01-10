#include "include/io.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function: coff_read_file
 * ---------------------------
 *   Simply reads a file and returns it contents
 *
 *   filename: the file to read
 * 
 *   returns : char*
 */
char* coff_read_file(const char* filename)
{
	FILE* fp;
	fp = fopen(filename, "rb");

	char* line = NULL;

	size_t len = 0;
	ssize_t read;

	
	if(fp == NULL)
	{
		printf("Could not read file %s\n", filename);
		exit(1);
	}

	char* buffer = (char*) calloc(1, sizeof(char));
	buffer[0] = '\0';

	while((read = getline(&line, &len, fp)) != -1)
	{
		buffer = (char*) realloc(buffer, (strlen(buffer) + strlen(line) + 1) * sizeof(char));
		strcat(buffer, line);
	}

	fclose(fp);

	if(line)
	{
		free(line);
	}

	return buffer;	
}