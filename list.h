#ifndef LIST_H_
#define LIST_H_

/*
 * Doubly linked list
 *
 *         prev  next
 * 0 <== item <==> item <==> ... <==> item ==> 0
 *       head                         tail
 */

struct list_info {
	struct list_info *next;
	struct list_info *prev;
};

#define LIST_INIT(list) {&(list), &(list)}

#define list_entry(elem, type, field) \
	container_of(elem, type, field)

#define list_entry_safe(elem, type, field) \
	(((elem) != NULL) ? list_entry((elem), type, field) : NULL)

#define list_for_each(cursor, list) \
	for ((cursor) = (list).next; (cursor) != &(list); (cursor) = (cursor)->next)

#define list_for_each_safe(cursor, cursor2, list) \
	for ((cursor) = (list).next, (cursor2) = (cursor)->next; \
		(cursor) != &(list); \
		(cursor) = (cursor2), (cursor2) = (cursor2)->next)

#define list_for_each_entry(elem, list, type, field) \
	for ((elem) = list_entry((list).next, type, field); \
		&((elem)->field) != &(list); \
		(elem) = list_entry((elem)->field.next, type, field))

#define list_for_each_entry_safe(elem, elem2, list, type, field) \
	for ((elem) = list_entry((list).next, type, field), \
			(elem2) = list_entry((elem)->field.next, type, field); \
		&((elem)->field) != &(list); \
		(elem) = (elem2), (elem2) = list_entry((elem2)->field.next, type, field))

#define list_init(list) \
	do { \
		(list).next = (list).prev = &(list); \
	} while (0)

#define list_empty(list) \
	list_init(list)

#define list_is_empty(list) \
	((list).next == &(list))

#define list_get_next(list, item) \
	(((item)->next == &(list)) ? NULL : (item)->next)

#define list_get_prev(list, item) \
	(((item)->prev == &(list)) ? NULL : (item)->prev)

#define list_get_head(list) \
	list_get_next((list), &(list))

#define list_get_tail(list) \
	list_get_prev((list), &(list))

#define list_insert_after(item, item_new) \
	do { \
		struct list_info *_next_item = (item)->next; \
		(item)->next = (item_new); \
		_next_item->prev = (item_new); \
		(item_new)->next = _next_item; \
		(item_new)->prev = (item); \
	} while (0)

#define list_insert_before(item, item_new) \
	do { \
		struct list_info *_prev_item = (item)->prev; \
		(item)->prev = (item_new); \
		_prev_item->next = (item_new); \
		(item_new)->next = (item); \
		(item_new)->prev = _prev_item; \
	} while (0)

#define list_add_head(list, item) \
	list_insert_after(&(list), (item))

#define list_add_tail(list, item) \
	list_insert_before(&(list), (item))

#define list_remove(item) \
	do { \
		struct list_info *_prev_item = (item)->prev; \
		struct list_info *_next_item = (item)->next; \
		_next_item->prev = _prev_item; \
		_prev_item->next = _next_item; \
	} while (0)

#define list_remove_head(list) \
	list_remove((list).next)

#define list_remove_tail(list) \
	list_remove((list).prev)

#endif
