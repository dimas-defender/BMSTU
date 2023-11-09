#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defines.h"
#include "process.h"

void divide_into_even_and_odd(node_t *head, node_t *even_head, node_t *odd_head, int deg)
{
	while (head)
	{
		if (deg % 2 == 0)
		{
			even_head->coef = head->coef;
			even_head = even_head->next;
		}
		else
		{
			odd_head->coef = head->coef;
			odd_head = odd_head->next;
		}
		head = head->next;
		deg--;
	}
}

int get_polynom_value(node_t *head, int deg, int a)
{
	int value = 0;
	for (node_t *cur = head; cur; cur = cur->next)
	{
		value += cur->coef * pow(a, deg);
		deg--;
	}
	return value;
}

int create_node(node_t **node)
{
	int rc = SUCCESS;
	*node = malloc(sizeof(node_t));
	if (*node)
	{
		(*node)->coef = 0;
		(*node)->next = NULL;
	}
	else
		rc = MEMORY_ERR;
	return rc;
}

int create_list(node_t **head, int len)
{
	int rc = SUCCESS;
	node_t *last, *cur;
	rc = create_node(&last);
	if (!rc)
		*head = last;
	for (int i = 0; i < len - 1 && !rc; i++)
	{
		rc = create_node(&cur);
		last->next = cur;
		last = cur;
	}
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

void get_derivative(node_t *head, node_t *deriv_head, int deg)
{
	node_t *cur = head, *deriv_cur = deriv_head;
	while (cur->next)
	{
		deriv_cur->coef = cur->coef * deg;
		deg--;
		cur = cur->next;
		deriv_cur = deriv_cur->next;
	}
}

void sum_polynom(node_t *higher_head, node_t *lower_head, node_t *sum_head, int deg_diff)
{
	for (int i = 0; i < deg_diff; i++)
	{
		sum_head->coef = higher_head->coef;
		higher_head = higher_head->next;
		sum_head = sum_head->next;
	}
	while (higher_head)
	{
		sum_head->coef = higher_head->coef + lower_head->coef;
		higher_head = higher_head->next;
		lower_head = lower_head->next;
		sum_head = sum_head->next;
	}
}

int equal_lists(node_t *head1, node_t *head2)
{
	int res = 1;
	while (head1)
	{
		if (head1->coef != head2->coef)
		{
			res = 0;
			break;
		}
		head1 = head1->next;
		head2 = head2->next;
	}
	return res;
}