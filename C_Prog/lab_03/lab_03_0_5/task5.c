#include <stdio.h>
#define N 10

void transform(int **a, int *matrix, int n, int m);
int input(int **a, int n, int m);
void output(int **a, int n, int m);
void swap(int *a, int *b);
int number_is_prime(int x);
int make_array(int **a, int *b, int n, int m);
void reverse_array(int *b, int n);
void insert_numbers(int **a, int *b, int n, int m);

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
			int k = make_array(a, b, n, m);
			if (k == 0)
			{
				printf("No prime numbers");
				ret_key = 1;				
			}
			else
			{
				reverse_array(b, k);
				insert_numbers(a, b, n, m);
				output(a, n, m);
			}
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

int number_is_prime(int x)
{
	int ret_val = 1;
	if (x < 2)
		ret_val = 0;
	for (int i = 2; i <= x / 2; i++)
		if (x % i == 0)
		{
			ret_val = 0;
			break;
		}
	return ret_val;	
}

int make_array(int **a, int *b, int n, int m)
{
	int k = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (number_is_prime(a[i][j]))
			{
				b[k] = a[i][j];
				k++;
			}
	return k;
}

void reverse_array(int *b, int n)
{
	for (int i = 0; i < n / 2; i++)
		swap(b + i, b + n - i - 1);	
}

void insert_numbers(int **a, int *b, int n, int m)
{
	int k = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (number_is_prime(a[i][j]))
			{
				a[i][j] = b[k];
				k++;
			}
}