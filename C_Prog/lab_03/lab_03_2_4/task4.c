#include <stdio.h>
#include <stdlib.h>
#define N 10

void transform(int **a, int *matrix, int n, int m);
int input(int **a, int n, int m);
int find_number(int **a, int n);

int main(void)
{
	int n, m, ret_key = 0;
	printf("Input amount of rows and columns: ");
	if ((scanf("%d%d", &n, &m) != 2) || (n < 1) || (n > 10) || (n != m))
	{
		printf("Input error");
		ret_key = 1;
	}
	else
	{
		int matrix[N][N];
		int *a[N];
		transform(a, *matrix, N, N);
		if (n * m != input(a, n, m))
		{
			printf("Input error");
			ret_key = 1;
		}
		else
		{
			int num = find_number(a, n);
			if (num == 0)
			{
				printf("No matching numbers");
				ret_key = 1;
			}
			else
				printf("%d", num);
		}
	}
	return ret_key;
}

void transform(int **a, int *matrix, int n, int m)
{
	for (int i = 0; i < n; i++)
		a[i] = matrix + m * i;
}

int input(int **a, int n, int m)
{
	int rc = 0;
	printf("Input matrix values:\n");
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			rc += scanf("%d", *(a + i) + j);
	return rc;
}

int find_number(int **a, int n)
{
	int max = 0;
	for (int i = 0; i < n; i++)
		for (int j = n - i; j < n; j++)
			if (abs(a[i][j]) % 10 == 5)
				if (max == 0 || a[i][j] > max)
					max = a[i][j];
	return max;
}