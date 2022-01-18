#include "include/AST.h"
#include <stdlib.h>

/* Function: init_ast
 * ---------------------------
 *   Simply initialize and returns the AST
 *
 *   type : AST type
 * 
 *   returns : AST_T*
 */
AST_T* init_ast(int type)
{
	AST_T* ast = calloc(1, sizeof(struct AST_STRUCT));
	ast->type = type;

	if(type == AST_COMP)
	{
		ast->children = init_list(sizeof(struct AST_STRUCT*));
	}

	return ast;
}