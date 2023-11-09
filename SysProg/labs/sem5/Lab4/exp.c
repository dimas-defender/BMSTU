//Вычисление exp(x) с заданной точностью с помощью разложения в ряд Тейлора
#include <stdio.h>
#include <math.h>

float func(float x, float eps);
int output(float x, float eps, double f, double abs_p, double otn_p);

int main(void)
{
	float x, eps;
	int rc, ret_key;
	
	printf("Input x, eps: ");
	rc = scanf("%f %f", &x, &eps);
	if ((rc == 2) && (eps >= 0) && (eps <= 1))
	{
		double f = exp(x);
		double abs_p = fabs(f - func(x, eps));
		double otn_p = abs_p / f;
		output(x, eps, f, abs_p, otn_p);
		ret_key = 0;
	}		
	else
	{
		printf("Input error\n");
		ret_key = 1;
	}
	return ret_key;
}

int output(float x, float eps, double f, double abs_p, double otn_p)
{
	printf("s(x) = %.6f", func(x, eps));
	printf("\nf(x) = %.6f", f);
	printf("\nAbsolute error = %.6f", abs_p);
	printf("\nRelative error = %.6f\n", otn_p);
	
	return 0;
}

float func(float x, float eps)
{
	int n = 0;
	float s = 1, elem = 1;
	
	while (fabs(elem) > eps)
	{
		n++;
		elem *= x / n;
		s += elem;
	}
	return s;
}
