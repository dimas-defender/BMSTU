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
    strcpy(product.name, "pten");
    strcpy(product.manufacturer, "qtwenty");
    product.price = 5;
    product.quantity = 13;
    put_product_by_pos(file, 0, &product);

    memset(&product, 0, sizeof(product_t));
    strcpy(product.name, "pten");
    strcpy(product.manufacturer, "qsix");
    product.price = 7;
    product.quantity = 20;
    put_product_by_pos(file, 1, &product);

    memset(&product, 0, sizeof(product_t));
    strcpy(product.name, "ptenf");
    strcpy(product.manufacturer, "qninef");
    product.price = 15;
    product.quantity = 26;
    put_product_by_pos(file, 2, &product);

    memset(&product, 0, sizeof(product_t));
    strcpy(product.name, "ptens");
    strcpy(product.manufacturer, "qnines");
    product.price = 3;
    product.quantity = 9;
    put_product_by_pos(file, 3, &product);

    memset(&product, 0, sizeof(product_t));
    strcpy(product.name, "pzerof");
    strcpy(product.manufacturer, "qzerof");
    product.price = 0;
    product.quantity = 0;
    put_product_by_pos(file, 4, &product);

    memset(&product, 0, sizeof(product_t));
    strcpy(product.name, "pzeros");
    strcpy(product.manufacturer, "qzeros");
    product.price = 1;
    product.quantity = 11;
    put_product_by_pos(file, 5, &product);

    memset(&product, 0, sizeof(product_t));
    strcpy(product.name, "ptent");
    strcpy(product.manufacturer, "qninet");
    product.price = 34;
    product.quantity = 4;
    put_product_by_pos(file, 6, &product);

    memset(&product, 0, sizeof(product_t));
    strcpy(product.name, "ptenc");
    strcpy(product.manufacturer, "qninec");
    product.price = 1;
    product.quantity = 29;
    put_product_by_pos(file, 7, &product);

    memset(&product, 0, sizeof(product_t));
    strcpy(product.name, "pten");
    strcpy(product.manufacturer, "qeight");
    product.price = 42;
    product.quantity = 26;
    put_product_by_pos(file, 8, &product);

    memset(&product, 0, sizeof(product_t));
    strcpy(product.name, "pnine");
    strcpy(product.manufacturer, "qtwenty");
    product.price = 21;
    product.quantity = 7;
    put_product_by_pos(file, 9, &product);
}

int put_product_by_pos(FILE *file_out, size_t pos, product_t *product)
{
    int r = fseek(file_out, pos * sizeof(product_t), SEEK_SET) < 0;
    if (!r)
        r = fwrite(product, sizeof(product_t), 1, file_out) != 1;
    return r;
}