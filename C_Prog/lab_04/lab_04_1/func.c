#include <stdlib.h>
#include "func.h"

int char_is_in_str(const char ch, const char *string)
{
	int ret_val = 0;
	while (*string != '\0')
	{
		if (*string == ch)
			ret_val = 1;
		string++;
	}
	return ret_val;
}

size_t my_strspn(const char *string1, const char *string2)
{
	size_t i = 0;
	while (string1[i] != '\0' && char_is_in_str(string1[i], string2)) 
		i++;
	return i;
}