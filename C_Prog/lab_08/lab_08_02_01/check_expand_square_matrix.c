#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "defines.h"

START_TEST(test_one_elem_matrix)
{
	int old_size = 1;
	int new_size = 3;
	int *in = malloc(old_size * old_size * sizeof(int));
	in[0] = 5;
	int out[] = {5, 5, 5, 5, 5, 5, 5, 5, 5};
	int ret_key = expand_square_matrix(&in, old_size, old_size, new_size);
	ck_assert_int_eq(ret_key, SUCCESS);
	for (int i = 0; i < new_size; i++)
		for (int j = 0; j < new_size; j++)
			ck_assert_int_eq(in[i * new_size + j], out[i * new_size + j]);
	free(in);
}
END_TEST

START_TEST(test_same_size)
{
	int old_size = 3;
	int new_size = 3;
	int *in = malloc(old_size * old_size * sizeof(int));
	in[0] = 1;
	in[1] = 2;
	in[2] = 3;
	in[3] = 4;
	in[4] = 5;
	in[5] = 6;
	in[6] = 7;
	in[7] = 8;
	in[8] = 9;
	int out[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int ret_key = expand_square_matrix(&in, old_size, old_size, new_size);
	ck_assert_int_eq(ret_key, SUCCESS);
	for (int i = 0; i < new_size; i++)
		for (int j = 0; j < new_size; j++)
			ck_assert_int_eq(in[i * new_size + j], out[i * new_size + j]);
	free(in);
}
END_TEST

START_TEST(test_positive_avg)
{
	int old_size = 2;
	int new_size = 4;
	int *in = malloc(old_size * old_size * sizeof(int));
	in[0] = 5;
	in[1] = -2;
	in[2] = 7;
	in[3] = 13;
	int out[] = {5, -2, -2, -2, 7, 13, 7, 7, 6, 5, 5, 5, 6, 5, 5, 5};
	int ret_key = expand_square_matrix(&in, old_size, old_size, new_size);
	ck_assert_int_eq(ret_key, SUCCESS);
	for (int i = 0; i < new_size; i++)
		for (int j = 0; j < new_size; j++)
			ck_assert_int_eq(in[i * new_size + j], out[i * new_size + j]);
	free(in);
}
END_TEST

START_TEST(test_negative_avg)
{
	int old_size = 3;
	int new_size = 4;
	int *in = malloc(old_size * old_size * sizeof(int));
	in[0] = 3;
	in[1] = -10;
	in[2] = 0;
	in[3] = -8;
	in[4] = 2;
	in[5] = -20;
	in[6] = -5;
	in[7] = 1;
	in[8] = -1;
	int out[] = {3, -10, 0, -10, -8, 2, -20, -20, -5, 1, -1, -5, -4, -3, -7, -7};
	int ret_key = expand_square_matrix(&in, old_size, old_size, new_size);
	ck_assert_int_eq(ret_key, SUCCESS);
	for (int i = 0; i < new_size; i++)
		for (int j = 0; j < new_size; j++)
			ck_assert_int_eq(in[i * new_size + j], out[i * new_size + j]);
	free(in);
}
END_TEST

Suite *expand_square_matrix_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("expand_square_matrix");
	
	tc_all = tcase_create("all");
	tcase_add_test(tc_all, test_one_elem_matrix);
	tcase_add_test(tc_all, test_same_size);
	tcase_add_test(tc_all, test_positive_avg);
	tcase_add_test(tc_all, test_negative_avg);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}