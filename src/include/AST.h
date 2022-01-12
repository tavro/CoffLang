#ifndef COFF_AST_H
#define COFF_AST_H

typedef struct AST_STRUCT
{
	enum 
	{
		AST_COMP,
		AST_FUNC, //function definition
		AST_TYPE, //definition type
		AST_VAR, 
		AST_STMT, //statement
		AST_NOOP, //no operation
	} type;
} AST_T;

AST_T* init_ast(int type);

#endif