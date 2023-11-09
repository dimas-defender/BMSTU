#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SUCCESS 0
#define INPUT_ERR 1
#define MEMORY_ERR 2
#define FILE_ERR 3

typedef struct
{
	int *MN;
	int *IM;
	int *JM;
} sparse_M;

void free_matrix(int **data, int n);
void free_sparse_matrix(sparse_M sparse_A);
int input_matrix(int ***matrix, int n, int m);
int input_matrix_from_file(int ***matrix, int n, int m);
int generate_matrix(int ***matrix, int n, int m);
int allocate_matrix(int ***data, int n, int m);
int allocate_sparse_matrix(sparse_M *sparse_A, int len, int m);
int menu_select(void);
void print_matrix(int **matrix, int n, int m);
void add_up_matrix(int **A, int **B, int **S, int n, int m);
int create_sparse_matrix(int **A, int n, int m, sparse_M *sparse_A, int *lenA);
int count_nonzero_elems(int **A, int n, int m);
int input_type(void);
void init_sparse_matrix(sparse_M *sparse_A);
void print_array(int *a, int n);
void print_sparse_matrix(sparse_M sparse_A, int len, int m);
void add_sparse_matrix(sparse_M matrix_a, sparse_M matrix_b, sparse_M *res, int m, int lenA, int lenB, int *lenS);
void remove_zeros(sparse_M matrix, int *len);
void fill_col_entry(sparse_M sparse, int **matrix, int n, int m);
unsigned long long tick(void);

int main(void)
{
	setbuf(stdout, NULL);
    char choice;
	int n, m;
	int lenA, lenB, lenS = 0;
	int **A = NULL, **B = NULL, **S = NULL;
	sparse_M sparse_A, sparse_B, sparse_S;
	unsigned long tb, te, t_1, t_2;
	unsigned long size_1, size_2;
	init_sparse_matrix(&sparse_A);
	init_sparse_matrix(&sparse_B);
	init_sparse_matrix(&sparse_S);
	
	printf("This program adds up matrix A and matrix B (gets matrix S as a result), which are input from file or keyboard or generated automatically.\n\n");
	printf("Input numbers of matrix rows and columns (both from 1 to 1000): ");
	if (scanf("%d%d", &n, &m) != 2 || n < 1 || n > 1000 || m < 1 || m > 1000)
	{
		printf("Input error\n");
		return INPUT_ERR;
	}
	
	printf("How do you want to input matrix A?\n");
	choice = input_type();
	switch(choice)
	{
	case 1:
		if (input_matrix_from_file(&A, n, m))
		{
			printf("Input error\n");
			return INPUT_ERR;
		}
		printf("OK\n");
        break;
	case 2:
		if (input_matrix(&A, n, m))
		{
			printf("Input error\n");
			return INPUT_ERR;
		}
		printf("OK\n");
        break;
	case 3:
		if (generate_matrix(&A, n, m))
		{
			printf("Input error\n");
			return INPUT_ERR;
		}
		printf("OK\n");
        break;
	}
	
	if (create_sparse_matrix(A, n, m, &sparse_A, &lenA))
	{
		printf("Memory error\n");
		return INPUT_ERR;
	}
	
	printf("How do you want to input matrix B?\n");
	choice = input_type();
	switch(choice)
	{
	case 1:
		if (input_matrix_from_file(&B, n, m))
		{
			printf("Input error\n");
			return INPUT_ERR;
		}
		printf("OK\n");
        break;
	case 2:
		if (input_matrix(&B, n, m))
		{
			printf("Input error\n");
			return INPUT_ERR;
		}
		printf("OK\n");
        break;
	case 3:
		if (generate_matrix(&B, n, m))
		{
			printf("Input error\n");
			return INPUT_ERR;
		}
		printf("OK\n");
        break;
	}
	
	if (create_sparse_matrix(B, n, m, &sparse_B, &lenB))
	{
		printf("Memory error\n");
		return INPUT_ERR;
	}
	if (allocate_matrix(&S, n, m))
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	if (allocate_sparse_matrix(&sparse_S, lenA + lenB, m))
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	
	tb = tick();
	add_up_matrix(A, B, S, n, m);
	te = tick();
	t_1 = te - tb;
	size_1 = 3 * (n * sizeof(int *) + n * m * sizeof(int));
	printf("\nStandard addition time: %lu\n", t_1);
	printf("Memory size needed for standard form: %lu\n\n", size_1);
	
	tb = tick();
	add_sparse_matrix(sparse_A, sparse_B, &sparse_S, m, lenA, lenB, &lenS);
	te = tick();
	t_2 = te - tb;
	size_2 = 3 * sizeof(sparse_M) + sizeof(int) * (2 * (lenA + lenB + lenS) + 3 * m);
	printf("Sparse addition time: %lu\n", t_2);
	printf("Memory size needed for sparse form: %lu\n\n", size_2);
	
	remove_zeros(sparse_S, &lenS);
	fill_col_entry(sparse_S, S, n, m);
	
    for(;;)
    {
        choice = menu_select();
        switch(choice)
        {
        case 1:
			printf("Matrix A:\n");
			print_matrix(A, n, m);
			printf("\n");
            break;
		case 2:
			printf("Matrix B:\n");
			print_matrix(B, n, m);
			printf("\n");
            break;
		case 3:
			printf("Matrix S:\n");
			print_matrix(S, n, m);
			printf("\n");
			break;
		case 4:
			printf("Matrix A:\n");
			print_sparse_matrix(sparse_A, lenA, m);
			printf("\n");
            break;
		case 5:
			printf("Matrix B:\n");
			print_sparse_matrix(sparse_B, lenB, m);
			printf("\n");
			break;
		case 6:
			printf("Matrix S:\n");
			print_sparse_matrix(sparse_S, lenS, m);
			printf("\n");
			break;
        case 0:
			free_matrix(A, n);
			free_matrix(B, n);
			free_matrix(S, n);
			free_sparse_matrix(sparse_A);
			free_sparse_matrix(sparse_B);
			free_sparse_matrix(sparse_S);
        	return 0;
        }
    }
}

