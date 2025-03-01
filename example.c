#include "defs.h"
#include "list.h"
#include "deque.h"
#include "array.h"
#include "dyn_string.h"
#include <stdlib.h>
#include <stdio.h>


struct list_item {
	struct list_info link;
	int i;
};

TYPEDEF_ARRAY(int, int_array);

static void print_list(const char *header, struct list_info *list)
{
	struct list_item *item;
	size_t i;

	puts(header);
	i = 0;
	list_for_each_entry(item, *list, struct list_item, link) {
		printf("  list item %d: %d\n", i, item->i);
		i++;
	}
}

static void test_list()
{
	struct list_item data_pos[5];
	struct list_item data_neg[5];
	struct list_info list;
	size_t i;

	list_init(list);
	for (i = 0; i < ARRAY_SIZE(data_pos); i++) {
		data_pos[i].i = (int)(i + 1) * 10;
		list_add_tail(list, &data_pos[i].link);
	}
	for (i = 0; i < ARRAY_SIZE(data_neg); i++) {
		data_neg[i].i = -(int)(i + 1) * 10;
		list_add_head(list, &data_neg[i].link);
	}
	print_list("List before", &list);

	list_remove_head(list);
	list_remove_tail(list);
	print_list("List after", &list);
	puts("");
}

static void test_deque()
{
	DEQUE_TYPE(int) d;
	int buf[10];

	deque_init(d, buf, ARRAY_SIZE(buf));
	deque_put_head(d, 10);
	deque_put_head(d, 20);
	deque_put_tail(d, -10);
	deque_put_tail(d, -20);

	puts("Deque");
	while (!deque_is_empty(d)) {
		int i;
		deque_get_tail(d, i);
		printf("  %d\n", i);
	}
	puts("");
}

static void print_array(const char *header, int_array *a)
{
	size_t i;

	puts(header);
	for (i = 0; i < array_get_size(*a); i++) {
		printf("  %d\n", array_get_at(*a, i));
	}
}

static void test_array()
{
	int_array a = ARRAY_INIT();
	size_t i;

	for (i = 0; i < 10; i++) {
		array_add(a, (int)i);
	}
	print_array("Array before", &a);

	for (i = array_get_size(a); i >= 2; i -= 2) {
		array_remove(a, i - 1);
	}
	print_array("Array after", &a);
	puts("");

	array_free(a);
}

static void test_string()
{
	string_info s;

	string_init(&s);
	string_assign(&s, "This is dynamic null terminated string.\n");
	string_append_printf(&s, "Current string length is %zu bytes.\n",
		string_get_length(&s));
	puts(string_get_data(&s));
	string_free(&s);
}

int main()
{
	test_list();
	test_deque();
	test_array();
	test_string();

	return EXIT_SUCCESS;
}
