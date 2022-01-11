#ifndef COFF_IO_H
#define COFF_IO_H

#include <stdio.h>

char* coff_read_file(const char* filename);

ssize_t getline(char **lineptr, ssize_t *n, FILE *stream);

#endif