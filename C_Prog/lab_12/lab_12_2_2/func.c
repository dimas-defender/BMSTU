#include "func.h"

DLL void __cdecl fill_fibon(int *a, int n)
{
	if (!a || n < 1)
		return;
	a[0] = 0;
	if (n > 1)
		a[1] = 1;
	for (int i = 2; i < n; i++)
		a[i] = a[i - 2] + a[i - 1];	
}

int num_is_in_arr(int num, int *arr, int n)
{
	int res = 0;
	for (int i = 0; i < n; i++)
		if (num == arr[i])
		{
			res = 1;
			break;
		}
	return res;
}

DLL int __cdecl get_unique_elems(int *src, int *dst, int src_len, int *dst_len)
{
	int rc = SUCCESS;
	int cnt = 0;
	for (int i = 0; i < src_len; i++)
	{
		if (!num_is_in_arr(src[i], src, i))
		{
			if (cnt < *dst_len)
				dst[cnt] = src[i];
			cnt++;
		}
	}
	if (cnt > *dst_len)
		rc = NOT_ENOUGH_SPACE;
	*dst_len = cnt;
	return rc;
}