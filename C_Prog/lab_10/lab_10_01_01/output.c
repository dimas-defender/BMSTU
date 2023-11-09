#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "output.h"

void print_towns(char *str, node_t *head)
{
	FILE *f = fopen(str, "w");
	while (head)
	{
		fprintf(f, "%s\n%d\n", ((town_t*)(head->data))->name, ((town_t*)(head->data))->year);
		head = head->next;
	}
	fclose(f);
}