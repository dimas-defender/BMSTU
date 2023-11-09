#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "defines.h"

START_TEST(test_zero_deg)
{
	node_t *head;
	create_list(&head, 1);
	head->coef = 5;
	int arg = 10;
	int expected_value = 5;
	int actual_value = get_polynom_value(head, 0, arg);
	ck_assert_int_eq(expected_value, actual_value);
	free_list(head);
}
END_TEST

START_TEST(test_zero_arg)
{
	node_t *head;
	create_list(&head, 3);
	node_t *cur = head;
	cur->coef = 1;
	cur = cur->next;
	cur->coef = -3;
	cur = cur->next;
	cur->coef = 16;
	int arg = 0;
	int expected_value = 16;
	int actual_value = get_polynom_value(head, 2, arg);
	ck_assert_int_eq(expected_value, actual_value);
	free_list(head);
}
END_TEST

START_TEST(test_normal)
{
	node_t *head;
	create_list(&head, 4);
	node_t *cur = head;
	cur->coef = 1;
	cur = cur->next;
	cur->coef = 2;
	cur = cur->next;
	cur->coef = -3;
	cur = cur->next;
	cur->coef = 4;
	int arg = 3;
	int expected_value = 40;
	int actual_value = get_polynom_value(head, 3, arg);
	ck_assert_int_eq(expected_value, actual_value);
	free_list(head);	
}
END_TEST

Suite *value_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("value");
	
	tc_all = tcase_create("val");
	tcase_add_test(tc_all, test_zero_deg);
	tcase_add_test(tc_all, test_zero_arg);
	tcase_add_test(tc_all, test_normal);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}