#include "include/list.h"

/* Function: init_list
 * ---------------------------
 *   Simply initialize and returns the list
 *
 *   item_size : size of item
 * 
 *   returns : list_T*
 */
list_T* init_list(size_t item_size)
{
	list_T* list = calloc(1, sizeof(struct LIST_STRUCT));
	list->size = 0;
	list->item_size = item_size;
	list->items = 0;

	return list;
}

/* Function: list_push
 * ---------------------------
 *   Pushes item to list
 *
 *   list : the list to push to
 *   item : the item to push to the list
 * 
 *   returns : nothing
 */
void list_push(list_T* list, void* item) 
{
	list->size += 1;

	if (!list->items) 
	{
		list->items = calloc(1, list->item_size);
	}
	else 
	{
		list->items = realloc(list->items, (list->size * list->item_size));
	}
	list->items[list->size-1] = item;
}