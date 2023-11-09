#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "func.h"

int file_size(FILE *f, size_t *size)
{
	int ret_key = 0;
	long sz;
	if (fseek(f, 0L, SEEK_END))
		ret_key = SEEK_ERR;
	else
	{
		sz = ftell(f);
		if (sz < 0)
			ret_key = SEEK_ERR;
		else
			*size = sz;
	}
	return ret_key || fseek(f, 0L, SEEK_SET);
}

int print_bin_file(FILE *f)
{
	size_t size;
	int num;
	int rc = file_size(f, &size) || size <= 0 || size % sizeof(int);
	if (rc == 0)
	{
		for (size_t i = 0; !rc && i < size / sizeof(int); i++)
		{
			if (fread(&num, sizeof(int), 1, f) == 1)
				printf("%d ", num);
			else
				rc = READ_ERR;
		}
	}
	return rc;
}

int create_bin_file(FILE *f, int amount)
{
	int rc = 0;
	srand(time(0));
	for (int i = 0; !rc && i < amount; i++)
	{
		int num = rand() % (MAX_RAND - MIN_RAND + 1) + MIN_RAND;
		if (fwrite(&num, sizeof(int), 1, f) != 1)
			rc = WRITE_ERR;
	}
	return rc;
}

int get_number_by_pos(FILE *f, size_t pos, int *num)
{
	int ret_key = 0;
	if (!fseek(f, pos * sizeof(int), SEEK_SET))
	{
		if (fread(num, sizeof(int), 1, f) != 1)
			ret_key = READ_ERR;
	}
	else
		ret_key = SEEK_ERR;
	return ret_key;
}

int put_number_by_pos(FILE *f, size_t pos, int *num)
{
	int ret_key = 0;
	if (!fseek(f, pos * sizeof(int), SEEK_SET))
	{
		if (fwrite(num, sizeof(int), 1, f) != 1)
			ret_key = WRITE_ERR;
	}
	else
		ret_key = SEEK_ERR;
	return ret_key;
}

int sort_bin_file(FILE *f)
{
	size_t i, j;
	size_t size;
	int rc = file_size(f, &size) || size <= 0 || size % sizeof(int);
	if (rc == 0)
	{
		size /= sizeof(int);
		int num1, num2;
		for (i = 0; !rc && i < size - 1; i++)
			for (j = 0; !rc && j < size - i - 1; j++)
			{
				rc = get_number_by_pos(f, j, &num1) || get_number_by_pos(f, j + 1, &num2);
				if (!rc && num1 > num2)
					rc = put_number_by_pos(f, j + 1, &num1) || put_number_by_pos(f, j, &num2);
			}
	}
	return rc;
}