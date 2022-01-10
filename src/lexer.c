#include "include/lexer.h"
#include "include/macros.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Function: init_lexer
 * ---------------------------
 *   Simply initialized and returns the lexer
 *
 *   src : the coff source code
 * 
 *   returns : lexer_T*
 */
lexer_T* init_lexer(char* src)
{
	lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
	lexer->src = src;
	lexer->src_size = strlen(src);
	lexer->iteration = 0;
	lexer->content = src[lexer->iteration];

	return lexer;
}

/* Function: lexer_advance
 * ---------------------------
 *   Simply makes the lexer go thorugh one
 *   iteration.
 *
 *   lexer : the coff language lexer
 * 
 *   returns : nothing
 */
void lexer_advance(lexer_T* lexer) 
{
	if (lexer->iteration < lexer->src_size && lexer->content != '\0') 
	{
		lexer->iteration += 1;
		lexer->content = lexer->src[lexer->iteration];
	}
}

/* Function: lexer_advance_current
 * ---------------------------
 *   Simply makes the lexer advance
 * 	 and sends back the given token.
 *   For examples of when you would like functionality 
 *   like this, see the bottom-most function.
 *
 *   lexer : the coff language lexer
 *   token : the token you want
 * 
 *   returns : token_T*
 */
token_T* lexer_advance_with(lexer_T* lexer, token_T* token) 
{
	lexer_advance(lexer);
	return token;
}

/* Function: lexer_advance_current
 * ---------------------------
 *   Function that cunstructs a token and make the
 *   lexer advance.
 *
 *   lexer : the coff language lexer
 *   type  : the type of token (for example: TOKEN_EOF)
 * 
 *   returns : token_T*
 */
token_T* lexer_advance_current(lexer_T* lexer, int type) 
{
	char* value = calloc(2, sizeof(char));
	value[0] = lexer->content;
	value[1] = '\0';

	token_T* token = init_token(value, type);
	lexer_advance(lexer);

	return token;
}

/* Function: lexer_skip_whitespace
 * ---------------------------
 *   Function that allows the lexer to ignore certain chars
 *   such as tabs, spaces, new lines etc.
 *
 *   lexer : the coff language lexer
 * 
 *   returns : nothing
 */
void lexer_skip_whitespace(lexer_T* lexer) 
{
	// 13 - carriage return, 10 - new line
	while(lexer->content == 13 || lexer->content == 10 
		|| lexer->content == ' ' || lexer->content == '\t') 
	{
		lexer_advance(lexer);
	}
}

/* Function: lexer_parse_id
 * ---------------------------
 *   Returns a token containing chars found in lexer
 *
 *   lexer : the coff language lexer
 * 
 *   returns : token_T*
 */
token_T* lexer_parse_id(lexer_T* lexer) 
{
	char* value = calloc(1, sizeof(char));
	while(isalpha(lexer->content))
	{
		value = realloc(value, (strlen(value) + 2) * sizeof(char));
		strcat(value, (char[]){lexer->content, 0});
		lexer_advance(lexer);
	}

	return init_token(value, TOKEN_ID);
}

/* Function: lexer_parse_number
 * ---------------------------
 *   Returns a token containing integer found in lexer
 *
 *   lexer : the coff language lexer
 * 
 *   returns : token_T*
 */
token_T* lexer_parse_number(lexer_T* lexer) 
{
	char* value = calloc(1, sizeof(char));
	while(isdigit(lexer->content))
	{
		value = realloc(value, (strlen(value) + 2) * sizeof(char));
		strcat(value, (char[]){lexer->content, 0});
		lexer_advance(lexer);
	}

	return init_token(value, TOKEN_INT);
}

/* Function: lexer_peek
 * ---------------------------
 *   Returns char that is offset iterations away from current char
 *
 *   lexer : the coff language lexer
 *   offset: the offset from current char in lexer
 * 
 *   returns : char
 */
char lexer_peek(lexer_T* lexer, int offset) 
{
	return lexer->src[MIN(lexer->iteration + offset, lexer->src_size)];
}

/* Function: lexer_next_token
 * ---------------------------
 *   Returns token based on symbol found in coff source-code
 *
 *   lexer: the coff language lexer
 * 
 *   returns : token_T*
 */
token_T* lexer_next_token(lexer_T* lexer)
{
	while(lexer->content != '\0')
	{
		if (isalpha(lexer->content)) 
		{
			return lexer_advance_with(lexer, lexer_parse_id(lexer));
		}

		if (isdigit(lexer->content))
		{
			return lexer_advance_with(lexer, lexer_parse_number(lexer));
		}

		switch (lexer->content)
		{
			case ':': return lexer_advance_current(lexer, TOKEN_EQUALS);

			case '(': return lexer_advance_current(lexer, TOKEN_OPAREN);
			case ')': return lexer_advance_current(lexer, TOKEN_CPAREN);

			case '[': return lexer_advance_current(lexer, TOKEN_OSQUAR);
			case ']': return lexer_advance_current(lexer, TOKEN_CSQUAR);

			case '{': return lexer_advance_current(lexer, TOKEN_OBRACE);
			case '}': return lexer_advance_current(lexer, TOKEN_CBRACE);

			case ',': return lexer_advance_current(lexer, TOKEN_COMMA);

			case '<': return lexer_advance_current(lexer, TOKEN_RETURN);
			case '>': return lexer_advance_current(lexer, TOKEN_PRINT);

			case ';': return lexer_advance_current(lexer, TOKEN_SEMI);
			case '\0': break;
			default: printf("Unexpected char %c found in lexer...\n", lexer->content); exit(1); break;
		}
	}

	return init_token(0, TOKEN_EOF);
}