int menu_select(void)
{
    int c;
    printf("1. Print matrix A in standard form\n");
	printf("2. Print matrix B in standard form\n");
	printf("3. Print matrix S in standard form\n");
	printf("4. Print matrix A in sparse form\n");
	printf("5. Print matrix B in sparse form\n");
	printf("6. Print matrix S in sparse form\n");
    printf("\n0. Exit\n");

    do {
        printf("\nYour choice: ");
        if (scanf("%d", &c) != 1)
        {
            printf("\nWrong input\n");
            scanf("%*s");
            c = -1;
        }
    } while (c < 0 || c > 6);

    printf("\n");
    return c;
}

void init_sparse_matrix(sparse_M *sparse_A)
{
	(sparse_A->MN) = NULL;
	(sparse_A->IM) = NULL;
	(sparse_A->JM) = NULL;
}

int input_type(void)
{
    int c;
    printf("1. From file\n");
	printf("2. By keyboard\n");
	printf("3. Automatically\n");

    do {
        printf("\nYour choice: ");
        if (scanf("%d", &c) != 1)
        {
            printf("\nWrong input\n");
            scanf("%*s");
            c = -1;
        }
    } while (c < 1 || c > 3);

    printf("\n");
    return c;
}

int count_nonzero_elems(int **A, int n, int m)
{
	int cnt = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (A[i][j])
				cnt++;
	return cnt;
}

int allocate_sparse_matrix(sparse_M *sparse_A, int len, int m)
{
	sparse_A->MN = malloc(len * sizeof(int));
	if (!sparse_A->MN)
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	sparse_A->IM = malloc(len * sizeof(int));
	if (!sparse_A->IM)
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	sparse_A->JM = malloc(m * sizeof(int));
	if (!sparse_A->JM)
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	for (int j = 0; j < m; j++)
		(sparse_A->JM)[j] = -1;
	return SUCCESS;
}

