#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defines.h"
#include "output.h"

void print_polynom(node_t *head)
{
	int flag = 0;
	while (head)
	{
		if (flag || head->coef)
		{
			flag = 1;
			printf("%d ", head->coef);
		}
		head = head->next;
	}
	printf("L\n");
}