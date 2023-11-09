#include <stdio.h>
#define N 10

int array_in(int *a, int n);

void array_out(int *a, int n);

int del_square_numbers(int *a, int n);

int number_is_square(int x);

int main(void)
{
	int n, rc, ret_key = 0;
	printf("Input amount of numbers: ");
	rc = scanf("%d", &n);
	if ((rc == 0) || (rc == -1) || (n < 1) || (n > 10))
	{
		printf("Input error");
		ret_key = 1;
	}
	else
	{
		int a[N];
		if (array_in(a, n) == 1)
		{
			printf("Input error");
			ret_key = 1;
		}
		else
		{
			int q = del_square_numbers(a, n);
			if (q == n)
			{
				printf("All numbers deleted");
				ret_key = 1;
			}
			else
			{
				printf("Array without square numbers:\n");
				array_out(a, n - q);
			}
		}
	}
	return ret_key;
}

int array_in(int *a, int n)
{
	int flag = 0, rc;
	for (int i = 0; i < n; i++)
	{
		rc = scanf("%d", a + i);
		if ((rc == 0) || (rc == -1))
			flag = 1;
	}
	return flag;
}

void array_out(int *a, int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", a[i]);
}

int number_is_square(int x)
{
	int ret_val = 0;
	if (x == 1)
		ret_val = 1;
	for (int i = 0; i <= x / 2; i++)
	{
		if (i * i == x)
		{
			ret_val = 1;
			break;
		}
	}
	return ret_val;	
}

int del_square_numbers(int *a, int n)
{
	int q = 0, k = 0, i, j;
	for (i = 0; i < n; i++)
	{
		if (number_is_square(a[k]))
		{
			for (j = k; j < n - 1; j++)
				a[j] = a[j + 1];
			k--;
			q++;
		}
		k++;
	}
	return q;
}