#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int main(void)
{
	char *string_1 = "this is a test string";
	char *string_2 = "abdcthil";

	if (strspn(string_1, string_2) == my_strspn(string_1, string_2))
		printf("Test1 was passed\n");
	else
		printf("Test1 was failed\n");
	
	string_1 = "bridge over the river";
	
	if (strspn(string_1, string_2) == my_strspn(string_1, string_2))
		printf("Test2 was passed\n");
	else
		printf("Test2 was failed\n");
	
	string_1 = "we could see the sunset";
	
	if (strspn(string_1, string_2) == my_strspn(string_1, string_2))
		printf("Test3 was passed\n");
	else
		printf("Test3 was failed\n");
	return 0;
}
