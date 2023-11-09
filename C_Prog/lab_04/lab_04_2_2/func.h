#ifndef _FUNC_H_
#define _FUNC_H_

#define MAX_STR_LEN 256
#define MAX_WORD_LEN 16

int read_line(char *str);
int ch_is_separator(char ch);
int find_max_word_length(char *str);
void transform(char **a, char *matrix, int n, int m);
void divide_str_into_words(char **words, char *str, int len);
int get_words_amount(char **words);
int word_is_in_array(char *str, char **words, int len);
void output(char **words1, char **words2, int n1, int n2);

#endif