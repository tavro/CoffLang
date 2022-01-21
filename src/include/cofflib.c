#include "include/cofflib.h"
#include <string.h>
#include <stdio.h>

/* Function: mkstr
 * ---------------------------
 *   allocates memory for string
 *
 *   str : the string to allocate memory for
 * 
 *   returns : char*
 */
static char* mkstr(const char* str) 
{
	char* out = (char*) calloc(strlen(str) + 1, sizeof(char));
	strcpy(out, str);

	return out;
}

/* Function: fptr_print
 * ---------------------------
 *   Generates assembly code for printing
 *
 *   visitor : 
 *   node : abstract syntax tree
 *   list : 
 * 
 *   returns : char*
 */
AST_T* fptr_print(visitor_T* visitor, AST_T* node, list_T* list)
{
	//TODO: Implement assembly print functionality
	return node;
} 

/* Function: library_register_fptr
 * ---------------------------
 *   Register the function pointer print function
 *
 *   list :
 *   name : name of the function
 *   function pointer : 
 * 
 *   returns : nothing
 */
void library_register_fptr(list_T* list, const char* name, AST_T* (*fptr)(visitor_T* visitor, AST_T* node, list_T* list))
{
	AST_T* fptr_print_var = init_ast(AST_VAR);
	fptr_print_var->name = mkstr(name);
	fptr_print_var->fptr = fptr;

	list_push(list, fptr_print_var);
}

/* Function: library_init
 * ---------------------------
 *   Initializes coff's inbuilt functions
 *
 *   list : 
 * 
 *   returns : nothing
 */
void library_init(list_T* list)
{
	library_register_fptr(list, "print", fptr_print);
}