//Факториал числа
#include <stdio.h>

int fact(int n);

int main(void)
{
	int n, rc, ret_key;

	printf("Input n: ");
	rc = scanf("%d", &n);
	if ((rc == 1) && (n > 0))
	{
		printf("n! = %d\n", fact(n));
		ret_key = 0;
	}
	else
	{
		printf("Input error");
		ret_key = 1;
	}
	return ret_key;
}

int fact(int n)
{
	if (n == 1)
		return 1;
	else
		return n * fact(n - 1);
}
