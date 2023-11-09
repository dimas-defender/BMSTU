#include <stdlib.h>
#include <check.h>

Suite *snprintf_suite(void);

int main(void)
{
	int no_failed = 0;
	SRunner *runner = srunner_create(snprintf_suite());
	srunner_run_all(runner, CK_NORMAL);
	if (srunner_ntests_failed(runner))
		no_failed++;
	srunner_free(runner);
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
