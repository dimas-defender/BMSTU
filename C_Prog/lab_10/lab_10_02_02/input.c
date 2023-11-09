#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defines.h"
#include "process.h"
#include "input.h"

int input_polynom(node_t **head, int *degree)
{
	int rc = SUCCESS;
	int n;
	if (scanf("%d", &n) != 1 || n < 1)
		rc = INPUT_ERR;
	else
	{
		*degree = n - 1;
		rc = create_list(head, n);
		if (!rc)
		{
			node_t *cur = *head;
			int counter = 0;
			char sep = ' ';
			while (sep != '\n' && !rc && counter < n)
			{
				if (scanf("%d%c", &(cur->coef), &sep) != 2)
					rc = INPUT_ERR;
				else
				{
					cur = cur->next;
					counter++;
				}
			}
			if (sep != '\n' || counter != n)
				rc = INPUT_ERR;
		}
	}
	return rc;
}