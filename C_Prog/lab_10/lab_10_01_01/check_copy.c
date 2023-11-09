#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "defines.h"

START_TEST(test_NULL_head)
{
	node_t *head = NULL, *new_head = NULL;
	int rc = copy(head, &new_head);
	ck_assert_int_eq(rc, PTR_ERR);
	ck_assert_ptr_eq(new_head, head);
	ck_assert_ptr_null(new_head);
}
END_TEST

START_TEST(test_standard)
{
	node_t *head = NULL, *new_head = NULL;
	create_list(&head, 4);
	town_t towns[4] = {
	{"New York", 1624},
	{"Moscow", 1147},
	{"SaintPB", 1703},
	{"Madrid", 852}
	};
	fill_list(head, towns);
	int rc = copy(head, &new_head);
	ck_assert_int_eq(rc, SUCCESS);
	ck_assert_int_eq(equal_lists(head, new_head), 1);
	free_list(head);
	free_list(new_head);
}
END_TEST

Suite *copy_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("copy");
	
	tc_all = tcase_create("copy");
	tcase_add_test(tc_all, test_NULL_head);
	tcase_add_test(tc_all, test_standard);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}