#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "input.h"
#include "output.h"
#include "process.h"
#include "defines.h"

int main(void)
{
	setbuf(stdout, NULL);
	int rc = SUCCESS;
	node_t *head = NULL;
	rc = read_records(&head);
	if (!rc)
	{
		node_t *mil_head = NULL, *ost_head = NULL;
		divide_list(head, &mil_head, &ost_head);
		unite_lists(mil_head, ost_head);
		print_list(mil_head);
		free_list(mil_head);
	}
	else
		free_list(head);
	return rc;
}