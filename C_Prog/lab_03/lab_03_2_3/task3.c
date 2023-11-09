#include <stdio.h>
#define N 10

void transform(int **a, int *matrix, int n, int m);
int input(int **a, int n, int m);
void output(int **a, int n, int m);
void swap(int *a, int *b);
void array_product_of_numbers(int **a, int *b, int n, int m);
void sort_matrix(int **a, int *b, int n, int m);

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
			int b[N];
			array_product_of_numbers(a, b, n, m);
			sort_matrix(a, b, n, m);
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

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void array_product_of_numbers(int **a, int *b, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		int p = 1;
		for (int j = 0; j < m; j++)
			p *= a[i][j];
		b[i] = p;
	}
}

void sort_matrix(int **a, int *b, int n, int m)
{
	for (int i = 0; i < n - 1; i++)
		for (int j = 1; j < n - i; j++)
			if (b[j] < b[j - 1])
			{
				swap(b + j, b + j - 1);
				for (int k = 0; k < m; k++)
					swap(*(a + j) + k, *(a + j - 1) + k);
			}
}