#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "defines.h"

int create_node(node_t **node);
void free_list(node_t *head);
void divide_list(node_t *head, node_t **mil_head, node_t **ost_head);
void unite_lists(node_t *head1, node_t *head2);

#endif