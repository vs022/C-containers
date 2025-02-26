#ifndef DEQUE_H_
#define DEQUE_H_


#define DEQUE_TYPE(type) \
	struct { \
		type *buf_begin; \
		type *buf_end; \
		type *head; \
		type *tail; \
		size_t buf_size; \
		size_t count; \
	}

#define DEQUE_INIT(buf, buf_len) \
	{ \
		.buf_begin = (buf), \
		.buf_end = (buf) + ((buf_len) - 1), \
		.head = (buf) + ((buf_len) - 1), \
		.tail = (buf), \
		.buf_size = (buf_len), \
		.count = 0 \
	}

#define TYPEDEF_DEQUE(type, deque_type) \
	typedef DEQUE_TYPE(type) deque_type

#define deque_init(deq, buf, buf_len) \
	do { \
		(deq).buf_begin = (deq).tail = (buf); \
		(deq).buf_end = (deq).head = ((buf) + ((buf_len) - 1)); \
		(deq).buf_size = (buf_len); \
		(deq).count = 0; \
	} while (0)

#define deque_empty(deq) \
	do { \
		(deq).head = (deq).buf_end; \
		(deq).tail = (deq).buf_begin; \
		(deq).count = 0; \
	} while (0)

#define deque_is_empty(deq) \
	((deq).count == 0)

#define deque_is_full(deq) \
	((deq).count == (deq).buf_size)

#define deque_get_buffer(deq) \
	((deq).buf_begin)

#define deque_get_buffer_size(deq) \
	((deq).buf_size)

#define deque_get_size(deq) \
	((deq).count)

#define deque_get_free(deq) \
	((deq).buf_size - (deq).count)


/* Deque head operations */

#define deque_peek_head(deq) \
	((!deque_is_empty(deq)) ? (deq).head : NULL)

#define deque_peek_empty_head(deq) \
	((!deque_is_full(deq)) ? \
		(((deq).head == (deq).buf_end) ? (deq).buf_begin : ((deq).head + 1)) : \
		NULL)

#define deque_put_head(deq, t) \
	do { \
		if (!deque_is_full(deq)) { \
			if ((deq).head == (deq).buf_end) \
				(deq).head = (deq).buf_begin; \
			else \
				++((deq).head); \
			*((deq).head) = (t); \
			++((deq).count); \
		} \
	} while (0)

#define deque_push_head(deq) \
	do { \
		if (!deque_is_full(deq)) { \
			if ((deq).head == (deq).buf_end) \
				(deq).head = (deq).buf_begin; \
			else \
				++((deq).head); \
			++((deq).count); \
		} \
	} while (0)

#define deque_put_buf_head(type, deq, buf, buf_len, put) \
	do { \
		const type *_put_buf = (buf); \
		size_t _deq_free = deque_get_free(deq); \
		size_t _n; \
		(put) = (buf_len); \
		if ((put) > _deq_free) \
			(put) = _deq_free; \
		_n = (put); \
		(deq).count += _n; \
		while (_n--) { \
			if ((deq).head == (deq).buf_end) \
				(deq).head = (deq).buf_begin; \
			else \
				++((deq).head); \
			*((deq).head) = *(_put_buf++); \
		} \
	} while (0)

#define deque_put_reversed_buf_head(type, deq, buf, buf_len, put) \
	do { \
		const type *_put_buf = (buf) + (buf_len); \
		size_t _deq_free = deque_get_free(deq); \
		size_t _n; \
		(put) = (buf_len); \
		if ((put) > _deq_free) \
			(put) = _deq_free; \
		_n = (put); \
		(deq).count += _n; \
		while (_n--) { \
			if ((deq).head == (deq).buf_end) \
				(deq).head = (deq).buf_begin; \
			else \
				++((deq).head); \
			*((deq).head) = *(--_put_buf); \
		} \
	} while (0)

#define deque_get_head(deq, t) \
	do { \
		if (!deque_is_empty(deq)) { \
			(t) = *((deq).head); \
			if ((deq).head == (deq).buf_begin) \
				(deq).head = (deq).buf_end; \
			else \
				--((deq).head); \
			--((deq).count); \
		} \
	} while (0)

#define deque_pop_head(deq) \
	do { \
		if (!deque_is_empty(deq)) { \
			if ((deq).head == (deq).buf_begin) \
				(deq).head = (deq).buf_end; \
			else \
				--((deq).head); \
			--((deq).count); \
		} \
	} while (0)

#define deque_get_buf_head(type, deq, buf, buf_len, got) \
	do { \
		type *_get_buf = (buf); \
		size_t _deq_size = deque_get_size(deq); \
		size_t _n; \
		(got) = (buf_len); \
		if ((got) > _deq_size) \
			(got) = _deq_size; \
		_n = (got); \
		(deq).count -= _n; \
		while (_n--) { \
			*(_get_buf++) = *((deq).head); \
			if ((deq).head == (deq).buf_begin) \
				(deq).head = (deq).buf_end; \
			else \
				--((deq).head); \
		} \
	} while (0)

