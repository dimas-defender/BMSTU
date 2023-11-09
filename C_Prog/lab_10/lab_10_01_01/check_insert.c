#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "defines.h"

START_TEST(test_NULL_head)
{
	node_t *head_exp = NULL, *head_act = NULL;
	node_t *elem = create_node();
	insert(&head_act, elem, head_act);
	ck_assert_ptr_eq(head_exp, head_act);
	ck_assert_ptr_null(head_act);
	free(elem);
}
END_TEST

START_TEST(test_wrong_insert_place)
{
	node_t *head_exp = NULL, *head_act = NULL;
	create_list(&head_exp, 3);
	create_list(&head_act, 3);
	town_t towns[5] = {
	{"New York", 1624},
	{"Moscow", 1147},
	{"SaintPB", 1703},
	{"Madrid", 852},
	{"Berlin", 1237}
	};
	fill_list(head_exp, towns);
	fill_list(head_act, towns);
	node_t *elem = create_node();
	elem->data = towns + 3;
	node_t *before = create_node();
	before->data = towns + 4;
	insert(&head_act, elem, before);
	ck_assert_int_eq(equal_lists(head_exp, head_act), 1);
	free(elem);
	free(before);
	free_list(head_exp);
	free_list(head_act);
}
END_TEST

START_TEST(test_insert_first)
{
	node_t *head_exp = NULL, *head_act = NULL;
	create_list(&head_exp, 4);
	create_list(&head_act, 3);
	town_t towns[4] = {
	{"New York", 1624},
	{"Moscow", 1147},
	{"SaintPB", 1703},
	{"Madrid", 852}
	};
	fill_list(head_exp, towns);
	fill_list(head_act, towns + 1);
	node_t *elem = create_node();
	elem->data = towns;
	insert(&head_act, elem, head_act);
	ck_assert_int_eq(equal_lists(head_exp, head_act), 1);
	free_list(head_exp);
	free_list(head_act);
}
END_TEST

START_TEST(test_insert_in)
{
	node_t *head_exp = NULL, *head_act = NULL;
	create_list(&head_exp, 4);
	create_list(&head_act, 3);
	town_t towns[4] = {
	{"New York", 1624},
	{"Moscow", 1147},
	{"SaintPB", 1703},
	{"Madrid", 852}
	};
	fill_list(head_exp, towns);
	node_t *cur = head_act;
	cur->data = towns;
	cur = cur->next;
	cur->data = towns + 1;
	cur = cur->next;
	cur->data = towns + 3;
	node_t *elem = create_node();
	elem->data = towns + 2;
	insert(&head_act, elem, cur);
	ck_assert_int_eq(equal_lists(head_exp, head_act), 1);
	free_list(head_exp);
	free_list(head_act);
}
END_TEST

Suite *insert_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("insert");
	
	tc_all = tcase_create("insert");
	tcase_add_test(tc_all, test_NULL_head);
	tcase_add_test(tc_all, test_wrong_insert_place);
	tcase_add_test(tc_all, test_insert_first);
	tcase_add_test(tc_all, test_insert_in);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}