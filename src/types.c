#include "include/types.h"

#include <string.h>

int typename_to_int(const char* name) 
{
	int t = 0;
	size_t len = strlen(name);

	for(int i = 0; i < len; i++)
	{
		t += name[i];
	}

	return t;
}