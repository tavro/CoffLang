#ifndef COFF_ASSEMBLY_H
#define COFF_ASSEMBLY_H

#include "AST.h"

char* assembly_frontend(AST_T* ast, list_T* list);
char* assembly_frontend_int(AST_T* ast, list_T* list);
char* assembly_frontend_get(AST_T* ast, list_T* list);
char* assembly_frontend_call(AST_T* ast, list_T* list);
char* assembly_frontend_root(AST_T* ast, list_T* list);
char* assembly_frontend_compound(AST_T* ast, list_T* list);
char* assembly_frontend_variable(AST_T* ast, list_T* list);
char* assembly_frontend_assignment(AST_T* ast, list_T* list);

#endif