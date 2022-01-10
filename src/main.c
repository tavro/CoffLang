#include "include/coff.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Specify file\n");
		return 1;
	}

	coff_compile_file(argv[1]);

	return 0;
}