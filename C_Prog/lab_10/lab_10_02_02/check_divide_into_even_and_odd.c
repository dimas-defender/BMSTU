#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "defines.h"

START_TEST(test_even_deg)
{
	node_t *head, *exp_even, *exp_odd, *act_even, *act_odd;
	create_list(&head, 5);
	node_t *cur = head;
	cur->coef = 7;
	cur = cur->next;
	cur->coef = 15;
	cur = cur->next;
	cur->coef = -2;
	cur = cur->next;
	cur->coef = 0;
	cur = cur->next;
	cur->coef = 8;
	create_list(&exp_even, 3);
	cur = exp_even;
	cur->coef = 7;
	cur = cur->next;
	cur->coef = -2;
	cur = cur->next;
	cur->coef = 8;
	create_list(&exp_odd, 2);
	cur = exp_odd;
	cur->coef = 15;
	cur = cur->next;
	cur->coef = 0;
	create_list(&act_even, 3);
	create_list(&act_odd, 2);
	divide_into_even_and_odd(head, act_even, act_odd, 4);
	ck_assert_int_eq(equal_lists(exp_even, act_even), 1);
	ck_assert_int_eq(equal_lists(exp_odd, act_odd), 1);
	free_list(head);
	free_list(exp_even);
	free_list(act_even);
	free_list(exp_odd);
	free_list(act_odd);
}
END_TEST

START_TEST(test_odd_deg)
{
	node_t *head, *exp_even, *exp_odd, *act_even, *act_odd;
	create_list(&head, 4);
	node_t *cur = head;
	cur->coef = -28;
	cur = cur->next;
	cur->coef = 6;
	cur = cur->next;
	cur->coef = 1;
	cur = cur->next;
	cur->coef = 12;
	create_list(&exp_even, 2);
	cur = exp_even;
	cur->coef = 6;
	cur = cur->next;
	cur->coef = 12;
	create_list(&exp_odd, 2);
	cur = exp_odd;
	cur->coef = -28;
	cur = cur->next;
	cur->coef = 1;
	create_list(&act_even, 2);
	create_list(&act_odd, 2);
	divide_into_even_and_odd(head, act_even, act_odd, 3);
	ck_assert_int_eq(equal_lists(exp_even, act_even), 1);
	ck_assert_int_eq(equal_lists(exp_odd, act_odd), 1);
	free_list(head);
	free_list(exp_even);
	free_list(act_even);
	free_list(exp_odd);
	free_list(act_odd);
}
END_TEST

START_TEST(test_zero_deg)
{
	node_t *head, *exp_even, *exp_odd = NULL, *act_even, *act_odd = NULL;
	create_list(&head, 1);
	head->coef = 55;
	create_list(&exp_even, 1);
	exp_even->coef = 55;
	create_list(&act_even, 1);
	divide_into_even_and_odd(head, act_even, act_odd, 0);
	ck_assert_int_eq(equal_lists(exp_even, act_even), 1);
	ck_assert_int_eq(equal_lists(exp_odd, act_odd), 1);
	free_list(head);
	free_list(exp_even);
	free_list(act_even);
}
END_TEST

Suite *divide_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("divide");
	
	tc_all = tcase_create("dvd");
	tcase_add_test(tc_all, test_even_deg);
	tcase_add_test(tc_all, test_odd_deg);
	tcase_add_test(tc_all, test_zero_deg);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}