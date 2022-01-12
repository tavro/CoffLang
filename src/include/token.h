#ifndef COFF_TOKEN_H
#define COFF_TOKEN_H

typedef struct TOKEN_STRUCT
{
	char* value;

	// All the tokens currently handeled by the coff compiler
	enum
	{
		TOKEN_ID,
		TOKEN_INT,
		TOKEN_EOF, 
		TOKEN_SEMI, 	// ;
		TOKEN_COMMA, 	// ,
		TOKEN_EQUALS,	// :
		TOKEN_OPAREN,	// (
		TOKEN_CPAREN,	// )
		TOKEN_OSQUAR,	// {
		TOKEN_CSQUAR,	// }
		TOKEN_OBRACE,	// [
		TOKEN_CBRACE,	// ]
		TOKEN_RETURN,	// <
		TOKEN_PRINT,	// >
	} type;

} token_T;

token_T* init_token(char* value, int type);

const char* token_type_to_str(int type);
char* token_to_str(token_T* token);

#endif