int create_sparse_matrix(int **A, int n, int m, sparse_M *sparse_A, int *lenA)
{
	int len = count_nonzero_elems(A, n, m);
	*lenA = len;
	sparse_A->MN = malloc(len * sizeof(int));
	if (!sparse_A->MN)
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	sparse_A->IM = malloc(len * sizeof(int));
	if (!sparse_A->IM)
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	sparse_A->JM = malloc(m * sizeof(int));
	if (!sparse_A->JM)
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	int cnt = 0;
	for (int j = 0; j < m; j++)
	{
		int col_elems = 0;
		for (int i = 0; i < n; i++)
			if (A[i][j])
			{
				(sparse_A->MN)[cnt] = A[i][j];
				(sparse_A->IM)[cnt] = i;
				col_elems++;
				cnt++;
			}
		(sparse_A->JM)[j] = cnt - col_elems;
	}
	return SUCCESS;
}

void fill_col_entry(sparse_M sparse, int **matrix, int n, int m)
{
    int elems = 0;
    for (int col = 0; col < m; col++)
    {
        int col_elems = 0;
        for (int row = 0; row < n; row++)
        {
            if (matrix[row][col])
            {
                col_elems++;
                elems++;
            }
        }
        sparse.JM[col] = elems - col_elems;
    }
}

void add_up_matrix(int **A, int **B, int **S, int n, int m)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			S[i][j] = A[i][j] + B[i][j];
}

void print_sparse_matrix(sparse_M sparse_A, int len, int m)
{
	printf("N: ");
	print_array(sparse_A.MN, len);
	printf("\nJ: ");
	print_array(sparse_A.IM, len);
	printf("\nI: ");
	print_array(sparse_A.JM, m);
	printf("\n");
}

void print_array(int *a, int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", a[i]);
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

int generate_matrix(int ***matrix, int n, int m)
{
	int perc;
	printf("Input zero elements percentage: ");
	if (scanf("%d", &perc) != 1 || perc < 0 || perc > 100)
	{
		printf("Wrong input\n");
		return INPUT_ERR;
	}
	if (allocate_matrix(matrix, n, m))
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	int i = 0, j = 0;
	for (int k = 0; k < (int)((1 - (float)perc / 100) * n * m); k++)
	{
		while ((*matrix)[i][j])
		{
			i = rand() % n;
			j = rand() % m;
		}
		(*matrix)[i][j] = (rand() % 1000) + 1;
	}
	return SUCCESS;
}

int input_matrix(int ***matrix, int n, int m)
{
	if (allocate_matrix(matrix, n, m))
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	printf("Input matrix elements:\n");
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (scanf("%d", &(*matrix)[i][j]) != 1)
			{
				printf("Input error\n");
				return INPUT_ERR;
			}
	return SUCCESS;
}

int input_matrix_from_file(int ***matrix, int n, int m)
{
	char name[30];

    printf("Input source file name (with extension): ");
    scanf("%s", name);

	FILE *f = fopen(name, "r");
	if (f == NULL)
    {
        printf("Can't open file\n\n");
        return FILE_ERR;
    }
	if (allocate_matrix(matrix, n, m))
	{
		printf("Memory error\n");
		fclose(f);
		return MEMORY_ERR;
	}
	printf("Input matrix elements:\n");
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (fscanf(f, "%d", &(*matrix)[i][j]) != 1)
			{
				printf("Input error\n");
				fclose(f);
				return INPUT_ERR;
			}
	fclose(f);
	return SUCCESS;
}

int allocate_matrix(int ***data, int n, int m)
{
	*data = calloc(n, sizeof(int *));
	if (!*data)
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}
	for (int i = 0; i < n; i++)
	{
		(*data)[i] = calloc(m, sizeof(int));
		if (!(*data)[i])
		{
			free_matrix(*data, n);
			printf("Memory error\n");
			return MEMORY_ERR;
		}
	}
	return SUCCESS;
}

void free_sparse_matrix(sparse_M sparse_A)
{
	if (sparse_A.MN)
		free(sparse_A.MN);
	if (sparse_A.IM)
		free(sparse_A.IM);
	if (sparse_A.JM)
		free(sparse_A.JM);
}

