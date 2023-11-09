/*
Целочисленный тип - int
Алгоритм сортировки - пузырьком
«Направление» упорядочивания - по возрастанию
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "func.h"

int main(int argc, char **argv)
{
	int ret_key = 0;
	FILE *f;
	if (argc != 3 && argc != 4)
	{
		fprintf(stderr, "Error\n");
		ret_key = WRONG_LAUNCH;
	}
	else
	{
		if (!strcmp(argv[1], "c"))
		{
			int amount; 
			sscanf(argv[2], "%d", &amount);
			f = fopen(argv[3], "wb");
			ret_key = create_bin_file(f, amount);			
		}
		if (!strcmp(argv[1], "p"))
		{
			f = fopen(argv[2], "rb");
			if (f == NULL)
			{
				fprintf(stderr, "I/O error\n");
				ret_key = NO_FILE;
			}
			else
				ret_key = print_bin_file(f);
		}
		if (!strcmp(argv[1], "s"))
		{
			f = fopen(argv[2], "rb+");
			if (f == NULL)
			{
				fprintf(stderr, "I/O error\n");
				ret_key = NO_FILE;
			}
			else
				ret_key = sort_bin_file(f);
		}
		fclose(f);
	}
	return ret_key;
}