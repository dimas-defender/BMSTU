#include <stdio.h>
#include <string.h>
#include "func.h"

int read_line(char *str)
{
	int ch, i = 0;
	printf("Input string: ");
	while ((ch = getchar()) != '\n' && ch != EOF && i <= MAX_STR_LEN)
		str[i++] = ch;
	if (i <= MAX_STR_LEN)
		str[i] = '\0';
	else
		i = 0;
	return i;
}

int ch_is_separator(char ch)
{
	int ret_key = 0;
	char separators[] = " ,;:-.!?";
	char *p = separators;
	while (*p)
	{
		if (ch == *p)
			ret_key = 1;
		p++;
	}
	return ret_key;
}

int find_max_word_length(char *str)
{
	char *p = str;
	int max_word_length = 0, cnt = 0;
	while (*p)
	{
		if (ch_is_separator(*p))
		{
			if (cnt > max_word_length)
				max_word_length = cnt;
			cnt = 0;
		}
		else
			cnt++;
		p++;
	}
	if (cnt > max_word_length)
		max_word_length = cnt;
	return max_word_length;
}

void transform(char **a, char *matrix, int n, int m)
{
	for (int i = 0; i < n; i++)
		a[i] = matrix + m * i;
}

void divide_str_into_words(char **words, char *str, int len)
{
	int k = 0;
	for (int i = 0; i < len; i++)
	{
		while (ch_is_separator(str[k]))
			k++;
		int j = 0;
		while (!ch_is_separator(str[k]) && str[k] != '\0')
			words[i][j++] = str[k++];
		while (j <= MAX_WORD_LEN)
			words[i][j++] = '\0';
	}
}

int get_words_amount(char **words)
{
	int i = 0;
	while (words[i][0])
		i++;
	return i;
}

int word_is_in_array(char *str, char **words, int len)
{
	int ret_key = 0;
	for (int i = 0; i < len; i++)
		if (!strcmp(str, words[i]))
			if (str[0] != '\0')
				ret_key = 1;
	return ret_key;
}

void output(char **words1, char **words2, int n1, int n2)
{
	char matrix[MAX_STR_LEN / 2][MAX_WORD_LEN + 1] = { 0 };
	char *printed_words[MAX_STR_LEN / 2] = { 0 };
	transform(printed_words, *matrix, MAX_STR_LEN / 2, MAX_WORD_LEN + 1);
	int n = 0;
	printf("Result:\n");
	for (int i = 0; i < n1; i++)
	{
		if (!word_is_in_array(words1[i], printed_words, n))
		{
			strcat(printed_words[n++], words1[i]);
			printf("%s %s\n", words1[i], word_is_in_array(words1[i], words2, n2) ? "yes" : "no");
		}
	}
}