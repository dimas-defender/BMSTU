#include <stdio.h>
#define N 10

int array_in(int *pb, int *pe);

int process(int *pb, int *pe);

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
		if (n != array_in(a, a + n))
		{
			printf("Input error");
			ret_key = 1;
		}
		else
		{
			int q = process(a, a + n);
			if (q == 0)
			{
				printf("Value can't be found");
				ret_key = 1;				
			}
			else
				printf("Value is %d", q);
		}
	}
	return ret_key;
}

int array_in(int *pb, int *pe)
{
	int rc = 0;
	for (int *pcur = pb; pcur < pe; pcur++)
		rc += scanf("%d", pcur);
	return rc;
}

int process(int *pb, int *pe)
{
	int sum = 0;
	for (int *pcur1 = pb; pcur1 < pe; pcur1++)
		if (*pcur1 < 0)
			for (int *pcur2 = pe - 1; pcur2 > pb; pcur2--)
				if (*pcur2 > 0)
				{
					sum += *pcur1 * *pcur2;
					*pcur1 = 0;
					*pcur2 = 0;
					break;
				}
	return sum;
}