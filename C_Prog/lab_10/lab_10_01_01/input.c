#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "input.h"

int input_item(FILE *f, town_t *towns)
{
	int rc = SUCCESS;
	if (!fgets(towns->name, NAME_LEN + 1, f))
		rc = READ_ERR;
	else if ((towns->name)[strlen(towns->name) - 1] == '\n')
		(towns->name)[strlen((towns->name)) - 1] = '\0';
	if (rc || fscanf(f, "%d\n", &(towns->year)) != 1)
		rc = READ_ERR;
	if (!rc && (!strlen(towns->name) || strlen(towns->name) > NAME_LEN || towns->year < 1))
		rc = DATA_ERR;
	return rc;
}

int read_file(char *str, town_t *towns, int *quantity)
{
	int rc = SUCCESS;
	FILE *f = fopen(str, "r");
	if (f == NULL)
		rc = WRONG_FILE;
	else
	{
		int len = 0;
		while (!rc && !feof(f))
		{
			rc = input_item(f, towns + len);
			if (!rc)
				len++;
		}
		if (f != NULL)
			fclose(f);
		if (!len || len > MAX_RECORDS)
			rc = WRONG_FILE;
		else
			*quantity = len;
	}
	return rc;
}