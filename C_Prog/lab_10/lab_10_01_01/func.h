#ifndef _FUNC_H_
#define _FUNC_H_
#include "defines.h"

void insert(node_t **head, node_t *elem, node_t *before);
node_t *create_node(void);
int copy(node_t *head, node_t **new_head);
node_t *sort(node_t *head, int (*comparator)(const void *, const void *));
void front_back_split(node_t *head, node_t **back);
node_t *sorted_merge(node_t **head_a, node_t **head_b, int (*comparator)(const void *, const void *));
int create_list(node_t **head, int len);
void free_list(node_t *head);
int compare_by_year(const void *arg1, const void *arg2);
void fill_list(node_t *head, town_t *towns);
int equal_lists(node_t *head1, node_t *head2);

#endif