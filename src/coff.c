#include "include/coff.h"
#include "include/lexer.h"
#include "include/io.h"

#include <stdlib.h>

/* Function: coff_compile
 * ---------------------------
 *   Function that compiles coff language source code.
 *
 *   src: the coff language source-code you want to compile
 * 
 *   returns : nothing
 */
void coff_compile(char* src) 
{
	lexer_T* lexer = init_lexer(src);
	token_T* t = 0;

	while ((t = lexer_next_token(lexer))->type != TOKEN_EOF)
	{
		printf("%s\n", token_to_str(t));
	}
}

/* Function: coff_compile_file
 * ---------------------------
 *   Function that compiles coff language source code
 *	 from given file.
 *
 *   filename: the coff language source-file you want to compile
 * 
 *   returns : nothing
 */
void coff_compile_file(const char* filename) 
{
	char* src = coff_read_file(filename);
	coff_compile(src);
	free(src);
}