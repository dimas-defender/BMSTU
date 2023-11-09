#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define FILE_ERR 1
#define DATA_ERR 2
#define MEMORY_ERR 3

int allocate_matrix(int n, int m, int ***data);
void free_data(int **data, int n);
unsigned long long tick(void);
int input_graph(char *name, int *n, int ***matrix);
void export_to_dot(int **matrix, int n);
void init_arr(int *a, int len);
int dfs(int u, int **matrix, int n, int *visited);

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Некорректное количество аргументов командой строки!\n");
		return FILE_ERR;
	}
	setbuf(stdout, NULL);
    printf("Данная программа позволяет построить неориентированный граф по данным из файла и\nопределить, является ли он связным.\n\n");
	int n;
	int **matrix;
	if (input_graph(argv[1], &n, &matrix))
		return FILE_ERR;
	int visited[n];
	init_arr(visited, n);
	unsigned long time = tick();
	int cnt_visited = dfs(0, matrix, n, visited);
	time = tick() - time;
	if (cnt_visited == n)
		printf("Заданный граф связен\n");
	else
		printf("Заданный граф не является связным\n");
	printf("Время работы алгоритма: %lu тактов процессора\n", time);
	unsigned long size = sizeof(int) * n * n + sizeof(int *) * n;
	printf("Объем занимаемой памяти: %lu байт\n", size);
	export_to_dot(matrix, n);
	system("dot -Tpng graph.txt -o graph.png");
	system("graph.png");
	free_data(matrix, n);
	return SUCCESS;
}

int dfs(int u, int **matrix, int n, int *visited)
{
	int cnt_visited = 1;
	visited[u] = 1;
	for (int v = 0; v < n; v++)
		if (matrix[u][v])
			if (!visited[v])
				cnt_visited += dfs(v, matrix, n, visited);
	return cnt_visited;
}

void init_arr(int *a, int len)
{
	for (int i = 0; i < len; i++)
		a[i] = 0;
}

void export_to_dot(int **matrix, int n)
{
	FILE *f = fopen("graph.txt", "w");
    fprintf(f, "graph %s {\n", "mygraph");
	for (int i = 0; i < n; i++)
	{
		fprintf(f, "%d;\n", i);
		for (int j = i; j < n; j++)
			if (matrix[i][j])
				fprintf(f, "%d -- %d;\n", i, j);
	}
    fprintf(f, "}\n");
	fclose(f);
}


int input_graph(char *name, int *n, int ***matrix)
{
	FILE *f = fopen(name, "r");
    if (f == NULL)
    {
        printf("Неверное имя файла!\n");
        return FILE_ERR;
    }
    if (fgetc(f) == EOF)
    {
        printf("Файл пуст!\n");
        return FILE_ERR;
    }
	rewind(f);
	
	if (fscanf(f, "%d", n) != 1 || *n < 1)
	{
		printf("Некорректное количество вершин графа!\n");
		return DATA_ERR;
	}
	if (allocate_matrix(*n, *n, matrix))
	{
		printf("Ошибка выделения памяти!\n");
		return MEMORY_ERR;
	}
	
	int num1, num2;
	while (!feof(f))
	{
		if (fscanf(f, "%d %d\n", &num1, &num2) == 2 && num1 > -1 && num1 < *n && num2 > -1 && num1 < *n)
		{
			(*matrix)[num1][num2] = 1;
			(*matrix)[num2][num1] = 1;
		}
		else
		{
			printf("Некорректный ввод ребер графа!\n");
			return DATA_ERR;
		}
	}
	return SUCCESS;
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
			(*data)[i] = calloc(m, sizeof(int));
			if (!(*data)[i])
			{
				rc = MEMORY_ERR;
				free_data(*data, n);
			}
		}
	}
	return rc;
}

void free_data(int **data, int n)
{
	for (int i = 0; i < n; i++)
		free(data[i]);
	free(data);
}

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );
    return d;
}