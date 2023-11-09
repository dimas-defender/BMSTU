#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "defines.h"

START_TEST(test_zero_degree)
{
	node_t *head, *exp_ddx, *act_ddx;
	create_list(&head, 1);
	head->coef = 7;
	create_list(&exp_ddx, 1);
	exp_ddx->coef = 0;
	create_list(&act_ddx, 1);
	get_derivative(head, act_ddx, 0);
	ck_assert_int_eq(equal_lists(exp_ddx, act_ddx), 1);
	free_list(head);
	free_list(exp_ddx);
	free_list(act_ddx);
}
END_TEST

START_TEST(test_first_degree)
{
	node_t *head, *exp_ddx, *act_ddx;
	create_list(&head, 2);
	node_t *cur = head;
	cur->coef = 17;
	cur = cur->next;
	cur->coef = -32;
	create_list(&exp_ddx, 1);
	exp_ddx->coef = 17;
	create_list(&act_ddx, 1);
	get_derivative(head, act_ddx, 1);
	ck_assert_int_eq(equal_lists(exp_ddx, act_ddx), 1);
	free_list(head);
	free_list(exp_ddx);
	free_list(act_ddx);
}
END_TEST

START_TEST(test_standard)
{
	node_t *head, *exp_ddx, *act_ddx;
	create_list(&head, 5);
	node_t *cur = head;
	cur->coef = 2;
	cur = cur->next;
	cur->coef = -3;
	cur = cur->next;
	cur->coef = 15;
	cur = cur->next;
	cur->coef = -7;
	cur = cur->next;
	cur->coef = 8;
	create_list(&exp_ddx, 4);
	cur = exp_ddx;
	cur->coef = 8;
	cur = cur->next;
	cur->coef = -9;
	cur = cur->next;
	cur->coef = 30;
	cur = cur->next;
	cur->coef = -7;
	create_list(&act_ddx, 4);
	get_derivative(head, act_ddx, 4);
	ck_assert_int_eq(equal_lists(exp_ddx, act_ddx), 1);
	free_list(head);
	free_list(exp_ddx);
	free_list(act_ddx);
}
END_TEST

Suite *deriv_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("deriv");
	
	tc_all = tcase_create("ddx");
	tcase_add_test(tc_all, test_zero_degree);
	tcase_add_test(tc_all, test_first_degree);
	tcase_add_test(tc_all, test_standard);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}