#include "include/visitor.h"
#include "include/cofflib.h"

#include <stdio.h>
#include <string.h>

/* Function: var_lookup
 * ---------------------------
 *   Looks up given variable in 'var table'
 *
 *   list :  
 *   name : 
 * 
 *   returns : AST_T*
 */
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

/* Function: init_visitor
 * ---------------------------
 *   Description
 *
 *   returns : visitor_T*
 */
visitor_T* init_visitor() 
{
	visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
	visitor->obj = init_ast(AST_COMP);

	library_init(visitor->obj->children);

	return visitor;
}

/* Function: visitor_visit_assignment
 * ---------------------------
 *   Description
 *
 *   visitor :  
 *   node :
 *   list : 
 * 
 *   returns : AST_T*
 */
AST_T* visitor_visit_assignment(visitor_T* visitor, AST_T* node, list_T* list)
{
	AST_T* var = init_ast(AST_ASSIGNMENT);
	var->name = node->name;
	var->value = visitor_visit(visitor, node->value, list);
	list_push(list, var);

	//return var;
	return node;
}

/* Function: visitor_visit_compound
 * ---------------------------
 *   Description
 *
 *   visitor :  
 *   node :
 *   list : 
 * 
 *   returns : AST_T*
 */
AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node, list_T* list)
{
	AST_T* compound = init_ast(AST_COMP);

	for (unsigned int i = 0; i < node->children->size; i++)
	{
		list_push(compound->children, visitor_visit(visitor, (AST_T*) node->children->items[i], list));
	}

	return compound;
}

/* Function: visitor_visit_variable
 * ---------------------------
 *   Description
 *
 *   visitor :  
 *   node :
 *   list :
 * 
 *   returns : AST_T*
 */
AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node, list_T* list)
{
	AST_T* var = var_lookup(visitor->obj->children, node->name);

	if(var) 
	{
		return var;
	}

	return node;
}

/* Function: visitor_visit_function
 * ---------------------------
 *   Description
 *
 *   visitor :  
 *   node :
 *   list :
 * 
 *   returns : AST_T*
 */
AST_T* visitor_visit_function(visitor_T* visitor, AST_T* node, list_T* list)
{
	AST_T* func = init_ast(AST_FUNC);
	func->children = init_list(sizeof(struct AST_STRUCT*));
	func->value = init_ast(AST_COMP);

	for (unsigned int i = 0; i < node->children->size; i++)
	{
		list_push(func->children, (AST_T*) node->children->items[i]);
	}

	for (unsigned int i = 0; i < node->value->children->size; i++)
	{
		list_push(func->value->children, visitor_visit(visitor, (AST_T*) node->value->children->items[i], list));
	}

	return func;
}

/* Function: visitor_visit_call
 * ---------------------------
 *   Description
 *
 *   visitor :  
 *   node :
 *   list :
 * 
 *   returns : AST_T*
 */
AST_T* visitor_visit_call(visitor_T* visitor, AST_T* node, list_T* list)
{
	AST_T* var = var_lookup(visitor->obj->children, node->name);

	if (var) 
	{
		if (var->fptr)
		{
			return var->fptr(visitor, var, node->value->children);
		}
	}

	return node;
}

/* Function: visitor_visit_int
 * ---------------------------
 *   Description
 *
 *   visitor :  
 *   node :
 *   list :
 * 
 *   returns : AST_T*
 */
AST_T* visitor_visit_int(visitor_T* visitor, AST_T* node, list_T* list)
{
	return node;
}

/* Function: visitor_visit_get
 * ---------------------------
 *   Description
 *
 *   visitor :  
 *   node :
 *   list :
 * 
 *   returns : 
 */
AST_T* visitor_visit_get(visitor_T* visitor, AST_T* node, list_T* list)
{
	return node;
}

/* Function: visitor_visit
 * ---------------------------
 *   Description
 *
 *   visitor :  
 *   node :
 *   list :
 * 
 *   returns : AST_T*
 */
AST_T* visitor_visit(visitor_T* visitor, AST_T* node, list_T* list)
{
	switch (node->type)
	{
		case AST_COMP: return visitor_visit_compound(visitor, node, list); break;
		case AST_ASSIGNMENT: return visitor_visit_assignment(visitor, node, list); break;
		case AST_VAR: return visitor_visit_variable(visitor, node, list); break;
		case AST_CALL: return visitor_visit_call(visitor, node, list); break;
		case AST_INT: return visitor_visit_int(visitor, node, list); break;
		case AST_GET: return visitor_visit_get(visitor, node, list); break;
		case AST_FUNC: return visitor_visit_function(visitor, node, list); break;
		default: 
		{
			printf("Visitor can't handle AST type %d...\n", node->type); exit(1);
		} break;
	}

	return node;
}
