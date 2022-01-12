#ifndef COFF_AST_H
#define COFF_AST_H
#include "list.h"

typedef struct AST_STRUCT
{
	enum 
	{
		AST_COMP,
		AST_FUNC, //function definition
		AST_CALL, //function call
		AST_ASSIGNMENT,
		AST_TYPE, //definition type
		AST_VAR,
		AST_INT, 
		AST_STMT, //statement
		AST_NOOP, //no operation
	} type;

	list_T* children;
	char* name;
	struct AST_STRUCT* value;
	int int_value;
	int data_type;
} AST_T;

AST_T* init_ast(int type);

#endif