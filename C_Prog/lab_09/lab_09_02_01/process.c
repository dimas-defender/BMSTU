#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "defines.h"
#include "process.h"

void free_data(item_t *items, int n)
{
	for (int i = 0; i < n; i++)
		free(items[i].name);
	free(items);
}

void swap(item_t *a, item_t *b)
{
	item_t temp = *a;
	*a = *b;
	*b = temp;
}

void sort_by_density(item_t *items, int len)
{
	for (int i = 0; i < len - 1; i++)
		for (int j = 0; j < len - i - 1; j++)
			if (items[j].weight / items[j].size > items[j + 1].weight / items[j + 1].size)
				swap(items + j, items + j + 1);
}

int starts_with(char *str, char *substr)
{
	int ret_key = 0;
	size_t str_len = strlen(str);
	size_t substr_len = strlen(substr);
	if (substr_len <= str_len)
		ret_key = !strncmp(str, substr, substr_len);
	return ret_key;	
}

int filter_items(item_t *items, int len, char *substr)
{
	int quantity = 0;
	for (int i = 0; i < len; i++)
		if (starts_with(items[i].name, substr))
			swap(items + i, items + quantity++);
	return quantity;
}