#include "defs.h"
#include "array.h"


#ifndef ARRAY_MIN_ALLOC_SIZE
#define ARRAY_MIN_ALLOC_SIZE 16
#endif

TYPEDEF_ARRAY(void, array_ptr);


static size_t calc_alloc_size(size_t req_size, size_t max_size)
{
	size_t ret, t;

	_Static_assert((ARRAY_MIN_ALLOC_SIZE & (ARRAY_MIN_ALLOC_SIZE - 1)) == 0);

	if (req_size > max_size) {
		abort();
	}

	ret = ARRAY_MIN_ALLOC_SIZE;
	while (ret) {
		if (ret >= req_size) {
			goto out;
		}
		ret <<= 1;
	}
	t = ARRAY_MIN_ALLOC_SIZE;
	ret = (SIZE_MAX / 2 + (size_t)1) | (size_t)ARRAY_MIN_ALLOC_SIZE;
	while (t) {
		if (ret >= req_size) {
			goto out;
		}
		t <<= 1;
		ret |= t;
	}
	ret = SIZE_MAX;
out:
	if (ret > max_size)
		ret = max_size;
	return ret;
}

void arrayptr_set_size_(void *array, size_t elem_size, size_t new_size, size_t max_size)
{
	array_ptr *arr = array;
	size_t reserve;
	void *new_data;

	arr->elem_count = new_size;
	reserve = MAX(new_size, arr->elem_reserve);

	if (!reserve) {
		if (arr->elem_data) {
			free(arr->elem_data);
			arr->elem_data = NULL;
			arr->elem_alloc = 0;
		}
	} else if ((reserve > arr->elem_alloc) ||
		(reserve <= (arr->elem_alloc / 2)))
	{
		reserve = calc_alloc_size(reserve, max_size);
		if (reserve != arr->elem_alloc) {
			new_data = realloc(arr->elem_data, reserve * elem_size);
			if (new_data == NULL) {
				if (new_size > arr->elem_alloc) {
					abort();
				}
			} else {
				arr->elem_data = new_data;
				arr->elem_alloc = reserve;
			}
		}
	}
}

bool arrayptr_bin_search_(const void *array, size_t *pos, size_t elem_size, const void *key,
	int (*compare_proc)(const void *key, const void *elem))
{
	const array_ptr *arr = array;
	const char *base;
	size_t count, count2, cur;
	int cmp_res;

	base = arr->elem_data;
	count = arr->elem_count;

	cur = 0;
	while (count) {
		count2 = cur + (count / 2);
		cmp_res = (*compare_proc)(key, base + (count2 * elem_size));
		if (cmp_res > 0) {
			cur = count2 + 1;
			--count;
		}
		count /= 2;
	}
	if (pos != NULL)
		*pos = cur;
	return (cur != arr->elem_count) &&
		((*compare_proc)(key, base + (cur * elem_size)) == 0);
}
