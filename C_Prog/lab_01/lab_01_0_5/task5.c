#include <stdio.h>

int power(int a, int n);

int main(void)
{
	int a, n, rc, ret_key;
	
	printf("Input a, n: ");
	rc = scanf("%d %d", &a, &n);
	if ((rc == 2) && (n > 0))
	{
		printf("a^n = %d", power(a, n));
		ret_key = 0;
	}		
	else
	{
		printf("Input error");
		ret_key = 1;
	}
	return ret_key;
}

int power(int a, int n)
{
	int value;
	if (n == 1)
		value = a;
	else
		value = a * power(a, n - 1);
		
	return value;
}