#ifndef COFF_VISITOR_H
#define COFF_VISITOR_H

#include "AST.h"
#include "list.h"

typedef struct VISITOR_STRUCT 
{
	AST_T* obj;
} visitor_T;

visitor_T* init_visitor();

AST_T* visitor_visit_assignment(visitor_T* visitor, AST_T* node, list_T* list);
AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node, list_T* list);
AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node, list_T* list);
AST_T* visitor_visit_function(visitor_T* visitor, AST_T* node, list_T* list);
AST_T* visitor_visit_call(visitor_T* visitor, AST_T* node, list_T* list);
AST_T* visitor_visit_int(visitor_T* visitor, AST_T* node, list_T* list);
AST_T* visitor_visit_get(visitor_T* visitor, AST_T* node, list_T* list);
AST_T* visitor_visit(visitor_T* visitor, AST_T* node, list_T* list);

#endif