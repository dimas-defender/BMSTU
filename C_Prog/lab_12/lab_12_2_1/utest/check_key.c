#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../stat/process.h"
#include "../stat/defines.h"

START_TEST(test_key_one_elem_out)
{
	int in[7] = {15, -7, 234, 0, 23, 2, 11};
	int out_correct[1] = {15};
	
	int *out_beg = NULL, *out_end = NULL;
	int ret_key = key(in, in + 7, &out_beg, &out_end);
	ck_assert_int_eq(ret_key, SUCCESS);
	ck_assert_int_eq(out_end - out_beg, 1);
	ck_assert_int_eq(out_beg[0], out_correct[0]);
	
	if (out_beg)
		free(out_beg);
}
END_TEST

START_TEST(test_key_random_array)
{
	int in[9] = {52, 17, 6, 234, 8, 0, -9, 59, -23};
	int out_correct[6] = {52, 17, 6, 234, 8, 0};
	
	int *out_beg = NULL, *out_end = NULL;
	int ret_key = key(in, in + 9, &out_beg, &out_end);
	ck_assert_int_eq(ret_key, SUCCESS);
	ck_assert_int_eq(out_end - out_beg, 6);
	for (size_t i = 0; i < 6; i++)
		ck_assert_int_eq(out_beg[i], out_correct[i]);
	
	if (out_beg)
		free(out_beg);
}
END_TEST

START_TEST(test_key_no_neg_numbers)
{
	int in[9] = {31, 415, 1, 53, 516, 13, 7, 0, 21};
	int out_correct[9] = {31, 415, 1, 53, 516, 13, 7, 0, 21};
	
	int *out_beg = NULL, *out_end = NULL;
	int ret_key = key(in, in + 9, &out_beg, &out_end);
	ck_assert_int_eq(ret_key, SUCCESS);
	ck_assert_int_eq(out_end - out_beg, 9);
	for (size_t i = 0; i < 9; i++)
		ck_assert_int_eq(out_beg[i], out_correct[i]);
	
	if (out_beg)
		free(out_beg);
}
END_TEST

START_TEST(test_key_invalid_params)
{
	int in[7] = {15, 7, 234, 0, -23, 2, 11};
	int *out_beg = NULL, *out_end = NULL;
	
	int ret_key = key(in, NULL, &out_beg, &out_end);
	ck_assert_int_eq(ret_key, POINTER_ERR);
}
END_TEST

START_TEST(test_key_new_arr_is_empty)
{
	int in[7] = {-15, 7, 234, 0, -23, 2, 11};
	int *out_beg = NULL, *out_end = NULL;
	
	int ret_key = key(in, in + 7, &out_beg, &out_end);
	ck_assert_int_eq(ret_key, NEW_ARR_IS_EMPTY);
}
END_TEST

Suite *key_suite(void)
{
	Suite *s;
	TCase *tc_pos;
	TCase *tc_neg;
	
	s = suite_create("key");
	
	tc_pos = tcase_create("positives");
	tcase_add_test(tc_pos, test_key_one_elem_out);
	tcase_add_test(tc_pos, test_key_random_array);
	tcase_add_test(tc_pos, test_key_no_neg_numbers);
	suite_add_tcase(s, tc_pos);
	
	tc_neg = tcase_create("negatives");
	tcase_add_test(tc_neg, test_key_invalid_params);
	tcase_add_test(tc_neg, test_key_new_arr_is_empty);
	suite_add_tcase(s, tc_neg);
	
	return s;
}