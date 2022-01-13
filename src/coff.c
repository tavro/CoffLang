#include "include/coff.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/io.h"
#include "include/assembly.h"

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
    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse_expr(parser);
	token_T* t = 0;

    char* s = assembly_frontend(root);
    printf("%s\n", s);

    /*
	while ((t = lexer_next_token(lexer))->type != TOKEN_EOF)
	{
		printf("%s\n", token_to_str(t));
	}
    */
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