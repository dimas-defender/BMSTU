#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "defines.h"
#include "input.h"

ssize_t my_getline(char **lineptr, size_t *n, FILE *stream)
{
	static char line[GETLINE_BUFLEN];
	char *ptr;
	ssize_t len;
	
	if (lineptr == NULL || n == NULL)
		return -1;
	
	if (ferror(stream))
		return -1;
	
	if (feof(stream))
		return -1;
	
	fgets(line, GETLINE_BUFLEN, stream);
	ptr = strchr(line, '\n');
	if (ptr)
		*ptr = '\0';
	len = (ssize_t) strlen(line);
	if (len + 1 < GETLINE_BUFLEN)
	{
		ptr = realloc(*lineptr, GETLINE_BUFLEN);
		if (ptr == NULL)
			return -1;
		*lineptr = ptr;
		*n = GETLINE_BUFLEN;
	}
	strcpy(*lineptr, line);
	return len;
}

int read_record(FILE *f)
{
	int ret_key = SUCCESS;
	char *line = NULL;
	size_t len = 0;
	float weight, size;
	if (my_getline(&line, &len, f) == -1)
		ret_key = READ_ERR;
	if (!ret_key && fscanf(f, "%f\n%f\n", &weight, &size) != 2)
		ret_key = READ_ERR;
	if (!ret_key && (!len || weight <= 0 || size <= 0))
		ret_key = DATA_ERR;
	free(line);
	return ret_key;
}

int count_records(FILE *f, int *len)
{
	int ret_key = SUCCESS;
	int cnt = 0;
	while (!ret_key && !feof(f))
	{
		if (!read_record(f))
			cnt++;
		else
			ret_key = DATA_ERR;
	}
	if (!cnt)
		ret_key = DATA_ERR;
	if (!ret_key)
		*len = cnt;
	return ret_key;
}

void input_record(FILE *f, item_t *items)
{
	size_t len = 0;
	items->name = NULL;
	my_getline(&(items->name), &len, f);
	fscanf(f, "%f\n%f\n", &(items->weight), &(items->size));
}

void fill_records_array(FILE *f, item_t *items, int n)
{
	for (int i = 0; i < n; i++)
		input_record(f, items + i);
}

int read_file(char *str, item_t **items, int *quantity)
{
	int ret_key = SUCCESS;
	FILE *f = fopen(str, "r");
	if (f == NULL)
		ret_key = NO_FILE;
	else
	{
		int len;
		ret_key = count_records(f, &len);
		if (!ret_key)
		{
			*items = malloc(len * sizeof(item_t));
			if (!*items)
				ret_key = MEMORY_ERR;
			else
			{
				*quantity = len;
				rewind(f);
				fill_records_array(f, *items, len);
			}
		}
		fclose(f);
	}
	return ret_key;
}