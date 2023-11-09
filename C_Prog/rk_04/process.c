#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"
#include "defines.h"

void divide_list(node_t *head, node_t **mil_head, node_t **ost_head)
{
	node_t **mil_next = mil_head, **ost_next = ost_head;
	for (; head; head = head->next)
	{
		if (head->population > 999999)
		{
			*mil_next = head;
			mil_next = &(head->next);
		}
		else
		{
			*ost_next = head;
			ost_next = &(head->next);
		}
	}
	*mil_next = NULL, *ost_next = NULL;
}

void unite_lists(node_t *head1, node_t *head2)
{
	while (head1->next)
		head1 = head1->next;
	head1->next = head2;
}

int create_node(node_t **node)
{
	int rc = SUCCESS;
	*node = malloc(sizeof(node_t));
	if (*node)
		(*node)->next = NULL;
	else
		rc = MEMORY_ERR;
	return rc;
}

void free_list(node_t *head)
{
	node_t *next;
	for (; head; head = next)
	{
		next = head->next;
		free(head);
	}
}