#ifndef _FUNC_H_
#define _FUNC_H_

#define WRONG_LAUNCH -1
#define NO_FILE -2
#define NO_DATA -3
#define SEEK_ERR -4
#define READ_ERR -5
#define WRITE_ERR -6

#define MAX_RAND 500
#define MIN_RAND -500

int file_size(FILE *f, size_t *size);
int print_bin_file(FILE *f);
int create_bin_file(FILE *f, int amount);
int get_number_by_pos(FILE *file, size_t pos, int *num);
int put_number_by_pos(FILE *file, size_t pos, int *num);
int sort_bin_file(FILE *file);

#endif