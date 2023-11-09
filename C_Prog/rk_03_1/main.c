#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define INPUT_ERR 1
#define MEMORY_ERR 2

void free_data(int **data, int n);
int read_matrix(int *n, int *m, int ***matrix);
int allocate_matrix(int n, int m, int ***data);
void print_matrix(int **matrix, int n, int m);
int create_rotated_matrix(int ***new, int **matrix, int n, int m);
void swap_rows(int **matrix, int i, int j);
int find_num_len(int num);
void find_min_max_num_len_rows(int **matrix, int n, int m, int *min_s, int *max_s);
void remove_row(int **matrix, int n, int index);

int main(void)
{
	int n, m, p, q;
	int **matrix, **new_matrix;
	int rc = read_matrix(&n, &m, &matrix);
	if (!rc)
		rc = create_rotated_matrix(&new_matrix, matrix, n, m);
	if (!rc)
	{
		p = m;
		q = n;
		print_matrix(new_matrix, p, q);
		int min_s, max_s;
		find_min_max_num_len_rows(new_matrix, p, q, &min_s, &max_s);
		if (min_s != max_s)
			swap_rows(new_matrix, min_s, max_s);
		else
			remove_row(new_matrix, p--, min_s);
		print_matrix(new_matrix, p, q);
		free_data(new_matrix, m);
	}
	free_data(matrix, n);
	return rc;
}

void remove_row(int **matrix, int n, int index)
{
	for (int i = index; i < n - 1; i++)
		matrix[i] = matrix[i + 1];
}

void find_min_max_num_len_rows(int **matrix, int n, int m, int *min_s, int *max_s)
{
	int min_i = 0, max_i = 0;
	int min_len = find_num_len(matrix[0][0]);
	int max_len = min_len, len;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			len = find_num_len(matrix[i][j]);
			if (len < min_len)
			{
				min_len = len;
				min_i = i;
			}
			if (len > max_len)
			{
				max_len = len;
				max_i = i;
			}
		}
	}
	*min_s = min_i;
	*max_s = max_i;
}

int find_num_len(int num)
{
	num = abs(num);
	int len = 0;
	if (!num)
		len = 1;
	while (num > 0)
	{
		num /= 10;
		len++;
	}
	return len;
}

void swap_rows(int **data, int i, int j)
{
	int *temp = data[i];
	data[i] = data[j];
	data[j] = temp;	
}

int create_rotated_matrix(int ***new, int **matrix, int n, int m)
{
	int rc = allocate_matrix(m, n, new);
	if (!rc)
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				(*new)[j][n  - i - 1] = matrix[i][j];
	}
	return rc;
}

void print_matrix(int **matrix, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}

void free_data(int **data, int n)
{
	for (int i = 0; i < n; i++)
		free(data[i]);
	free(data);
}

int read_matrix(int *n, int *m, int ***matrix)
{
	int rc = SUCCESS;
	if (scanf("%d%d", n, m) != 2)
		rc = INPUT_ERR;
	else
		rc = allocate_matrix(*n, *m, matrix);
	if (!rc)
	{
		for (int i = 0; i < *n; i++)
			for (int j = 0; j < *m; j++)
				if (scanf("%d", &(*matrix)[i][j]) != 1)
					rc = INPUT_ERR;
	}
	return rc;
}

int allocate_matrix(int n, int m, int ***data)
{
	int rc = SUCCESS;
	*data = calloc(n, sizeof(int *));
	if (!*data)
		rc = MEMORY_ERR;
	else
	{
		for (int i = 0; i < n; i++)
		{
			(*data)[i] = malloc(m * sizeof(int));
			if (!(*data)[i])
			{
				rc = MEMORY_ERR;
				free_data(*data, n);
			}
		}
	}
	return rc;
}