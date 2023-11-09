#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "defines.h"

START_TEST(test_no_suitable_records)
{
	item_t in[3] = {
	{"apple", 5, 0.1},
	{"orange", 1, 1},
	{"pear", 0.5, 10}
	};
	int correct_q = 0;
	int quantity = filter_items(in, 3, "b");
	ck_assert_int_eq(correct_q, quantity);
}
END_TEST

START_TEST(test_all_suitable_records)
{
	item_t in[3] = {
	{"big_apple", 5, 0.1},
	{"big_orange", 1, 1},
	{"big_pear", 0.5, 10}
	};
	int correct_q = 3;
	int quantity = filter_items(in, 3, "b");
	ck_assert_int_eq(correct_q, quantity);
	for (int i = 0; i < quantity; i++)
		ck_assert_int_ne(starts_with(in[i].name, "b"), 0);      
}
END_TEST

START_TEST(test_some_suitable_records)
{
	item_t in[3] = {
	{"red_apple", 5, 0.1},
	{"big_orange", 1, 1},
	{"green_pear", 0.5, 10}
	};
	int correct_q = 1;
	int quantity = filter_items(in, 3, "gr");
	ck_assert_int_eq(correct_q, quantity);
	ck_assert_int_ne(starts_with(in[0].name, "gr"), 0);    
}
END_TEST

Suite *filter_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("filter_items");
	
	tc_all = tcase_create("filter_items");
	tcase_add_test(tc_all, test_no_suitable_records);
	tcase_add_test(tc_all, test_all_suitable_records);
	tcase_add_test(tc_all, test_some_suitable_records);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}