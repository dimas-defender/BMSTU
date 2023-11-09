#include <stdio.h>
#include <inttypes.h>
#include <sys/time.h>
#define N 10000

int array_in(int *pb, int *pe);

void copy_array(int *pb1, int *pe1, int *pb2);

int process_3(int *pb, int *pe);

int process_1(int *a, int n);

int process_2(int *a, int n);

int64_t elapsed_time(struct timeval tv_start, struct timeval tv_stop);

void test_process_1(int *array, int n);

void test_process_2(int *array, int n);

int test_process_3(int *array, int n);

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
		if (n != array_in(a, a + n))
		{
			printf("Input error");
			ret_key = 1;
		}
		else
		{
			int b[N], c[N];
			copy_array(a, a + n, b);
			copy_array(a, a + n, c);
			
			test_process_1(b, n);
			test_process_2(c, n);
			int q = test_process_3(a, n);	
			
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

void copy_array(int *pb1, int *pe1, int *pb2)
{
    for (int *pcur = pb1; pcur < pe1; pcur++)
	{
		*pb2 = *pcur;
		pb2++;
	}
}

int process_3(int *pb, int *pe)
{
	int sum = 0;
	for (int *pcur1 = pb; pcur1 < pe; pcur1++)
		if (*pcur1 < 0)
			for (int *pcur2 = pe - 1; pcur2 >= pb; pcur2--)
				if (*pcur2 > 0)
				{
					sum += *pcur1 * *pcur2;
					*pcur1 = 0;
					*pcur2 = 0;
					break;
				}
	return sum;
}

int process_1(int *a, int n)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
		if (a[i] < 0)
			for (int j = n - 1; j >= 0; j--)
				if (a[j] > 0)
				{
					sum += a[i] * a[j];
					a[i] = 0;
					a[j] = 0;
					break;
				}
	return sum;
}

int process_2(int *a, int n)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
		if (*(a + i) < 0)
			for (int j = n - 1; j >= 0; j--)
				if (*(a + j) > 0)
				{
					sum += *(a + i) * *(a + j);
					*(a + i) = 0;
					*(a + j) = 0;
					break;
				}
	return sum;
}

int64_t elapsed_time(struct timeval tv_start, struct timeval tv_stop)
{
    return 
        (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + 
		(tv_stop.tv_usec - tv_start.tv_usec);
}

void test_process_1(int *array, int n)
{
    struct timeval tv_start, tv_stop;
    gettimeofday(&tv_start, NULL);
    process_1(array, n);
    gettimeofday(&tv_stop, NULL);
    printf("%" PRId64 " µs\n", elapsed_time(tv_start, tv_stop)); 
}

void test_process_2(int *array, int n)
{
    struct timeval tv_start, tv_stop;
    gettimeofday(&tv_start, NULL);
    process_2(array, n);
    gettimeofday(&tv_stop, NULL);
    printf("%" PRId64 " µs\n", elapsed_time(tv_start, tv_stop)); 
}

int test_process_3(int *array, int n)
{
    struct timeval tv_start, tv_stop;
    gettimeofday(&tv_start, NULL);
	int q = process_3(array, array + n);
    gettimeofday(&tv_stop, NULL);
    printf("%" PRId64 " µs\n", elapsed_time(tv_start, tv_stop)); 
	return q;
}