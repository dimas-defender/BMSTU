#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int main(int argc, char **argv)
{
	int ret_key = SUCCESS;
	if (argc != 2 && argc != 3)
		ret_key = WRONG_LAUNCH;
	else
	{
		item_t items[MAX_AMOUNT];
		int quantity;
		ret_key = read_file(argv[1], items, &quantity);
		if (!ret_key)
		{
			if (argc == 2)
				sort_by_density(items, quantity);
			else if (strlen(argv[2]) <= MAX_NAME_LEN && strcmp(argv[2], "ALL"))
				quantity = filter_items(items, quantity, argv[2]);
			else if (strlen(argv[2]) > MAX_NAME_LEN)
				ret_key = WRONG_SUBSTR_LEN;
			if (!ret_key)
				print_items(items, quantity);
		}
	}
	return ret_key;
}