#define deque_get_reversed_buf_head(type, deq, buf, buf_len, got) \
	do { \
		type *_get_buf = (buf) + (buf_len); \
		size_t _deq_size = deque_get_size(deq); \
		size_t _n; \
		(got) = (buf_len); \
		if ((got) > _deq_size) \
			(got) = _deq_size; \
		_n = (got); \
		(deq).count -= _n; \
		while (_n--) { \
			*(--_get_buf) = *((deq).head); \
			if ((deq).head == (deq).buf_begin) \
				(deq).head = (deq).buf_end; \
			else \
				--((deq).head); \
		} \
	} while (0)

/* Deque tail operations */

#define deque_peek_tail(deq) \
	((!deque_is_empty(deq)) ? (deq).tail : NULL)

#define deque_peek_empty_tail(deq) \
	((!deque_is_full(deq)) ? \
		(((deq).tail == (deq).buf_begin) ? (deq).buf_end : ((deq).tail - 1)) : \
		NULL)

#define deque_put_tail(deq, t) \
	do { \
		if (!deque_is_full(deq)) { \
			if ((deq).tail == (deq).buf_begin) \
				(deq).tail = (deq).buf_end; \
			else \
				--((deq).tail); \
			*((deq).tail) = (t); \
			++((deq).count); \
		} \
	} while (0)

#define deque_push_tail(deq) \
	do { \
		if (!deque_is_full(deq)) { \
			if ((deq).tail == (deq).buf_begin) \
				(deq).tail = (deq).buf_end; \
			else \
				--((deq).tail); \
			++((deq).count); \
		} \
	} while (0)

#define deque_put_buf_tail(type, deq, buf, buf_len, put) \
	do { \
		const type *_put_buf = (buf); \
		size_t _deq_free = deque_get_free(deq); \
		size_t _n; \
		(put) = (buf_len); \
		if ((put) > _deq_free) \
			(put) = _deq_free; \
		_n = (put); \
		(deq).count += _n; \
		while (_n--) { \
			if ((deq).tail == (deq).buf_begin) \
				(deq).tail = (deq).buf_end; \
			else \
				--((deq).tail); \
			*((deq).tail) = *(_put_buf++); \
		} \
	} while (0)

#define deque_put_reversed_buf_tail(type, deq, buf, buf_len, put) \
	do { \
		const type *_put_buf = (buf) + (buf_len); \
		size_t _deq_free = deque_get_free(deq); \
		size_t _n; \
		(put) = (buf_len); \
		if ((put) > _deq_free) \
			(put) = _deq_free; \
		_n = (put); \
		(deq).count += _n; \
		while (_n--) { \
			if ((deq).tail == (deq).buf_begin) \
				(deq).tail = (deq).buf_end; \
			else \
				--((deq).tail); \
			*((deq).tail) = *(--_put_buf); \
		} \
	} while (0)

#define deque_get_tail(deq, t) \
	do { \
		if (!deque_is_empty(deq)) { \
			(t) = *((deq).tail); \
			if ((deq).tail == (deq).buf_end) \
				(deq).tail = (deq).buf_begin; \
			else \
				++((deq).tail); \
			--((deq).count); \
		} \
	} while (0)

#define deque_pop_tail(deq) \
	do { \
		if (!deque_is_empty(deq)) { \
			if ((deq).tail == (deq).buf_end) \
				(deq).tail = (deq).buf_begin; \
			else \
				++((deq).tail); \
			--((deq).count); \
		} \
	} while (0)

#define deque_get_buf_tail(type, deq, buf, buf_len, got) \
	do { \
		type *_get_buf = (buf); \
		size_t _deq_size = deque_get_size(deq); \
		size_t _n; \
		(got) = (buf_len); \
		if ((got) > _deq_size) \
			(got) = _deq_size; \
		_n = (got); \
		(deq).count -= _n; \
		while (_n--) { \
			*(_get_buf++) = *((deq).tail); \
			if ((deq).tail == (deq).buf_end) \
				(deq).tail = (deq).buf_begin; \
			else \
				++((deq).tail); \
		} \
	} while (0)

#define deque_get_reversed_buf_tail(type, deq, buf, buf_len, got) \
	do { \
		type *_get_buf = (buf) + (buf_len); \
		size_t _deq_size = deque_get_size(deq); \
		size_t _n; \
		(got) = (buf_len); \
		if ((got) > _deq_size) \
			(got) = _deq_size; \
		_n = (got); \
		(deq).count -= _n; \
		while (_n--) { \
			*(--_get_buf) = *((deq).tail); \
			if ((deq).tail == (deq).buf_end) \
				(deq).tail = (deq).buf_begin; \
			else \
				++((deq).tail); \
		} \
	} while (0)

#endif
