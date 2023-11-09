#ifndef _MY_SNPRINTF_H_
#define _MY_SNPRINTF_H_
#include <stddef.h>

#define DEC 10
#define HEX 16

int my_snprintf(char *str, size_t n, const char *format, ...);

#endif