void free_matrix(int **data, int n)
{
	if (data)
	{
		for (int i = 0; i < n; i++)
			free(data[i]);
		free(data);
	}
}

void add_sparse_matrix(sparse_M matrix_a, sparse_M matrix_b, sparse_M *res, int m, int lenA, int lenB, int *lenS)
{
    int a_ind = 0, b_ind = 0, res_ind = 0;
    for (int i = 0; i < m; i++)
    {
        res->JM[i] = res_ind;
        if (i < m - 1)
        {
            while (a_ind < matrix_a.JM[i + 1] || b_ind < matrix_b.JM[i + 1])
            {
                if (a_ind < matrix_a.JM[i + 1] && b_ind < matrix_b.JM[i + 1])
                {
                    if (matrix_a.IM[a_ind] == matrix_b.IM[b_ind])
                    {
						res->MN[res_ind] = matrix_a.MN[a_ind] + matrix_b.MN[b_ind];
						res->IM[res_ind] = matrix_a.IM[a_ind];
                        b_ind++;
                        a_ind++;
                    }
                    else if (matrix_a.IM[a_ind] > matrix_b.IM[b_ind])
                    {
                        res->MN[res_ind] = matrix_b.MN[b_ind];
                        res->IM[res_ind] = matrix_b.IM[b_ind];
                        b_ind++;
                    }
                    else
                    {
                        res->MN[res_ind] = matrix_a.MN[a_ind];
                        res->IM[res_ind] = matrix_a.IM[a_ind];
                        a_ind++;
                    }
                }
                else if (a_ind == matrix_a.JM[i + 1])
                {
                    res->MN[res_ind] = matrix_b.MN[b_ind];
                    res->IM[res_ind] = matrix_b.IM[b_ind];
                    b_ind++;
                }
                else if (b_ind == matrix_b.JM[i + 1])
                {
                    res->MN[res_ind] = matrix_a.MN[a_ind];
                    res->IM[res_ind] = matrix_a.IM[a_ind];
                    a_ind++;
                }
                res_ind++;
            }
        }
        else
        {
            while (a_ind < lenA || b_ind < lenB)
            {
                if (a_ind < lenA && b_ind < lenB)
                {
                    if (matrix_a.IM[a_ind] == matrix_b.IM[b_ind])
                    {
						res->MN[res_ind] = matrix_a.MN[a_ind] + matrix_b.MN[b_ind];
						res->IM[res_ind] = matrix_a.IM[a_ind];
                        b_ind++;
                        a_ind++;
                    }
                    else if (matrix_a.IM[a_ind] > matrix_b.IM[b_ind])
                    {
                        res->MN[res_ind] = matrix_b.MN[b_ind];
                        res->IM[res_ind] = matrix_b.IM[b_ind];
                        b_ind++;
                    }
                    else
                    {
                        res->MN[res_ind] = matrix_a.MN[a_ind];
                        res->IM[res_ind] = matrix_a.IM[a_ind];
                        a_ind++;
                    }
                }
                else if (a_ind == lenA)
                {
                    res->MN[res_ind] = matrix_b.MN[b_ind];
                    res->IM[res_ind] = matrix_b.IM[b_ind];
                    b_ind++;
                }
                else if (b_ind == lenB)
                {
                    res->MN[res_ind] = matrix_a.MN[a_ind];
                    res->IM[res_ind] = matrix_a.IM[a_ind];
                    a_ind++;
                }
                res_ind++;
            }
        }
    }
    *lenS = res_ind;
}

unsigned long long tick(void)
{
    unsigned long long d;

    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

void remove_zeros(sparse_M matrix, int *len)
{
	int n = *len;
	int i, j, q = 0, k = 0;
	for (i = 0; i < n; i++)
	{
		if (!matrix.MN[k])
		{
			for (j = k; j < n - 1; j++)
			{
				matrix.MN[j] = matrix.MN[j + 1];
				matrix.IM[j] = matrix.IM[j + 1];
			}
			k--;
			q++;
		}
		k++;
	}
	*len = n - q;
}