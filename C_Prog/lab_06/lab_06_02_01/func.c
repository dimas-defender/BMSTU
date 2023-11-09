#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int read_file(char *str, item_t *items, int *quantity)
{
	int ret_key = SUCCESS;
	FILE *f = fopen(str, "r");
	if (f == NULL)
		ret_key = NO_FILE;
	else
	{
		int len = 0;
		while (!ret_key && !feof(f))
		{
			ret_key = input_item(f, items + len);
			if (!ret_key)
				len++;
		}
		if (f != NULL)
			fclose(f);
		if (!len || len > MAX_AMOUNT)
			ret_key = WRONG_FILE;
		else
			*quantity = len;
	}
	return ret_key;
}

int input_item(FILE *f, item_t *items)
{
	int ret_key = SUCCESS;
	if (fscanf(f, "%25[^\n]\n%f\n%f\n", items->name, &(items->weight), &(items->size)) != FIELDS_PER_RECORD)
		ret_key = READ_ERR;
	if (!ret_key && (!strlen(items->name) || strlen(items->name) > MAX_NAME_LEN || items->weight <= 0 || items->size <= 0))
		ret_key = DATA_ERR;
	return ret_key;
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

int filter_items(item_t *items, int len, char *substr)
{
	int quantity = 0;
	for (int i = 0; i < len; i++)
		if (starts_with(items[i].name, substr))
			swap(items + i, items + quantity++);
	return quantity;
}

void print_items(item_t *items, int len)
{
	for (int i = 0; i < len; i++)
		printf("%s\n%f\n%f\n", items[i].name, items[i].weight, items[i].size);
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