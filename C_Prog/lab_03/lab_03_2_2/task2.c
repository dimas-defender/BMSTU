#include <stdio.h>
#define N 10

void transform(int **a, int *matrix, int n, int m);
int input(int **a, int n, int m);
void output(int **a, int n, int m);
void add_rows(int **a, int *n, int m);
void copy_row(int *a, int *b, int m);
int sum_of_digits(int x);

int main(void)
{
	int n, m, ret_key = 0;
	printf("Input amount of rows and columns: ");
	if ((scanf("%d%d", &n, &m) != 2) || (n < 1) || (n > 10) || (m < 1) || (m > 10))
	{
		printf("Input error");
		ret_key = 1;
	}
	else
	{
		int matrix[2 * N][N];
		int *a[2 * N];
		transform(a, *matrix, 2 * N, N);
		if (n * m != input(a, n, m))
		{
			printf("Input error");
			ret_key = 1;
		}
		else
		{
			add_rows(a, &n, m);
			output(a, n, m);
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

void output(int **a, int n, int m)
{
	printf("Matrix:\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
}

int sum_of_digits(int x)
{
	int s = 0;
	if (x < 0)
		x *= -1;
	while (x > 0)
	{
		s += x % 10;
		x /= 10;
	}
	return s;
}

void copy_row(int *a, int *b, int m)
{
	for (int j = 0; j < m; j++)
		a[j] = b[j];
}

void add_rows(int **a, int *n, int m)
{
	for (int i = 0; i < *n; i++)
	{
		int counter = 0;
		for (int j = 0; j < m; j++)
			if (sum_of_digits(a[i][j]) % 2 == 1)
				counter++;
		if (counter > 1)
		{
			for (int k = *n; k > i; k--)
				copy_row(a[k], a[k - 1], m);
			for (int l = 0; l < m; l++)
				a[i][l] = -1;
			*n += 1;
			i++;
		}
	}
}