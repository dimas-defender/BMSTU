#include <stdlib.h>
#include <check.h>
#include "suites.h"

int main(void)
{
	int no_failed = 0;
	Suite *s[3] = {make_matrix_square_suite(), expand_square_matrix_suite(), calculate_matrix_expression_suite()};
	for (size_t i = 0; i < 3; i++)
	{
		SRunner *runner = srunner_create(s[i]);
		srunner_run_all(runner, CK_NORMAL);
		if (srunner_ntests_failed(runner))
			no_failed++;
		srunner_free(runner);
	}
	
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}