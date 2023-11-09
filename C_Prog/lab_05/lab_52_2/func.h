#ifndef _FUNC_H_
#define _FUNC_H_

#define WRONG_LAUNCH 53
#define WRONG_INPUT -1
#define NO_FILE -2
#define NO_DATA -3
#define SEEK_ERR -4
#define READ_ERR -5
#define WRITE_ERR -6
#define NO_PRODUCTS_FOUND -7

#define MAX_NAME_LEN 30
#define MAX_MANUFACTURER_LEN 15

typedef struct
{
    char name[MAX_NAME_LEN + 1];
    char manufacturer[MAX_MANUFACTURER_LEN + 1];
    uint32_t price;
    uint32_t quantity;
} product_t;

int file_size(FILE *f, size_t *size);
int get_product_by_pos(FILE *f, size_t pos, product_t *product);
int put_product_by_pos(FILE *f, size_t pos, product_t *product);
int copy_bin_file(FILE *fin, FILE *fout);
int sort_bin_file(FILE *f);
int ends_with(char *str, char *substr);
int print_products(FILE *f, char *substr);
int read_line(FILE *f, char *str, int len);
int input_product(FILE *f, product_t *product);
int add_product(FILE *f, product_t *product, size_t size);

#endif