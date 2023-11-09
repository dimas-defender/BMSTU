#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "defines.h"
#include "output.h"

void print_items(item_t *items, int len)
{
	for (int i = 0; i < len; i++)
		printf("%s\n%f\n%f\n", items[i].name, items[i].weight, items[i].size);
}