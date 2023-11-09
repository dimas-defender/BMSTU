#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "output.h"
#include "defines.h"

void print_list(node_t *head)
{
	while (head)
	{
		printf("%s\n", head->name);
		printf("%s\n", head->region);
		printf("%s\n", head->okrug);
		printf("%d\n", head->population);
		printf("%d\n", head->year);
		head = head->next;
	}
	printf("end");
}