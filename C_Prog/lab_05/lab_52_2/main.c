#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "func.h"

int main(int argc, char **argv)
{
	int ret_key = 0;
	FILE *fin;
	FILE *fout;
	if (argc != 3 && argc != 4)
	{
		fprintf(stderr, "Error\n");
		ret_key = WRONG_LAUNCH;
	}
	else
	{
		if (!strcmp(argv[1], "sb") && argc == 4)
		{
			fin = fopen(argv[2], "rb");
			if (fin == NULL)
			{
				fprintf(stderr, "I/O error\n");
				ret_key = NO_FILE;
			}
			else
			{
				size_t size;
				ret_key = file_size(fin, &size) || size <= 0 || size % sizeof(product_t);
				if (!ret_key)
				{
					fout = fopen(argv[3], "wb+");
					ret_key = copy_bin_file(fin, fout);
					if (!ret_key)
						ret_key = sort_bin_file(fout);
					fclose(fout);
				}
			}
			fclose(fin);
		}
		else if (!strcmp(argv[1], "fb") && argc == 4)
		{
			fin = fopen(argv[2], "rb");
			if (fin == NULL)
			{
				fprintf(stderr, "I/O error\n");
				ret_key = NO_FILE;
			}
			else
			{
				char substr[MAX_NAME_LEN + 1] = { 0 };
				sscanf(argv[3], "%s", substr);
				ret_key = print_products(fin, substr);
			}
			fclose(fin);
		}
		else if (!strcmp(argv[1], "ab") && argc == 3)
		{
			fin = fopen(argv[2], "rb+");
			if (fin == NULL)
			{
				fprintf(stderr, "I/O error\n");
				ret_key = NO_FILE;
			}
			else
			{
				size_t size;
				ret_key = file_size(fin, &size) || size <= 0 || size % sizeof(product_t);
				if (!ret_key)
				{
					product_t product = { 0 };
					memset(&product, 0, sizeof(product_t));
					ret_key = input_product(stdin, &product);
					if (!ret_key)
						ret_key = add_product(fin, &product, size);
				}
			}
			if (fin != NULL)
				fclose(fin);
		}
		else
			ret_key = WRONG_LAUNCH;
	}
	return ret_key;
}