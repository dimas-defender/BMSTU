#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "process.h"

int key(const int *pb_src, const int *pe_src, int **pb_dst, int **pe_dst)
{
	int ret_key = SUCCESS;
	if (pb_src == NULL || pe_src == NULL || pb_dst == NULL || pe_dst == NULL || pb_src >= pe_src)
		ret_key = POINTER_ERR;
	else
	{
		int len = pe_src - pb_src;
		for (const int *pcur = pb_src; pcur < pe_src; pcur++) 
			if (*pcur < 0)
			{
				len = pcur - pb_src;
				break;
			}
		if (!len)
			ret_key = NEW_ARR_IS_EMPTY;
		else
		{
			*pb_dst = malloc(len * sizeof(int));
			if (!*pb_dst)
				ret_key = MEMORY_ERR;
			else
			{
				*pe_dst = *pb_dst + len;
				const int *pcur1;
				int *pcur2;
				for (pcur1 = pb_src, pcur2 = *pb_dst; pcur1 < pb_src + len; pcur1++, pcur2++)
					*pcur2 = *pcur1;
			}
		}
	}
	return ret_key;
}

void mysort(void *base, size_t num, size_t size, int (*compare)(const void *, const void *))
{
	int left, right, mid;
	for (int i = 1; (size_t)i < num; i++)
	{
		char new_elem[size];
		memcpy(new_elem, (char*)base + i * size, size);
		left = 0;
		right = i - 1;
		while (left <= right)
		{
			mid = left + (right - left) / 2;
			if (compare((char*)base + mid * size, new_elem) > 0)
				right = mid - 1;
			else
				left = mid + 1;
		}
		memmove((char*)base + (left + 1) * size, (char*)base + left * size, (i - left) * size);
		memcpy((char*)base + left * size, new_elem, size);
	}
}

int compare_int(const void *arg1, const void *arg2)
{
	return *(int*)arg1 - *(int*)arg2;
}

void free_data(int *data)
{
	if (data)
		free(data);
}