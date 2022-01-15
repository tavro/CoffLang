#include "include/assembly.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* assembly_frontend_int(AST_T* ast)
{
	printf("Entered assembly_frontend_int!\n");
}

char* assembly_frontend_call(AST_T* ast)
{
	char* s = calloc(1, sizeof(char));

	if(strcmp(ast->name, "return") == 0)
	{
		AST_T* arg = (AST_T*) ast->value->children->size ? ast->value->children->items[0] : (void*) 0;
		char* var_s = calloc(3, sizeof(char));

		var_s[0] = '$';
		var_s[1] = '0';
		var_s[2] = '\0';

		if(arg && arg->type == AST_VAR)
		{
			char* assembly_var_s = assembly_frontend_variable(arg, 8);
			var_s = realloc(var_s, (strlen(assembly_var_s) + 1) * sizeof(char));
			strcpy(var_s, assembly_var_s);
			free(assembly_var_s);
		}

		const char* template = "movl %s, %%eax\nmovl %%ebp, %%esp\npopl %%ebp\n\nret\n";
		char* ret_str = calloc(strlen(template) + 128, sizeof(char));
		sprintf(ret_str, template, var_s);
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

char* assembly_frontend_variable(AST_T* ast, int id)
{
	char* s = calloc(1, sizeof(char));

	if(ast->type == AST_INT) 
	{
		const char* template = "$%d";
		s = realloc(s, (strlen(template) + 256) * sizeof(char));
		sprintf(s, template, ast->int_value);
	}
	else 
	{
		const char* template = "%d(%%esp)";
		s = realloc(s, (strlen(template) + 8) * sizeof(char));
		sprintf(s, template, id);
	}

	return s;
}

char* assembly_frontend_assignment(AST_T* ast)
{
	char* s = calloc(1, sizeof(char));
	if(ast->value->type == AST_FUNC) 
	{
		const char* template = ".globl %s\n%s:\npushl %%ebp\nmovl %%esp, %%ebp\n";
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
	const char* section_text = ".section .text\n.globl _start\n_start:\npushl 0(\%esp)\ncall main\naddl $4, \%esp\nmovl \%eax, \%ebx\nmovl $1, \%eax\n\n";
	char* value = (char*) calloc((strlen(section_text) + 128), sizeof(char));
	strcpy(value, section_text);

	char* next_value = assembly_frontend(ast);

	value = (char*) realloc(value, (strlen(value) + strlen(next_value) + 1) * sizeof(char));
	strcat(value, next_value);

	return value;
}

char* assembly_frontend_get(AST_T* ast) 
{
	char* s = calloc(1, sizeof(char));
	//AST_T* arg = (AST_T*) ast->value->children->size ? ast->value->children->items[0] : (void*) 0;
	return s;
}

char* assembly_frontend(AST_T* ast) 
{
	
	char* next_value = 0;

	switch (ast->type)
	{
		case AST_COMP: next_value = assembly_frontend_compound(ast);break;
		case AST_ASSIGNMENT: next_value = assembly_frontend_assignment(ast); break;
		case AST_VAR: next_value = assembly_frontend_variable(ast, 0); break;
		case AST_CALL: next_value = assembly_frontend_call(ast); break;
		case AST_INT: next_value = assembly_frontend_int(ast); break;
		case AST_GET: next_value = assembly_frontend_get(ast); break;
		default: 
		{
			printf("No frontend for AST type %d in assembly...\n", ast->type); exit(1);
		} break;
	}

	char* value = calloc(1, sizeof(char));
	value = realloc(value, (strlen(next_value) + 1) * sizeof(char));

	strcat(value, next_value);

	return value;
}