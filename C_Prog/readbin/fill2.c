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

int put_product_by_pos(FILE *file_out, size_t pos, product_t *product);

int main()
{
    FILE *file;
    file = fopen("file_in.bin", "wb");
    product_t product = { 0 };

    memset(&product, 0, sizeof(product_t));
    strcpy(product.name, "TV");
    strcpy(product.manufacturer, "TMN Industr.");
    product.price = 28;
    product.quantity = 15;
    put_product_by_pos(file, 0, &product);
}

int put_product_by_pos(FILE *file_out, size_t pos, product_t *product)
{
    int r = fseek(file_out, pos * sizeof(product_t), SEEK_SET) < 0;
    if (!r)
        r = fwrite(product, sizeof(product_t), 1, file_out) != 1;
    return r;
}