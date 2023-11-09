#include <stdlib.h>
#include <check.h>
#include "suites.h"

int main(void)
{
	int no_failed = 0;
	Suite *s[2] = {sort_suite(), filter_suite()};
	for (size_t i = 0; i < 2; i++)
	{
		SRunner *runner = srunner_create(s[i]);
		srunner_run_all(runner, CK_NORMAL);
		if (srunner_ntests_failed(runner))
			no_failed++;
		srunner_free(runner);
	}
	
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}