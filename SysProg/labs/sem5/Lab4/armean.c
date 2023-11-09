//Вычисление среднего арифметического отрицательных чисел в массиве
#include <stdio.h>
#define N 10

int array_in(int *a, int n);
float arithmetic_mean(int *a, int n);

int main(void)
{
	int n, ret_key = 0;
	printf("Input amount of numbers: ");
	if ((scanf("%d", &n) == 0) || (n < 1) || (n > 10))
	{
		printf("Input error\n");
		ret_key = 1;
	}
	else
	{
		int a[N];
		if (n != array_in(a, n))
		{
			printf("Input error\n");
			ret_key = 1;
		}
		else
		{
			if (arithmetic_mean(a, n) == 0)
			{
				printf("No negative numbers\n");
				ret_key = 1;
			}
			else
				printf("Average of negative numbers is %f\n", arithmetic_mean(a, n));
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

float arithmetic_mean(int *a, int n)
{
	int sum = 0, quant = 0, i;
	float mean;
	for (i = 0; i < n; i++)
		if (a[i] < 0)
		{
			sum += a[i];
			quant++;
		}
	if (quant == 0)
		mean = 0;
	else
		mean = (float) sum / quant;
				
	return mean;
}
