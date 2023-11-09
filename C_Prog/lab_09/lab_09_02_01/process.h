#ifndef _PROCESS_H_
#define _PROCESS_H_
#include "defines.h"

void free_data(item_t *items, int n);
void sort_by_density(item_t *items, int len);
int filter_items(item_t *items, int len, char *substr);
int starts_with(char *str, char *substr);

#endif