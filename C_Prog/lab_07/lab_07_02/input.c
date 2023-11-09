#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "input.h"

void fill_array(FILE *f, int *pbeg, int *pend)
{
	for (int *pcur = pbeg; pcur < pend; pcur++)
		fscanf(f, "%d", pcur);
}

int count_numbers(FILE *f, int *len)
{
	int ret_key = SUCCESS;
	int num, cnt = 0;
	while (!ret_key && !feof(f))
	{
		if (fscanf(f, "%d", &num) == 1)
			cnt++;
		else
			ret_key = DATA_ERR;
	}
	if (!cnt)
		ret_key = DATA_ERR;
	if (!ret_key)
		*len = cnt;
	return ret_key;
}

int read_file(char *str, int **pbeg, int **pend)
{
	int ret_key = SUCCESS;
	FILE *f = fopen(str, "r");
	if (f == NULL)
		ret_key = NO_FILE;
	else
	{
		int len;
		ret_key = count_numbers(f, &len);
		if (!ret_key)
		{
			*pbeg = malloc(len * sizeof(int));
			if (!*pbeg)
				ret_key = MEMORY_ERR;
			else
			{
				*pend = *pbeg + len;
				rewind(f);
				fill_array(f, *pbeg, *pend);
			}
		}
		fclose(f);
	}
	return ret_key;
}