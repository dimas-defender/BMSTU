#include <stdio.h>
#define N 10

int array_in(int *a, int n);

void array_out(int *a, int n);

int fill_new_arr(int *a, int *b, int n);

int same_first_last_digit(int x);

int main(void)
{
	int n, ret_key = 0;
	printf("Input amount of numbers: ");
	if ((scanf("%d", &n) == 0) || (n < 1) || (n > 10))
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
			int b[N];
			if (fill_new_arr(a, b, n) == 0)
			{
				printf("No matching numbers");
				ret_key = 1;
			}
			else
			{
				printf("New array:\n");
				array_out(b, fill_new_arr(a, b, n));
			}
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

int same_first_last_digit(int x)
{
	int q = x, ret_val = 0;
	while ((q > 9) || (q < -9))
		q /= 10;
	if (q == x % 10)
		ret_val = 1;
	return ret_val;	
}

int fill_new_arr(int *a, int *b, int n)
{
	int q = 0, i;
	for (i = 0; i < n; i++)
	{
		if (same_first_last_digit(a[i]))
		{
			b[q] = a[i];
			q++;
		}
	}
	return q;
}