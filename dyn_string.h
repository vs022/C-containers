#ifndef DYN_STRING_H_
#define DYN_STRING_H_

#include "array.h"
#include <string.h>
#include <stdarg.h>
#include <assert.h>


#ifndef STRING_DEFAULT_RESERVE
#define STRING_DEFAULT_RESERVE 256
#endif

#define STRING_INIT_RESERVE(reserve) \
	ARRAY_INIT_RESERVE(reserve)

#define STRING_INIT() \
	STRING_INIT_RESERVE(STRING_DEFAULT_RESERVE)

typedef ARRAY_TYPE(char) string_info;


inline void string_init(string_info *str)
{
	array_init(*str);
	array_reserve(*str, STRING_DEFAULT_RESERVE);
}

inline void string_free(string_info *str)
{
	array_free(*str);
}

inline void string_reserve(string_info *str, size_t size)
{
	array_reserve(*str, size);
}

inline size_t string_get_length(const string_info *str)
{
	size_t len;

	len = array_get_size(*str);
	if (len)
		--len;
	return len;
}

#define string_get_data(str) \
	(array_is_empty(*(str)) ? \
		"" : \
		array_get_data(*(str)))

inline bool string_is_empty(const string_info *str)
{
	return array_is_empty(*str);
}

inline void string_empty(string_info *str)
{
	array_remove_all(*str);
}

void string_set_length(string_info *str, size_t len);

void string_assign_buf(string_info *str, const char *buf, size_t len);

inline void string_assign(string_info *str, const char *s)
{
	assert(s);
	string_assign_buf(str, s, strlen(s));
}

void string_append_buf(string_info *str, const char *buf, size_t len);

inline void string_append(string_info *str, const char *s)
{
	assert(s);
	string_append_buf(str, s, strlen(s));
}

inline void string_append_char(string_info *str, char c)
{
	string_append_buf(str, &c, 1);
}

void string_vprintf(string_info *str, const char *fmt_str, va_list ap);

void string_append_vprintf(string_info *str, const char *fmt_str, va_list ap);

inline void string_printf(string_info *str, const char *fmt_str, ...)
{
	va_list args;

	assert(fmt_str);

	va_start(args, fmt_str);
	string_vprintf(str, fmt_str, args);
	va_end(args);
}

inline void string_append_printf(string_info *str, const char *fmt_str, ...)
{
	va_list args;

	assert(fmt_str);

	va_start(args, fmt_str);
	string_append_vprintf(str, fmt_str, args);
	va_end(args);
}

#endif
