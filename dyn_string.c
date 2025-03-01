#include "defs.h"
#include "dyn_string.h"
#include <stdio.h>


#define STRING_PRINTF_BUF_LEN 2048

extern void string_init(string_info *str);

extern void string_free(string_info *str);

extern void string_reserve(string_info *str, size_t size);

extern size_t string_get_length(const string_info *str);

extern bool string_is_empty(const string_info *str);

extern void string_empty(string_info *str);

void string_set_length(string_info *str, size_t len)
{
	bool was_empty;

	if (len) {
		was_empty = array_is_empty(*str);
		array_set_size(*str, len + 1);
		if (was_empty)
			array_set_at(*str, 0, '\0');
		array_set_at(*str, len, '\0');
	} else {
		string_empty(str);
	}
}

void string_assign_buf(string_info *str, const char *buf, size_t len)
{
	char *data;

	if (len) {
		assert(buf);
		array_set_size(*str, len + 1);
		data = array_get_data(*str);
		memcpy(data, buf, len);
		data[len] = '\0';
	} else {
		string_empty(str);
	}
}

extern void string_assign(string_info *str, const char *s);

void string_append_buf(string_info *str, const char *buf, size_t len)
{
	char *data;
	size_t old_len;

	if (len) {
		assert(buf);
		old_len = string_get_length(str);
		array_set_size(*str, old_len + len + 1);
		data = array_get_data(*str);
		memcpy(&data[old_len], buf, len);
		data[old_len + len] = '\0';
	}
}

extern void string_append(string_info *str, const char *s);

extern void string_append_char(string_info *str, char c);

void string_vprintf(string_info *str, const char *fmt_str, va_list ap)
{
	char *data;
	char buf[STRING_PRINTF_BUF_LEN];
	int len, len2;

	assert(fmt_str);

	len = vsnprintf(buf, sizeof(buf), fmt_str, ap);
	if (len < 0)
		goto err;
	if (len < sizeof(buf)) {
		string_assign_buf(str, buf, (size_t)len);
	} else {
		/* len >= sizeof(buf) --- small buffer */
		++len;
		array_set_size(*str, (size_t)len);
		data = array_get_data(*str);
		len2 = vsnprintf(data, (size_t)len, fmt_str, ap);
		if ((len2 < 0) || (len2 >= len))
			goto err;
	}
	return;
err:
	string_empty(str);
	return;
}

void string_append_vprintf(string_info *str, const char *fmt_str, va_list ap)
{
	char *data;
	char buf[STRING_PRINTF_BUF_LEN];
	int len, len2;
	size_t old_len;

	assert(fmt_str);

	len = vsnprintf(buf, sizeof(buf), fmt_str, ap);
	if (len < 0)
		goto err;
	if (len < sizeof(buf)) {
		string_append_buf(str, buf, (size_t)len);
	} else {
		/* len >= sizeof(buf) */
		old_len = string_get_length(str);
		++len;
		array_set_size(*str, old_len + (size_t)len);
		data = array_get_data(*str);
		len2 = vsnprintf(&data[old_len], (size_t)len, fmt_str, ap);
		if ((len2 < 0) || (len2 >= len))
			goto err;
	}
	return;
err:
	string_empty(str);
	return;
}

extern void string_printf(string_info *str, const char *fmt_str, ...);

extern void string_append_printf(string_info *str, const char *fmt_str, ...);
