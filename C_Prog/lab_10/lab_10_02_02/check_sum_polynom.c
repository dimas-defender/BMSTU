#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "defines.h"

START_TEST(test_same_deg)
{
	node_t *head1, *head2, *exp_sum, *act_sum;
	create_list(&head1, 3);
	node_t *cur = head1;
	cur->coef = 32;
	cur = cur->next;
	cur->coef = 5;
	cur = cur->next;
	cur->coef = -23;
	create_list(&head2, 3);
	cur = head2;
	cur->coef = -11;
	cur = cur->next;
	cur->coef = 0;
	cur = cur->next;
	cur->coef = 7;
	create_list(&exp_sum, 3);
	cur = exp_sum;
	cur->coef = 21;
	cur = cur->next;
	cur->coef = 5;
	cur = cur->next;
	cur->coef = -16;
	create_list(&act_sum, 3);
	sum_polynom(head1, head2, act_sum, 0);
	ck_assert_int_eq(equal_lists(exp_sum, act_sum), 1);
	free_list(head1);
	free_list(head2);
	free_list(exp_sum);
	free_list(act_sum);
}
END_TEST

START_TEST(test_diff_deg)
{
	node_t *head1, *head2, *exp_sum, *act_sum;
	create_list(&head1, 5);
	node_t *cur = head1;
	cur->coef = -20;
	cur = cur->next;
	cur->coef = 12;
	cur = cur->next;
	cur->coef = -8;
	cur = cur->next;
	cur->coef = 0;
	cur = cur->next;
	cur->coef = 51;
	create_list(&head2, 3);
	cur = head2;
	cur->coef = 25;
	cur = cur->next;
	cur->coef = -12;
	cur = cur->next;
	cur->coef = 10;
	create_list(&exp_sum, 5);
	cur = exp_sum;
	cur->coef = -20;
	cur = cur->next;
	cur->coef = 12;
	cur = cur->next;
	cur->coef = 17;
	cur = cur->next;
	cur->coef = -12;
	cur = cur->next;
	cur->coef = 61;
	create_list(&act_sum, 5);
	sum_polynom(head1, head2, act_sum, 2);
	ck_assert_int_eq(equal_lists(exp_sum, act_sum), 1);
	free_list(head1);
	free_list(head2);
	free_list(exp_sum);
	free_list(act_sum);
}
END_TEST

Suite *sum_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("sum");
	
	tc_all = tcase_create("sum");
	tcase_add_test(tc_all, test_same_deg);
	tcase_add_test(tc_all, test_diff_deg);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}