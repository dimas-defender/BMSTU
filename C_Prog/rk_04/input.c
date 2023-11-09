#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "input.h"
#include "process.h"
#include "defines.h"

int read_records(node_t **head)
{
	int rc = SUCCESS;
	char name[STR_LEN + 1] = {'0'};
	char region[STR_LEN + 1];
	char okrug[STR_LEN + 1];
	int population;
	int year;
	while (!rc && strcmp(name, "end"))
	{
		if (!fgets(name, STR_LEN + 1, stdin))
			rc = READ_ERR;
		else if (name[strlen(name) - 1] == '\n')
			name[strlen(name) - 1] = '\0';
		if (!rc && strcmp(name, "end"))
		{
			if (!fgets(region, STR_LEN + 1, stdin))
				rc = READ_ERR;
			else if (region[strlen(region) - 1] == '\n')
				region[strlen(region) - 1] = '\0';
			if (!rc)
			{
				if (!fgets(okrug, STR_LEN + 1, stdin))
					rc = READ_ERR;
				else if (okrug[strlen(okrug) - 1] == '\n')
					okrug[strlen(okrug) - 1] = '\0';
			}
			if (!rc && scanf("%d\n", &population) == 1)
			{
				if (population < 0)
					rc = DATA_ERR;
				else if (scanf("%d\n", &year) != 1)
					rc = READ_ERR;
			}
			else
				rc = READ_ERR;
			if (!rc)
			{
				node_t *node, *last;
				rc = create_node(&node);
				if (!rc)
				{
					strcpy(node->name, name);
					strcpy(node->region, region);
					strcpy(node->okrug, okrug);
					node->population = population;
					node->year = year;
					if (!(*head))
						*head = node;
					else
						last->next = node;
					last = node;
				}
			}
		}
	}
	return rc;
}