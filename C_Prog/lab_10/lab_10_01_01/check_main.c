#include <stdlib.h>
#include <check.h>
#include "suites.h"

int main(void)
{
	int no_failed = 0;
	Suite *s[3] = {insert_suite(), copy_suite(), sort_suite()};
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