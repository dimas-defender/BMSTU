#include <stdio.h>
#include "func.h"

int main(void)
{
	int max_length = 0;
	int ret_key = process(stdin, &max_length);
	if (ret_key)
		printf("Error");
	else
		printf("Max length is: %d", max_length);
	return ret_key;
}