#ifndef COFF_AST_H
#define COFF_AST_H

#include "list.h"

struct VISITOR_STRUCT;

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
		AST_STR,
		AST_STMT, //statement
		AST_GET,  //indexing
		AST_NOOP, //no operation
	} type;

	list_T* children;
	char* name;
	char* string_value;
	struct AST_STRUCT* value;
	int int_value;
	int data_type;
	struct AST_STRUCT* (*fptr)(struct VISITOR_STRUCT* visitor, struct AST_STRUCT* node, list_T* list);
} AST_T;

AST_T* init_ast(int type);

#endif