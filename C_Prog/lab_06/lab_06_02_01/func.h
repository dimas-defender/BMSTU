#ifndef _FUNC_H_
#define _FUNC_H_

#define SUCCESS 0
#define WRONG_LAUNCH 53
#define WRONG_FILE -1
#define NO_FILE -2
#define READ_ERR -3
#define DATA_ERR -4
#define WRONG_SUBSTR_LEN -5

#define FIELDS_PER_RECORD 3
#define MAX_AMOUNT 15
#define MAX_NAME_LEN 25

typedef struct
{
    char name[MAX_NAME_LEN + 1];
    float weight;
    float size;
} item_t;

int read_file(char *str, item_t *items, int *quantity);
int input_item(FILE *f, item_t *items);
void swap(item_t *a, item_t *b);
void sort_by_density(item_t *items, int len);
int filter_items(item_t *items, int len, char *substr);
void print_items(item_t *items, int len);
int starts_with(char *str, char *substr);

#endif