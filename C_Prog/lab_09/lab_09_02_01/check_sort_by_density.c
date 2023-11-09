#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "defines.h"

START_TEST(test_sort_reversed_array)
{
	item_t in[3] = {
	{"a", 5, 0.1},
	{"b", 1, 1},
	{"c", 0.5, 10}
	};
	item_t out[3] = {
	{"c", 0.5, 10},
	{"b", 1, 1},
	{"a", 5, 0.1}
	};
	sort_by_density(in, 3);
	for (size_t i = 0; i < 3; i++)
	{
		ck_assert_str_eq(in[i].name, out[i].name);
		ck_assert_float_eq(in[i].weight, out[i].weight);
		ck_assert_float_eq(in[i].size, out[i].size);
	}
}
END_TEST

START_TEST(test_sort_sorted_array)
{
	item_t in[3] = {
	{"a", 5, 10},
	{"b", 1.2, 0.6},
	{"c", 10, 2.5}
	};
	item_t out[3] = {
	{"a", 5, 10},
	{"b", 1.2, 0.6},
	{"c", 10, 2.5}
	};
	sort_by_density(in, 3);
	for (size_t i = 0; i < 3; i++)
	{
		ck_assert_str_eq(in[i].name, out[i].name);
		ck_assert_float_eq(in[i].weight, out[i].weight);
		ck_assert_float_eq(in[i].size, out[i].size);
	}
}
END_TEST

START_TEST(test_sort_random_array)
{
	item_t in[4] = {
	{"a", 5, 16},
	{"b", 1.2, 27.6},
	{"c", 0.1, 53.8},
	{"d", 12.06, 2.53}
	};
	item_t out[4] = {
	{"c", 0.1, 53.8},
	{"b", 1.2, 27.6},
	{"a", 5, 16},
	{"d", 12.06, 2.53}
	};
	sort_by_density(in, 4);
	for (size_t i = 0; i < 4; i++)
	{
		ck_assert_str_eq(in[i].name, out[i].name);
		ck_assert_float_eq(in[i].weight, out[i].weight);
		ck_assert_float_eq(in[i].size, out[i].size);
	} 
}
END_TEST

START_TEST(test_sort_one_elem_array)
{
	item_t in[1] = {{"a", 5, 0.1}};
	item_t out[1] = {{"a", 5, 0.1}};
	sort_by_density(in, 1);
	ck_assert_str_eq(in[0].name, out[0].name);
	ck_assert_float_eq(in[0].weight, out[0].weight);
	ck_assert_float_eq(in[0].size, out[0].size);
}
END_TEST

Suite *sort_suite(void)
{
	Suite *s;
	TCase *tc_all;
	
	s = suite_create("sort_by_density");
	
	tc_all = tcase_create("sort_by_density");
	tcase_add_test(tc_all, test_sort_reversed_array);
	tcase_add_test(tc_all, test_sort_sorted_array);
	tcase_add_test(tc_all, test_sort_random_array);
	tcase_add_test(tc_all, test_sort_one_elem_array);
	
	suite_add_tcase(s, tc_all);
	
	return s;
}