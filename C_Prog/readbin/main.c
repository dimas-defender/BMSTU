#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define NAME_LEN 30
#define MANUFACTURER_LEN 15

typedef struct
{
    char name[NAME_LEN + 1];
    char manufacturer[MANUFACTURER_LEN + 1];
    uint32_t price;
    uint32_t quantity;
} product_t;

void size_of_file(FILE *file, int *size);

int get_product_by_pos(FILE *file_in, size_t pos, product_t *product);

int main(int argc, char **argv)
{
    FILE *file;
    file = fopen(argv[argc - 1], "rb");
    product_t product = { 0 };
    memset(&product, 0, sizeof(product_t));
    int size;
    size_of_file(file, &size);
    size /= sizeof(product_t);
    for (int i = 0; i < size; i++)
    {
        get_product_by_pos(file, i, &product);
        printf("%s\n%s\n%d\n%d\n", product.name, product.manufacturer, product.price, product.quantity);
        memset(&product, 0, sizeof(product_t));
    }

}

void size_of_file(FILE *file, int *size)
{
    if (!fseek(file, 0, SEEK_END))
    {
        *size = ftell(file);
        if (*size > 0)
            if (fseek(file, 0, SEEK_SET))
                *size = -1;
    }
    else
        *size = -1;
}

int get_product_by_pos(FILE *file_in, size_t pos, product_t *product)
{
    int r = fseek(file_in, pos * sizeof(product_t), SEEK_SET) < 0;
    if (!r)
        r = fread(product, sizeof(product_t), 1, file_in) != 1;
    return r;
}
