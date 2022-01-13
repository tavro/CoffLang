#include "include/assembly.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* assembly_frontend_int(AST_T* ast)
{

}

char* assembly_frontend_call(AST_T* ast)
{

}

char* assembly_frontend_compound(AST_T* ast)
{
	char* value = calloc(1, sizeof(char));
	for (int i = 0; i < (int) ast->children->size; i++)
	{
		AST_T* child_ast = (AST_T*) ast->children->items[i];
		char* next_value = assembly_frontend(child_ast);
		value = realloc(value, (strlen(next_value) + 1) * sizeof(char));
		strcat(value, next_value);
	}

	return value;
}

char* assembly_frontend_variable(AST_T* ast)
{

}

char* assembly_frontend_assignment(AST_T* ast)
{
	//TODO: Change this
	const char* example = "mov $128, \%eax";
	char* s = (char*) calloc(strlen(example) + 1, sizeof(char));
	strcat(s, example);
	return s;
}

char* assembly_frontend(AST_T* ast) 
{
	char* value = calloc(1, sizeof(char));
	char* next_value = 0;

	switch (ast->type)
	{
		case AST_COMP: next_value = assembly_frontend_compound(ast); break;
		case AST_ASSIGNMENT: next_value = assembly_frontend_assignment(ast); break;
		case AST_VAR: next_value = assembly_frontend_variable(ast); break;
		case AST_CALL: next_value = assembly_frontend_call(ast); break;
		case AST_INT: next_value = assembly_frontend_int(ast); break;
		default: 
		{
			printf("No frontend for AST type %d in assembly...\n", ast->type); exit(1);
		} break;
	}

	value = realloc(value, (strlen(next_value) + 1) * sizeof(char));
	strcat(value, next_value);

	return value;
}