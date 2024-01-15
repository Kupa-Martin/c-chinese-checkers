#ifndef __MACRO_UTILS_H__
#define __MACRO_UTILS_H__

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))
#define STRINGIFY(x) #x
#define LENGTH(str) (sizeof(str) - 1)
#define COUNT_DIGITS(n) (LENGTH(STRINGIFY(n)))

#endif