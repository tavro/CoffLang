#include "include/coff.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/io.h"
#include "include/assembly.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char* bash(const char* cmd)
{
    char* output = (char*) calloc(1, sizeof(char));
    output[0] = '\0';

    FILE* fp;
    char path[1035];

    fp = _popen(cmd, "r");

    if(fp == NULL) 
    {
        printf("Failed to run command...");
        exit(1);
    }

    while(fgets(path, sizeof(path), fp) != NULL) 
    {
        output = (char*) realloc(output, (strlen(output) + strlen(path) + 1) * sizeof(char));
        strcat(output, path);
    }

    _pclose(fp);

    return output;
}

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

    char* s = assembly_frontend_root(root);

    coff_write_file("ass.s", s);
    printf("%s\n", s);

    bash("as ass.s -o ass.o");
    bash("ld ass.o -o ass.exe");

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