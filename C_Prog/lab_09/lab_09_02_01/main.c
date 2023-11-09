#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "defines.h"
#include "input.h"
#include "process.h"
#include "output.h"

int main(int argc, char **argv)
{
	int ret_key = SUCCESS;
	if (argc != 2 && argc != 3)
		ret_key = WRONG_LAUNCH;
	else
	{
		item_t *items;
		int quantity;
		ret_key = read_file(argv[1], &items, &quantity);
		if (!ret_key)
		{
			int len = quantity;
			if (argc == 2)
				sort_by_density(items, quantity);
			else if (strcmp(argv[2], "ALL"))
				quantity = filter_items(items, quantity, argv[2]);
			print_items(items, quantity);
			free_data(items, len);
		}
	}
	return ret_key;
}