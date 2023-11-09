#ifndef _FUNC_H_
#define _FUNC_H_

#define MAX_STR_LEN 256
#define MAX_WORD_LEN 16

int read_line(char *str);
int ch_is_separator(char ch);
int find_max_word_length(char *str);
void transform(char **a, char *matrix, int n, int m);
void remove_letters(char *str);
int get_words(char **words, char *str);
void make_new_str(char **words, char *new_str, int len);

#endif