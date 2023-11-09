#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "output.h"

void print_matrix(int *matrix, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			int k = i * m + j;
			printf("%d ", matrix[k]);
		}
		printf("\n");
	}
}