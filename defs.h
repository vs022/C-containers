#ifndef DEFS_H_
#define DEFS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>


#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CLAMP(val, min, max) \
	((val) < (min) ? (min) : \
		((val) > (max) ? (max) : (val)))
#define ROUND_UP(x, n) \
	(((x) >= 0) ? \
		((((x) + (n) - 1) / (n)) * (n)) : \
		(((x) / (n)) * (n)))
#define ROUND_DOWN(x, n) \
	(((x) >= 0) ? \
		(((x) / (n)) * (n)) : \
		((((x) - (n) + 1) / (n)) * (n)))

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - offsetof(type, member)))

#endif
