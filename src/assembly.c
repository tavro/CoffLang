#include "include/assembly.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* assembly_frontend_int(AST_T* ast)
{

}

char* assembly_frontend_call(AST_T* ast)
{
	char* s = calloc(1, sizeof(char));

	if(strcmp(ast->name, "return") == 0)
	{
		AST_T* arg = (AST_T*) ast->value->children->size ? ast->value->children->items[0] : (void*) 0;
		const char* template = "mov $%d, %%eax\nret\n";
		char* ret_str = calloc(strlen(template) + 128, sizeof(char));
		sprintf(ret_str, template, arg ? arg->int_value : 0);
		s = realloc(s, (strlen(ret_str) + 1) * sizeof(char));
		strcat(s, ret_str);
	}

	return s;
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
	char* s = calloc(1, sizeof(char));
	if(ast->value->type == AST_FUNC) 
	{
		const char* template = ".globl %s\n%s:\n";
		s = realloc(s, (strlen(template) + (strlen(ast->name)*2) + 1) * sizeof(char));
		sprintf(s, template, ast->name, ast->name);

		AST_T* as_val = ast->value;

		char* as_val_val = assembly_frontend(as_val->value);

		s = realloc(s, (strlen(s) + strlen(as_val_val) + 1) * sizeof(char));
		strcat(s, as_val_val);
	}

	return s;
}

char* assembly_frontend_root(AST_T* ast)
{
	const char* section_text = ".section .text\n.globl _start\n_start:\ncall main\nmov \%eax, \%ebx\nmov $1, \%eax\n\n";
	char* value = (char*) calloc((strlen(section_text) + 128), sizeof(char));
	strcpy(value, section_text);

	char* next_value = assembly_frontend(ast);
	value = (char*) realloc(value, (strlen(value) + strlen(next_value) + 1) * sizeof(char));
	strcat(value, next_value);

	return value;
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