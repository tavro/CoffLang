#ifndef COFF_COFFLIB_H
#define COFF_COFFLIB_H

#include "visitor.h"

AST_T* fptr_print(visitor_T* visitor, AST_T* node, list_T* list);

void library_register_fptr(list_T* list, const char* name, AST_T* (*fptr)(visitor_T* visitor, AST_T* node, list_T* list));
void library_init(list_T* list);

#endif