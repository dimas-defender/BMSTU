#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "process.h"

START_TEST(test_mysort_reversed_array)
{
	int in[] = {27, 16, 8, 8, -10};
	int out[] = {-10, 8, 8, 16, 27};
	mysort(in, 5, sizeof(int), compare_int);
	for (size_t i = 0; i < 5; i++)
		ck_assert_int_eq(in[i], out[i]);    
}
END_TEST

START_TEST(test_mysort_sorted_array)
{
	int in[] = {-251, -52, 13, 32, 641, 996};
	int out[] = {-251, -52, 13, 32, 641, 996};
	mysort(in, 6, sizeof(int), compare_int);
	for (size_t i = 0; i < 6; i++)
		ck_assert_int_eq(in[i], out[i]);    
}
END_TEST

START_TEST(test_mysort_random_array)
{
	int in[] = {-23, 8, -12, 35, 68, -41, 124, 0, 26};
	int out[] = {-41, -23, -12, 0, 8, 26, 35, 68, 124};
	mysort(in, 9, sizeof(int), compare_int);
	for (size_t i = 0; i < 9; i++)
		ck_assert_int_eq(in[i], out[i]);    
}
END_TEST

START_TEST(test_mysort_one_elem_array)
{
	int in[] = {15};
	int out[] = {15};
	mysort(in, 1, sizeof(int), compare_int);
	ck_assert_int_eq(in[0], out[0]);    
}
END_TEST

Suite *mysort_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("mysort");
	
	tc_all = tcase_create("mysort");
	tcase_add_test(tc_all, test_mysort_reversed_array);
	tcase_add_test(tc_all, test_mysort_sorted_array);
	tcase_add_test(tc_all, test_mysort_random_array);
	tcase_add_test(tc_all, test_mysort_one_elem_array);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}