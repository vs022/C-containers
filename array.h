#ifndef ARRAY_H_
#define ARRAY_H_

#include <string.h>
#include <stdlib.h>
#include <assert.h>


#define ARRAY_TYPE(type) \
	struct { \
		type *elem_data; \
		size_t elem_count; \
		size_t elem_alloc; \
		size_t elem_reserve; \
	}

#define ARRAY_INIT_RESERVE(reserve) \
	{ \
		.elem_data = NULL, \
		.elem_count = 0, \
		.elem_alloc = 0, \
		.elem_reserve = (reserve) \
	}

#define ARRAY_INIT() \
	ARRAY_INIT_RESERVE(0)

#define TYPEDEF_ARRAY(type, array_type) \
	typedef ARRAY_TYPE(type) array_type

void arrayptr_set_size_(void *array, size_t elem_size, size_t new_size, size_t max_size);
bool arrayptr_bin_search_(const void *array, size_t *pos, size_t elem_size, const void *key,
	int (*compare_proc)(const void *key, const void *elem));

#define array_init(arr) \
	do { \
		(arr).elem_data = NULL; \
		(arr).elem_count = 0; \
		(arr).elem_alloc = 0; \
		(arr).elem_reserve = 0; \
	} while (0)

#define array_free(arr) \
	do { \
		if ((arr).elem_data) \
			free((arr).elem_data); \
	} while (0)

#define array_reserve(arr, size) \
	do { \
		(arr).elem_reserve = (size); \
	} while (0)

#define array_get_size(arr) \
	((arr).elem_count)

#define array_get_data(arr) \
	((arr).elem_data)

#define array_is_empty(arr) \
	(!(arr).elem_count)

#define array_set_size(arr, size) \
	arrayptr_set_size_(&(arr), sizeof(*((arr).elem_data)), (size), \
		SIZE_MAX / sizeof(*((arr).elem_data)))

#define array_set_at(arr, pos, elem) \
	do { \
		assert((pos) < (arr).elem_count); \
		(arr).elem_data[(pos)] = (elem); \
	} while (0)

#define array_get_at(arr, pos) \
	(assert((pos) < (arr).elem_count), ((arr).elem_data[(pos)]))

#define array_add(arr, elem) \
	do { \
		size_t _pos = (arr).elem_count; \
		array_set_size((arr), (arr).elem_count + 1); \
		(arr).elem_data[_pos] = (elem); \
	} while (0)

#define array_add_buf(arr, elem, count) \
	do { \
		size_t _pos = (arr).elem_count; \
		size_t _n; \
		array_set_size((arr), (arr).elem_count + (count)); \
		for (_n = 0; _n < (count); ++_n) \
			(arr).elem_data[_pos + _n] = (elem)[_n]; \
	} while (0)

#define array_insert(arr, pos, elem) \
	do { \
		size_t _elem_move = (arr).elem_count - (pos); \
		assert((pos) <= (arr).elem_count); \
		array_set_size((arr), (arr).elem_count + 1); \
		if (_elem_move) \
			memmove(&((arr).elem_data[(pos) + 1]), &((arr).elem_data[(pos)]), \
				_elem_move * sizeof(*((arr).elem_data))); \
		(arr).elem_data[(pos)] = (elem); \
	} while (0)

#define array_remove(arr, pos) \
	do { \
		size_t _elem_move = (arr).elem_count - (pos) - 1; \
		assert((pos) < (arr).elem_count); \
		if (_elem_move) \
			memmove(&((arr).elem_data[(pos)]), &((arr).elem_data[(pos) + 1]), \
				_elem_move * sizeof(*((arr).elem_data))); \
		array_set_size((arr), (arr).elem_count - 1); \
	} while (0)

#define array_remove_all(arr) \
	do { \
		if ((arr).elem_count) \
			array_set_size((arr), 0); \
	} while (0)

#define array_bin_search(arr, pos, key, cmp_proc) \
	arrayptr_bin_search_(&(arr), (pos), sizeof(*((arr).elem_data)), (key), (cmp_proc))

#endif
