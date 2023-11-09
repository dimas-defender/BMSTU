#include <stdio.h>
#include <string.h>
#include "func.h"

int main(void)
{
	int ret_key = 0;
	char str[MAX_STR_LEN + 1];
	int str_len = read_line(str);
	if (!str_len)
	{
		printf("Input error");
		ret_key = 1;
	}
	else
	{
		int max = find_max_word_length(str);
		if (max > MAX_WORD_LEN || !max)
		{
			printf("Input error");
			ret_key = 1;
		}
		else
		{
			char matrix[MAX_STR_LEN / 2][MAX_WORD_LEN + 1];
			char *words[MAX_STR_LEN / 2];
			transform(words, *matrix, MAX_STR_LEN / 2, MAX_WORD_LEN + 1);
			int len = get_words(words, str);
			char new_str[MAX_STR_LEN + 1];
			new_str[0] = '\0';
			make_new_str(words, new_str, len);
			if (new_str[0] == '\0')
			{
				printf("There aren't two unique words in the string");
				ret_key = 1;				
			}
			else
				printf("Result: %s\n", new_str);
		}
	}
	return ret_key;
}