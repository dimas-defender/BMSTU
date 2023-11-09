#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "input.h"
#include "output.h"
#include "process.h"

int main(void)
{
	setbuf(stdout, NULL);
	int *a = NULL, *b = NULL;
	int n, m, p, q;
	int ret_key = read_matrix(&n, &m, &a);
	if (!ret_key)
		ret_key = read_matrix(&p, &q, &b);
	if (!ret_key)
	{
		int k = n < m ? n : m;
		int s = p < q ? p : q;
		make_matrix_square(a, n, m);
		make_matrix_square(b, p, q);
		int z = s;
		if (k > s)
		{
			z = k;
			ret_key = expand_square_matrix(&b, p, q, k);
		}
		else if (s > k)
			ret_key = expand_square_matrix(&a, n, m, s);
		if (!ret_key)
		{
			int r, y;
			if (scanf("%d%d", &r, &y) != 2 || r < 0 || y < 0)
				ret_key = INPUT_ERR;
			else
			{
				int *res = NULL;
				ret_key = calculate_matrix_expression(a, b, &res, r, y, z);
				if (!ret_key)
					print_matrix(res, z, z);
				free_data(res);
			}
		}
	}
	free_data(a);
	free_data(b);
	return ret_key;
}