#include <stdio.h>
#define N 10000

int array_in(int *a, int n);

void array_out(int *a, int n);

void bubble_sort(int *a, int n);

void swap(int *a, int *b);

int main(void)
{
	int n, ret_key = 0;
	printf("Input amount of numbers: ");
	if ((scanf("%d", &n) == 0) || (n < 1) || (n > 10000))
	{
		printf("Input error");
		ret_key = 1;
	}
	else
	{
		int a[N];
		if (n != array_in(a, n))
		{
			printf("Input error");
			ret_key = 1;
		}
		else
		{
			bubble_sort(a, n);
			printf("Sorted array:\n");
			array_out(a, n);
		}
	}
	return ret_key;
}

int array_in(int *a, int n)
{
	int rc = 0;
	for (int i = 0; i < n; i++)
		rc += scanf("%d", a + i);
	return rc;
}

void array_out(int *a, int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", a[i]);
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void bubble_sort(int *a, int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (a[j] > a[j + 1])
				swap(a + j, a + j + 1);
		}
	}
}