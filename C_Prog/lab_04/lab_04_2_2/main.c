#include <stdio.h>
#include "func.h"

int main(void)
{
	int ret_key = 0;
	char str1[MAX_STR_LEN + 1];
	char str2[MAX_STR_LEN + 1];
	int str1_len = read_line(str1), str2_len = read_line(str2);
	if (!(str1_len && str2_len))
	{
		printf("Input error");
		ret_key = 1;
	}
	else
	{
		int max1 = find_max_word_length(str1), max2 = find_max_word_length(str2);
		if (max1 > MAX_WORD_LEN || !max1 || max2 > MAX_WORD_LEN || !max2)
		{
			printf("Input error");
			ret_key = 1;
		}
		else
		{
			char matrix1[MAX_STR_LEN / 2][MAX_WORD_LEN + 1];
			char *words1[MAX_STR_LEN / 2];
			char matrix2[MAX_STR_LEN / 2][MAX_WORD_LEN + 1];
			char *words2[MAX_STR_LEN / 2];
			transform(words1, *matrix1, MAX_STR_LEN / 2, MAX_WORD_LEN + 1);
			transform(words2, *matrix2, MAX_STR_LEN / 2, MAX_WORD_LEN + 1);
			divide_str_into_words(words1, str1, MAX_STR_LEN / 2);
			divide_str_into_words(words2, str2, MAX_STR_LEN / 2);
			int n1 = get_words_amount(words1), n2 = get_words_amount(words2);
			output(words1, words2, n1, n2);
		}
	}
	return ret_key;
}