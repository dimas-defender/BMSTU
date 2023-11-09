#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "defines.h"

START_TEST(test_one_zero_power)
{
	int size = 3;
	int powa = 0, powb = 2;
	int A[] = {15, -2, 7, 9, 0, 1, 6, 3, -8};
	int B[] = {21, -4, 7, 0, 2, 12, -8, 5, -3};
	int *RES = NULL;
	int RES_CORRECT[] = {385, -57, 78, -96, 64, -12, -144, 27, 13};
	
	int ret_key = calculate_matrix_expression(A, B, &RES, powa, powb, size);
	ck_assert_int_eq(ret_key, SUCCESS);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			ck_assert_int_eq(RES[i * size + j], RES_CORRECT[i * size + j]);
	free_data(RES);
}
END_TEST

START_TEST(test_both_zero_power)
{
	int size = 4;
	int powa = 0, powb = 0;
	int A[] = {15, -2, 7, 9, 0, 1, 6, 3, -8, 12, -10, 25, -7, 105, 3, -37};
	int B[] = {21, -4, 7, 0, 2, 12, -8, 5, -3, 275, -21, 54, 0, 1, 35, -59};
	int *RES = NULL;
	int RES_CORRECT[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	
	int ret_key = calculate_matrix_expression(A, B, &RES, powa, powb, size);
	ck_assert_int_eq(ret_key, SUCCESS);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			ck_assert_int_eq(RES[i * size + j], RES_CORRECT[i * size + j]);
	free_data(RES);
}
END_TEST

START_TEST(test_not_zero_power)
{
	int size = 3;
	int powa = 2, powb = 3;
	int A[] = {8, -2, 7, 9, 0, 1, 6, 3, -8};
	int B[] = {11, -4, 7, 0, 2, 12, -8, 5, -3};
	int *RES = NULL;
	int RES_CORRECT[] = {54104, -20842, 20962, 20738, -5967, 6761, -35935, 17745, -16390};
	
	int ret_key = calculate_matrix_expression(A, B, &RES, powa, powb, size);
	ck_assert_int_eq(ret_key, SUCCESS);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			ck_assert_int_eq(RES[i * size + j], RES_CORRECT[i * size + j]);
	free_data(RES);
}
END_TEST

Suite *calculate_matrix_expression_suite(void)
{
	Suite *s;
	TCase *tc_pos;
	
	s = suite_create("calculate_matrix_expression");
	
	tc_pos = tcase_create("positives");
	tcase_add_test(tc_pos, test_one_zero_power);
	tcase_add_test(tc_pos, test_both_zero_power);
	tcase_add_test(tc_pos, test_not_zero_power);
	suite_add_tcase(s, tc_pos);
	
	return s;
}