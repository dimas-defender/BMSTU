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

void remove_letters(char *str)
{
	int n = strlen(str);
	int k = 1, i, j;
	for (i = 0; i < n; i++)
	{
		if (str[k] == str[0])
		{
			for (j = k; j < n; j++)
				str[j] = str[j + 1];
			k--;
		}
		k++;
	}
}

int get_words(char **words, char *str)
{
	char const delim[] = " ,;:-.!?";
	char *word = NULL;
	word = strtok(str, delim);
	int i = 0;
	while (word != NULL)
	{
		strcpy(words[i++], word);
		word = strtok(NULL, delim);
	}
	return i;
}

void make_new_str(char **words, char *new_str, int len)
{
	int k = 0;
	for (int i = len - 2; i > -1; i--)
		if (strcmp(words[i], words[len - 1]))
		{
			remove_letters(words[i]);
			if (k)
				strcat(new_str, " ");
			strcat(new_str, words[i]);
			k++;
		}
}