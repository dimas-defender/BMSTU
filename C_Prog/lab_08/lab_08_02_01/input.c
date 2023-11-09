#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "input.h"

int read_matrix(int *n, int *m, int **matrix)
{
	int ret_key = SUCCESS;
	if (scanf("%d%d", n, m) != 2 || *n <= 0 || *m <= 0)
		ret_key = INPUT_ERR;
	else
	{
		*matrix = malloc(*n * *m * sizeof(int));
		if (!*matrix)
			ret_key = MEMORY_ERR;
		else
		{
			for (int i = 0; i < *n * *m; i++)
				if (scanf("%d", *matrix + i) != 1)
				{
					ret_key = INPUT_ERR;
					break;
				}
		}
	}
	return ret_key;
}