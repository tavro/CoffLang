#include "include/token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

const char* token_type_to_str(int type) 
{
    switch (type) 
    {
        case TOKEN_ID: return "TOKEN_ID";
        case TOKEN_INT: return "TOKEN_INT";
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_SEMI: return "TOKEN_SEMI";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_EQUALS: return "TOKEN_EQUALS";
        case TOKEN_OPAREN: return "TOKEN_OPAREN";
        case TOKEN_CPAREN: return "TOKEN_CPAREN";
        case TOKEN_OSQUAR: return "TOKEN_OSQUAR";
        case TOKEN_CSQUAR: return "TOKEN_CSQUAR";
        case TOKEN_OBRACE: return "TOKEN_OBRACE";
        case TOKEN_CBRACE: return "TOKEN_CBRACE";
        case TOKEN_RETURN: return "TOKEN_RETURN";
        case TOKEN_PRINT: return "TOKEN_PRINT";
    }

    return "unstringable_token";
}

char* token_to_str(token_T* token) 
{
    const char* type_str = token_type_to_str(token->type);
    const char* template = "<type='%s', int_type='%d', value='%s'>";

    char* str = calloc(strlen(type_str) + strlen(template) + 8, sizeof(char));
    sprintf(str, template, type_str, token->type, token->value);

    return str;
}