#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "my_snprintf.h"

START_TEST(test_std_d)
{
	char str_exp[20];
	char str_act[20];
	int exp_val = snprintf(str_exp, 20, "%d + %d = %d", 32, -32, 0);
	int act_val = my_snprintf(str_act, 20, "%d + %d = %d", 32, -32, 0);
	ck_assert_int_eq(exp_val, act_val);
	ck_assert_str_eq(str_exp, str_act);
	ck_assert_str_eq("32 + -32 = 0", str_act);
}
END_TEST

START_TEST(test_max_d)
{
	char str_exp[20];
	char str_act[20];
	int exp_val = snprintf(str_exp, 20, "%d", INT_MAX);
	int act_val = my_snprintf(str_act, 20, "%d", INT_MAX);
	ck_assert_int_eq(exp_val, act_val);
	ck_assert_str_eq(str_exp, str_act);
	ck_assert_str_eq("2147483647", str_act);
}
END_TEST

START_TEST(test_min_d)
{
	char str_exp[20];
	char str_act[20];
	int exp_val = snprintf(str_exp, 20, "%d", INT_MIN);
	int act_val = my_snprintf(str_act, 20, "%d", INT_MIN);
	ck_assert_int_eq(exp_val, act_val);
	ck_assert_str_eq(str_exp, str_act);
	ck_assert_str_eq("-2147483648", str_act);
}
END_TEST

START_TEST(test_std_hx)
{
	char str_exp[20];
	char str_act[20];
	int exp_val = snprintf(str_exp, 20, "%hx - %hx != %hx", 31, 0, 209);
	int act_val = my_snprintf(str_act, 20, "%hx - %hx != %hx", 31, 0, 209);
	ck_assert_int_eq(exp_val, act_val);
	ck_assert_str_eq(str_exp, str_act);
	ck_assert_str_eq("1f - 0 != d1", str_act);
}
END_TEST

START_TEST(test_std_c)
{
	char str_exp[20];
	char str_act[20];
	int exp_val = snprintf(str_exp, 20, "%c %c %c three chars", 'a', 'b', 'c');
	int act_val = my_snprintf(str_act, 20, "%c %c %c three chars", 'a', 'b', 'c');
	ck_assert_int_eq(exp_val, act_val);
	ck_assert_str_eq(str_exp, str_act);
	ck_assert_str_eq("a b c three chars", str_act);
}
END_TEST

/*START_TEST(test_not_enough_space)
{
	char str_exp[10];
	char str_act[10];
	int exp_val = snprintf(str_exp, 10, "%hx and %d are the same numbers", 31, 31);
	int act_val = my_snprintf(str_act, 10, "%hx and %d are the same numbers", 31, 31);
	ck_assert_int_eq(exp_val, act_val);
	ck_assert_str_eq(str_exp, str_act);
}
END_TEST*/

START_TEST(test_null_str_zero_n)
{
	int exp_val = snprintf(NULL, 0, "%hx %d %c", 310, -24, 'q');
	int act_val = my_snprintf(NULL, 0, "%hx %d %c", 310, -24, 'q');
	ck_assert_int_eq(exp_val, act_val);
}
END_TEST

/*START_TEST(test_null_str)
{
	int exp_val = snprintf(NULL, 10, "%hx %d %c", 310, -24, 'q');
	int act_val = my_snprintf(NULL, 10, "%hx %d %c", 310, -24, 'q');
	ck_assert_int_eq(exp_val, act_val);
}
END_TEST*/

START_TEST(test_no_spec)
{
	char str_exp[20];
	char str_act[20];
	int exp_val = snprintf(str_exp, 20, "Happy New Year");
	int act_val = my_snprintf(str_act, 20, "Happy New Year");
	ck_assert_int_eq(exp_val, act_val);
	ck_assert_str_eq(str_exp, str_act);
	ck_assert_str_eq("Happy New Year", str_act);
}
END_TEST

Suite *snprintf_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("snprintf");
	
	tc_all = tcase_create("snprintf");
	tcase_add_test(tc_all, test_std_d);
	tcase_add_test(tc_all, test_max_d);
	tcase_add_test(tc_all, test_min_d);
	tcase_add_test(tc_all, test_std_hx);
	tcase_add_test(tc_all, test_std_c);
	//tcase_add_test(tc_all, test_not_enough_space);
	tcase_add_test(tc_all, test_null_str_zero_n);
	//tcase_add_test(tc_all, test_null_str);
	tcase_add_test(tc_all, test_no_spec);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}