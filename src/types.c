#include "include/types.h"

#include <string.h>

/* Function: typename_to_int
 * ---------------------------
 *   Converts typename to an int
 *
 *   name : the typename you want to convert
 * 
 *   returns : int
 */
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