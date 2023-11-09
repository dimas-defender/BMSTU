#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "defines.h"

START_TEST(test_NULL_head)
{
	node_t *head_exp = NULL, *head_act = NULL;
	head_act = sort(head_act, compare_by_year);
	ck_assert_ptr_eq(head_act, head_exp);
	ck_assert_ptr_null(head_act);
}
END_TEST

START_TEST(test_one_elem_list)
{
	node_t *head_exp = NULL, *head_act = NULL;
	create_list(&head_exp, 1);
	create_list(&head_act, 1);
	town_t town = {"Moscow", 1147};
	fill_list(head_exp, &town);
	fill_list(head_act, &town);
	head_act = sort(head_act, compare_by_year);
	ck_assert_int_eq(equal_lists(head_exp, head_act), 1);
	free_list(head_exp);
	free_list(head_act);
}
END_TEST

START_TEST(test_standard)
{
	node_t *head_exp = NULL, *head_act = NULL;
	create_list(&head_exp, 4);
	create_list(&head_act, 4);
	town_t towns[4] = {
	{"New York", 1624},
	{"Moscow", 1147},
	{"SaintPB", 1703},
	{"Madrid", 852}
	};
	fill_list(head_act, towns);
	node_t *cur = head_exp;
	cur->data = towns + 3;
	cur = cur->next;
	cur->data = towns + 1;
	cur = cur->next;
	cur->data = towns;
	cur = cur->next;
	cur->data = towns + 2;
	head_act = sort(head_act, compare_by_year);
	ck_assert_int_eq(equal_lists(head_exp, head_act), 1);
	free_list(head_exp);
	free_list(head_act);
}
END_TEST

Suite *sort_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("sort");
	
	tc_all = tcase_create("sort");
	tcase_add_test(tc_all, test_NULL_head);
	tcase_add_test(tc_all, test_one_elem_list);
	tcase_add_test(tc_all, test_standard);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}