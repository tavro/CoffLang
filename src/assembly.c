#include "include/assembly.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static AST_T* var_lookup(list_T* list, const char* name)
{
	for (int i = 0; i < (int) list->size; i++)
	{
		AST_T* child_ast = (AST_T*) list->items[i];

		if(child_ast->type != AST_VAR || !child_ast->name) 
		{
			continue;
		}

		if(strcmp(child_ast->name, name) == 0)
		{
			return child_ast;
		}
	}

	return 0;
}

char* assembly_frontend_int(AST_T* ast, list_T* list)
{
	printf("Entered assembly_frontend_int!\n");
}

char* assembly_frontend_call(AST_T* ast, list_T* list)
{
	char* s = calloc(1, sizeof(char));

	if(strcmp(ast->name, "return") == 0)
	{
		AST_T* arg = (AST_T*) ast->value->children->size ? ast->value->children->items[0] : (void*) 0;
		char* var_s = calloc(3, sizeof(char));

		var_s[0] = '$';
		var_s[1] = '0';
		var_s[2] = '\0';

		if(arg)
		{
			char* assembly_var_s = assembly_frontend(arg, list);
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

char* assembly_frontend_compound(AST_T* ast, list_T* list)
{
	char* value = calloc(1, sizeof(char));
	for (int i = 0; i < (int) ast->children->size; i++)
	{
		AST_T* child_ast = (AST_T*) ast->children->items[i];
		char* next_value = assembly_frontend(child_ast, list);
		value = realloc(value, (strlen(next_value) + 1) * sizeof(char));
		strcat(value, next_value);
	}

	return value;
}

char* assembly_frontend_variable(AST_T* ast, list_T* list)
{
	char* s = calloc(1, sizeof(char));

	AST_T* var = var_lookup(list, ast->name);

	if(!var)
	{
		printf("%s is not defined in assembly frontend...\n", var->name);
		exit(1);
	}

	const char* template = "%d(%%esp)";
	s = realloc(s, (strlen(template) + 8) * sizeof(char));
	sprintf(s, template, var->int_value);

	return s;
}

char* assembly_frontend_assignment(AST_T* ast, list_T* list)
{
	char* s = calloc(1, sizeof(char));
	if(ast->value->type == AST_FUNC) 
	{
		const char* template = ".globl %s\n%s:\npushl %%ebp\nmovl %%esp, %%ebp\n";
		s = realloc(s, (strlen(template) + (strlen(ast->name)*2) + 1) * sizeof(char));
		sprintf(s, template, ast->name, ast->name);

		AST_T* as_val = ast->value;

		for (unsigned int i = 0; i < as_val->children->size; i++)
		{
			AST_T* farg = (AST_T*) as_val->children->items[i];
			AST_T* arg_var = init_ast(AST_VAR);
			arg_var->name = farg->name;
			arg_var->int_value = (4 * as_val->children->size) - (i*4);
			list_push(list, arg_var);
		}

		char* as_val_val = assembly_frontend(as_val->value, list);

		s = realloc(s, (strlen(s) + strlen(as_val_val) + 1) * sizeof(char));
		strcat(s, as_val_val);
	}

	return s;
}

char* assembly_frontend_root(AST_T* ast, list_T* list)
{
	const char* section_text = ".section .text\n.globl _start\n_start:\npushl 0(\%esp)\npushl 4(\%esp)\ncall main\naddl $4, \%esp\nmovl \%eax, \%ebx\nmovl $1, \%eax\n\n";
	char* value = (char*) calloc((strlen(section_text) + 128), sizeof(char));
	strcpy(value, section_text);

	char* next_value = assembly_frontend(ast, list);

	value = (char*) realloc(value, (strlen(value) + strlen(next_value) + 1) * sizeof(char));
	strcat(value, next_value);

	return value;
}

char* assembly_frontend_get(AST_T* ast, list_T* list) 
{
	AST_T* left = var_lookup(list, ast->name);
	char* left_as = assembly_frontend(left, list);
	
	AST_T* arg = (AST_T*) ast->value->children->size ? ast->value->children->items[0] : (void*) 0;
	
	const char* template = "%s, %%eax\nmovl %d(%%eax)";
	char* s = calloc(strlen(template) + strlen(left_as) + 128, sizeof(char));
	sprintf(s, template, left_as, (arg ? arg->int_value : 0) * 4);

	free(left_as);

	return s;
}

char* assembly_frontend(AST_T* ast, list_T* list) 
{
	
	char* next_value = 0;

	switch (ast->type)
	{
		case AST_COMP: next_value = assembly_frontend_compound(ast, list);break;
		case AST_ASSIGNMENT: next_value = assembly_frontend_assignment(ast, list); break;
		case AST_VAR: next_value = assembly_frontend_variable(ast, list); break;
		case AST_CALL: next_value = assembly_frontend_call(ast, list); break;
		case AST_INT: next_value = assembly_frontend_int(ast, list); break;
		case AST_GET: next_value = assembly_frontend_get(ast, list); break;
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