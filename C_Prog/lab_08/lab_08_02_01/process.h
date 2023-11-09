#ifndef _PROCESS_H_
#define _PROCESS_H_

void make_matrix_square(int *matrix, int n, int m);
int expand_square_matrix(int **matrix, int old_size_rows, int old_size_cols, int new_size);
int calculate_matrix_expression(int *a, int *b, int **res, int powa, int powb, int size);
void free_data(int *data);

#endif