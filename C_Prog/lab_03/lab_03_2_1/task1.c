#include <stdio.h>
#define N 10

void transform(int **a, int *matrix, int n, int m);
int input(int **a, int n, int m);
void array_out(int *a, int n);
void make_array(int **a, int *b, int n, int m);

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
			int b[N] = { 0 };
			make_array(a, b, n, m);
			array_out(b, m);	
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

void make_array(int **a, int *b, int n, int m)
{
	for (int j = 0; j < m; j++)
	{
		int previous_elem = a[0][j];
		int flag = 1;
		
		for (int i = 1; i < n; i++)
		{
			if ((a[i][j] * previous_elem > 0) || (a[i][j] * previous_elem == 0 && a[i][j] + previous_elem > 0))
				flag = 0;
			else
				previous_elem = a[i][j];
		}
		if (flag && n > 1)
			b[j] = 1;
	}
}

void array_out(int *a, int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", a[i]);
}