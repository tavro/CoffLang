#include "include/token.h"
#include <stdlib.h>

/* Function: init_token
 * ---------------------------
 *   Simply initialized and returns the token
 *
 *   value: the symbol making up the token (for example: ;)
 *   type : the token type        (for example: TOKEN_SEMI)
 * 
 *   returns : token_T*
 */
token_T* init_token(char* value, int type)
{
	token_T* token = calloc(1, sizeof(struct TOKEN_STRUCT));
	token->value = value;
	token->type = type;

	return token;
}