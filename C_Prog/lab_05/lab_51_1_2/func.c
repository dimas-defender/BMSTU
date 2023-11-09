#include <stdio.h>
#include "func.h"

int get_avg(FILE *f, float *avg)
{
	int ret_key = 0;
	float num;
	if (fscanf(f, "%f", &num) == 1)
	{
		float max = num, min = num;
		while (fscanf(f, "%f", &num) == 1)
		{
			if (num > max)
				max = num;
			if (num < min)
				min = num;
		}
		*avg = (max + min) / 2;
	}
	else
		ret_key = NO_DATA;
	return ret_key;
}

int count_numbers(FILE *f, float avg)
{
	int cnt = 0;
	float num;
	while (fscanf(f, "%f", &num) == 1)
		if (num > avg)
			cnt++;
	return cnt;
}