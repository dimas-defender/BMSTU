#include <stdlib.h>
#include "func.h"
#include "defines.h"

void insert(node_t **head, node_t *elem, node_t *before)
{
	if (!head || !(*head) || !elem || !before)
		return;
	if (*head == before)
	{
		elem->next = *head;
		*head = elem;
		return;
	}
	node_t *cur = *head;
	while (cur->next != before && cur->next != NULL)
		cur = cur->next;
	if (cur->next == NULL)
		return;
	elem->next = before;
	cur->next = elem;
}

node_t *create_node(void)
{
	node_t *node = malloc(sizeof(node_t));
	if (node)
		node->next = NULL;
	return node;
}

int copy(node_t *head, node_t **new_head)
{
	if (!head || !new_head)
		return PTR_ERR;
	node_t *cur, *new, *prev;
	for (cur = head; cur; cur = cur->next)
	{
		new = create_node();
		if (!new)
			return MEMORY_ERR;
		new->data = cur->data;
		if (cur == head)
			*new_head = new;
		else
			prev->next = new;
		prev = new;
	}
	return SUCCESS;
}

node_t *sort(node_t *head, int (*comparator)(const void *, const void *))
{
	if (!head)
		return NULL;
	if (!head->next)
		return head;
	node_t *back = NULL;
	front_back_split(head, &back);
	head = sort(head, comparator);
	back = sort(back, comparator);
	head = sorted_merge(&head, &back, comparator);
	return head;
}

void front_back_split(node_t *head, node_t **back)
{
	if (!head || !back)
		return;
	int len = 0;
	node_t *cur;
	for (cur = head; cur; cur = cur->next)
		len++;
	len = (len + 1) / 2;
	cur = head;
	for (int i = 0; i < len - 1; i++)
		cur = cur->next;
	*back = cur->next;
	cur->next = NULL;
}

node_t *sorted_merge(node_t **head_a, node_t **head_b, int (*comparator)(const void *, const void *))
{
	if (!head_a || !head_b || (!(*head_a) && !(*head_b)))
		return NULL;
	node_t *cur_a = *head_a, *cur_b = *head_b, *new_head = NULL, *last;
	while (cur_a && cur_b)
	{
		if (comparator(cur_a->data, cur_b->data) <= 0)
		{
			if (!new_head)
				new_head = cur_a;
			else
				last->next = cur_a;
			last = cur_a;
			cur_a = cur_a->next;
		}
		else
		{
			if (!new_head)
				new_head = cur_b;
			else
				last->next = cur_b;
			last = cur_b;
			cur_b = cur_b->next;
		}
	}
	if (!new_head)
		new_head = *head_a ? *head_a : *head_b;
	else
	{
		if (cur_a)
			last->next = cur_a;
		if (cur_b)
			last->next = cur_b;
	}
	*head_a = NULL;
	*head_b = NULL;
	return new_head;
}

int equal_lists(node_t *head1, node_t *head2)
{
	int res = 1;
	while (head1)
	{
		if (head1->data != head2->data)
		{
			res = 0;
			break;
		}
		head1 = head1->next;
		head2 = head2->next;
	}
	return res;
}

int create_list(node_t **head, int len)
{
	int rc = SUCCESS;
	node_t *last, *cur;
	last = create_node();
	if (last)
		*head = last;
	else
		rc = MEMORY_ERR;
	for (int i = 0; i < len - 1 && !rc; i++)
	{
		cur = create_node();
		if (cur)
		{
			last->next = cur;
			last = cur;
		}
		else
			rc = MEMORY_ERR;
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

int compare_by_year(const void *arg1, const void *arg2)
{
	return ((town_t*)arg1)->year - ((town_t*)arg2)->year;
}

void fill_list(node_t *head, town_t *towns)
{
	int i = 0;
	while (head)
	{
		head->data = towns + i;
		head = head->next;
		i++;
	}
}