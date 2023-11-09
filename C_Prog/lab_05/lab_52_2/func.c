#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "func.h"

int file_size(FILE *f, size_t *size)
{
	int ret_key = 0;
	long sz;
	if (fseek(f, 0L, SEEK_END))
		ret_key = SEEK_ERR;
	else
	{
		sz = ftell(f);
		if (sz < 0)
			ret_key = SEEK_ERR;
		else
			*size = sz;
	}
	return ret_key || fseek(f, 0L, SEEK_SET);
}

int get_product_by_pos(FILE *f, size_t pos, product_t *product)
{
	int ret_key = 0;
	if (!fseek(f, pos * sizeof(product_t), SEEK_SET))
	{
		if (fread(product, sizeof(product_t), 1, f) != 1)
			ret_key = READ_ERR;
	}
	else
		ret_key = SEEK_ERR;
	return ret_key;
}

int put_product_by_pos(FILE *f, size_t pos, product_t *product)
{
	int ret_key = 0;
	if (!fseek(f, pos * sizeof(product_t), SEEK_SET))
	{
		if (fwrite(product, sizeof(product_t), 1, f) != 1)
			ret_key = WRITE_ERR;
	}
	else
		ret_key = SEEK_ERR;
	return ret_key;
}

int sort_bin_file(FILE *f)
{
	size_t i, j;
	size_t size;
	int rc = file_size(f, &size) || size <= 0 || size % sizeof(product_t);
	if (rc == 0)
	{
		size /= sizeof(product_t);
		product_t product1 = { 0 }, product2 = { 0 };
		memset(&product1, 0, sizeof(product_t));
		memset(&product2, 0, sizeof(product_t));
		for (i = 0; !rc && i < size - 1; i++)
			for (j = 0; !rc && j < size - i - 1; j++)
			{
				rc = get_product_by_pos(f, j, &product1) || get_product_by_pos(f, j + 1, &product2);
				if (!rc && (product1.price < product2.price || (product1.price == product2.price && product1.quantity < product2.quantity)))
					rc = put_product_by_pos(f, j + 1, &product1) || put_product_by_pos(f, j, &product2);
			}
	}
	return rc;
}

int copy_bin_file(FILE *fin, FILE *fout)
{
	size_t size;
	product_t product = { 0 };
	memset(&product, 0, sizeof(product_t));
	int rc = file_size(fin, &size) || size <= 0 || size % sizeof(product_t);
	if (rc == 0)
	{
		for (size_t i = 0; !rc && i < size / sizeof(product_t); i++)
		{
			if (fread(&product, sizeof(product_t), 1, fin) == 1)
			{
				if (fwrite(&product, sizeof(product_t), 1, fout) != 1)
					rc = WRITE_ERR;
			}
			else
				rc = READ_ERR;
		}
	}
	return rc;
}

int ends_with(char *str, char *substr)
{
	int ret_key = 0;
	size_t str_len = strlen(str);
	size_t substr_len = strlen(substr);
	if (substr_len <= str_len)
		ret_key = !strcmp(str + str_len - substr_len, substr);
	return ret_key;	
}

int print_products(FILE *f, char *substr)
{
	size_t size;
	product_t product = { 0 };
	memset(&product, 0, sizeof(product_t));
	int rc = file_size(f, &size) || size <= 0 || size % sizeof(product_t);
	int cnt = 0;
	if (rc == 0)
	{
		for (size_t i = 0; !rc && i < size / sizeof(product_t); i++)
		{
			if (fread(&product, sizeof(product_t), 1, f) == 1)
			{
				if (ends_with(product.name, substr))
				{
					printf("%s\n", product.name);
					printf("%s\n", product.manufacturer);
					printf("%u\n", product.price);
					printf("%u\n", product.quantity);
					cnt++;
				}
			}
			else
				rc = READ_ERR;
		}
	}
	if (!cnt)
		rc = NO_PRODUCTS_FOUND;
	return rc;
}

int read_line(FILE *f, char *str, int len)
{
	int rc = 0;
	char ch = fgetc(f);
	int i;
	for (i = 0; i < len && ch != EOF && ch != '\n'; i++)
	{
		str[i] = ch;
		ch = fgetc(f);
	}
	str[i] = '\0';
	if (ch != '\n' && ch != EOF)
		rc = WRONG_INPUT;
	return rc;
}

int input_product(FILE *f, product_t *product)
{
	int rc = read_line(f, product->name, MAX_NAME_LEN);
	if (!rc)
		rc = read_line(f, product->manufacturer, MAX_MANUFACTURER_LEN);
	if (!rc)
		rc = fscanf(f, "%u", &product->price) != 1;
	if (!rc)
		rc = fscanf(f, "%u", &product->quantity) != 1;
	fscanf(f, "\n");
	return rc;	
}

int add_product(FILE *f, product_t *product, size_t size)
{
	size /= sizeof(product_t);
	int rc = put_product_by_pos(f, size, product);
	if (!rc)
	{
		int done = 0;
		product_t product1 = { 0 }, product2 = { 0 };
		for (int i = size; !rc && !done && i > 0; i--)
		{
			rc = get_product_by_pos(f, i, &product1) || get_product_by_pos(f, i - 1, &product2);
			if (!rc && (product1.price > product2.price || (product1.price == product2.price && product1.quantity > product2.quantity)))
				rc = put_product_by_pos(f, i - 1, &product1) || put_product_by_pos(f, i, &product2);
			else
				done = 1;
		}
	}
	return rc;
}