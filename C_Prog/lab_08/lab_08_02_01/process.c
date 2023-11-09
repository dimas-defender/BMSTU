#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defines.h"
#include "process.h"

int find_max_elem_row_or_col(int *matrix, int n, int m, int key)
{
	int row = 0, col = 0;
	int max = matrix[0];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
		{
			int k = i * m + j;
			if (matrix[k] >= max)
			{
				max = matrix[k];
				row = i;
				col = j;
			}
		}
	return key ? row : col;
}

void delete_matrix_row(int *matrix, int n, int m)
{
	int row = find_max_elem_row_or_col(matrix, n, m, ROW);
	for (int k = m * (row + 1); k < n * m; k++)
		matrix[k - m] = matrix[k];
}

void delete_matrix_col(int *matrix, int n, int m)
{
	int col = find_max_elem_row_or_col(matrix, n, m, COL);
	for (int i = 0; i < n; i++)
	{
		int q = (i + 1) * m + col;
		if (i == n - 1)
			q = q - col;
		for (int k = i * m + col + 1; k < q; k++)
			matrix[k - i - 1] = matrix[k];		
	}
}

void make_matrix_square(int *matrix, int n, int m)
{
	if (n > m)
	{
		int q = n - m;
		for (int i = 0; i < q; i++)
		{
			delete_matrix_row(matrix, n, m);
			n--;
		}
	}
	else if (n < m)
	{
		int q = m - n;
		for (int i = 0; i < q; i++)
		{
			delete_matrix_col(matrix, n, m);
			m--;
		}
	}
}

void add_matrix_row(int *matrix, int n, int m)
{
	for (int j = 0; j < m; j++)
	{
		int sum = 0;
		for (int i = 0; i < n; i++)
			sum += matrix[i * m + j];
		matrix[n * m + j] = floor((double) sum / n);
	}
}

void add_matrix_col(int *matrix, int n, int m)
{
	m++;
	for (int i = 0; i < n; i++)
	{
		for (int k = n * m - 1; k > (i + 1) * m - 1; k--)
			matrix[k] = matrix[k - 1];
		int min_in_row = matrix[i * m];
		for (int j = 1; j < m - 1; j++)
			if (matrix[i * m + j] < min_in_row)
				min_in_row = matrix[i * m + j];
		matrix[(i + 1) * m - 1] = min_in_row;
	}
}

int expand_square_matrix(int **matrix, int old_size_rows, int old_size_cols, int new_size)
{
	int ret_key = SUCCESS;
	if (new_size * new_size > old_size_rows * old_size_cols)
	{
		int *ptmp = realloc(*matrix, new_size * new_size * sizeof(int));
		if (ptmp)
			*matrix = ptmp;
		else
			ret_key = MEMORY_ERR;
	}
	if (!ret_key)
	{
		int old_size = old_size_rows < old_size_cols ? old_size_rows : old_size_cols;
		int n = old_size, m = old_size;
		for (int i = 0; i < new_size - old_size; i++)
			add_matrix_row(*matrix, n++, m);
		for (int i = 0; i < new_size - old_size; i++)
			add_matrix_col(*matrix, n, m++);
	}
	return ret_key;
}

void multiply_square_matrix(int *a, int *b, int *res, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			int elem = 0;
			for (int k = 0; k < size; k++)
				elem += a[i * size + k] * b[k * size + j];
			res[i * size + j] = elem;
		}			
}

void copy_square_matrix(int *src, int *dst, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			dst[i * size + j] = src[i * size + j];
}

void make_square_identity_matrix(int *matrix, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (i == j)
				matrix[i * size + j] = 1;
			else
				matrix[i * size + j] = 0;
}

int calculate_matrix_expression(int *a, int *b, int **res, int powa, int powb, int size)
{
	int ret_key = SUCCESS;
	*res = malloc(size * size * sizeof(int));
	if (!*res)
		ret_key = MEMORY_ERR;
	else
	{
		if (powa)
			copy_square_matrix(a, *res, size);
		else
			make_square_identity_matrix(*res, size);
		int *extra_matrix = malloc(size * size * sizeof(int));
		if (!extra_matrix)
			ret_key = MEMORY_ERR;
		else
		{
			for (int i = 1; i < powa; i++)
			{
				multiply_square_matrix(*res, a, extra_matrix, size);
				copy_square_matrix(extra_matrix, *res, size);
			}
			for (int i = 0; i < powb; i++)
			{
				multiply_square_matrix(*res, b, extra_matrix, size);
				copy_square_matrix(extra_matrix, *res, size);
			}
			free(extra_matrix);
		}
	}
	return ret_key;
}

void free_data(int *data)
{
	if (data)
		free(data);
}