#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "process.h"

START_TEST(test_square_matrix)
{
	int n = 3, m = 3;
	int k = n < m ? n : m;
	int in[] = {27, 16, 8, 8, -10, 25, -32, 5, 16};
	int out[] = {27, 16, 8, 8, -10, 25, -32, 5, 16};
	make_matrix_square(in, n, m);
	ck_assert_int_eq(k, n);
	ck_assert_int_eq(k, m);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			ck_assert_int_eq(in[i * k + j], out[i * k + j]);    
}
END_TEST

START_TEST(test_delete_first_row)
{
	int n = 4, m = 3;
	int k = n < m ? n : m;
	int in[] = {5, 16, -2, 7, 4, 10, 13, 8, 11, 1, 0, 4};
	int out[] = {7, 4, 10, 13, 8, 11, 1, 0, 4};
	make_matrix_square(in, n, m);
	ck_assert_int_eq(k, m);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			ck_assert_int_eq(in[i * k + j], out[i * k + j]);    
}
END_TEST

START_TEST(test_delete_first_col)
{
	int n = 2, m = 3;
	int k = n < m ? n : m;
	int in[] = {7, -2, 10, 13, 5, 11};
	int out[] = {-2, 10, 5, 11};
	make_matrix_square(in, n, m);
	ck_assert_int_eq(k, n);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			ck_assert_int_eq(in[i * k + j], out[i * k + j]);    
}
END_TEST

START_TEST(test_delete_last_row)
{
	int n = 4, m = 3;
	int k = n < m ? n : m;
	int in[] = {5, 0, -2, 7, 4, 10, 13, 8, 11, 1, 16, 4};
	int out[] = {5, 0, -2, 7, 4, 10, 13, 8, 11};
	make_matrix_square(in, n, m);
	ck_assert_int_eq(k, m);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			ck_assert_int_eq(in[i * k + j], out[i * k + j]);    
}
END_TEST

START_TEST(test_delete_last_col)
{
	int n = 2, m = 3;
	int k = n < m ? n : m;
	int in[] = {7, -2, 10, 11, 5, 13};
	int out[] = {7, -2, 11, 5};
	make_matrix_square(in, n, m);
	ck_assert_int_eq(k, n);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			ck_assert_int_eq(in[i * k + j], out[i * k + j]);    
}
END_TEST

START_TEST(test_delete_last_max_elem)
{
	int n = 2, m = 3;
	int k = n < m ? n : m;
	int in[] = {7, 13, 10, 11, 5, 13};
	int out[] = {7, 13, 11, 5};
	make_matrix_square(in, n, m);
	ck_assert_int_eq(k, n);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			ck_assert_int_eq(in[i * k + j], out[i * k + j]);    
}
END_TEST

START_TEST(test_few_removals)
{
	int n = 5, m = 3;
	int k = n < m ? n : m;
	int in[] = {1, 2, 10, 5, -7, 8, 14, 9, 3, -6, 0, 4, 15, 15, -2};
	int out[] = {1, 2, 10, 5, -7, 8, -6, 0, 4};
	make_matrix_square(in, n, m);
	ck_assert_int_eq(k, m);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			ck_assert_int_eq(in[i * k + j], out[i * k + j]);    
}
END_TEST

Suite *make_matrix_square_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("make_matrix_square");
	
	tc_all = tcase_create("all");
	tcase_add_test(tc_all, test_square_matrix);
	tcase_add_test(tc_all, test_delete_first_row);
	tcase_add_test(tc_all, test_delete_first_col);
	tcase_add_test(tc_all, test_delete_last_row);
	tcase_add_test(tc_all, test_delete_last_col);
	tcase_add_test(tc_all, test_delete_last_max_elem);
	tcase_add_test(tc_all, test_few_removals);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}