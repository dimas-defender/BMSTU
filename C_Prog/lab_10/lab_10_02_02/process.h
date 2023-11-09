#ifndef _PROCESS_H_
#define _PROCESS_H_
#include "defines.h"

int create_list(node_t **head, int len);
void free_list(node_t *head);
int get_polynom_value(node_t *head, int deg, int a);
void get_derivative(node_t *head, node_t *deriv_head, int deg);
void sum_polynom(node_t *higher_head, node_t *lower_head, node_t *sum_head, int deg_diff);
void divide_into_even_and_odd(node_t *head, node_t *even_head, node_t *odd_head, int deg);
int equal_lists(node_t *head1, node_t *head2);

#endif