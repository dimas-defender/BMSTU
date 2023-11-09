#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include "my_snprintf.h"

int my_snprintf(char *str, size_t n, const char *format, ...);
void reverse_str(char *str, int len);
int get_num_len(unsigned int num, int base);
void int_to_str(unsigned int num, int len, char *str, int base);
int print_int(char *str, size_t n, unsigned int num, int chars_printed, int base);

int my_snprintf(char *str, size_t n, const char *format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);
	int chars_printed = 0, num;
	unsigned short hex_num;
	char c;
	
	for (int i = 0; format[i]; i++)
	{
		if (format[i] == '%')
		{
			i++;
			if (format[i] == 'c')
			{
				c = va_arg(arg_list, int);
				if (chars_printed < (int)(n - 1) && str)
					str[chars_printed] = c;
				chars_printed++;
			}
			else if (format[i] == 'd')
			{
				num = va_arg(arg_list, int);
				unsigned int num_abs = num;
				if (num < 0)
				{
					if (chars_printed < (int)(n - 1) && str)
						str[chars_printed] = '-';
					chars_printed++;
					num_abs = -num;
				}
				chars_printed = print_int(str, n, num_abs, chars_printed, DEC);
			}
			else if (format[i] == 'h' && format[++i] == 'x')
			{
				hex_num = va_arg(arg_list, unsigned int);
				chars_printed = print_int(str, n, hex_num, chars_printed, HEX);
			}
		}
		else
		{
			if (chars_printed < (int)(n - 1) && str)
				str[chars_printed] = format[i];
			chars_printed++;
		}
	}
	if (n > 0 && str)
	{
		if (chars_printed < (int)(n - 1))
			str[chars_printed] = '\0';
		else
			str[n - 1] = '\0';
	}
	va_end(arg_list);
	return chars_printed;
}

int print_int(char *str, size_t n, unsigned int num, int chars_printed, int base)
{
	int len = get_num_len(num, base);
	char tmp_str[len + 1];
	int_to_str(num, len, tmp_str, base);
	int i = 0;
	while (tmp_str[i] && chars_printed < (int)(n - 1) && str)
		str[chars_printed++] = tmp_str[i++];
	while (tmp_str[i])
	{
		chars_printed++;
		i++;
	}
	return chars_printed;
}

int get_num_len(unsigned int num, int base)
{
	int len = 0;
	if (!num)
		len = 1;
	while (num)
	{
		num /= base;
		len++;
	}
	return len;
}

void reverse_str(char *str, int len)
{
	char tmp;
	for (int i = 0; i < len / 2; i++)
	{
		tmp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = tmp;
	}
}

void int_to_str(unsigned int num, int len, char *str, int base)
{
	int i = 0;
	str[i] = '0';
	while (num)
	{
		if (num % base < DEC)
			str[i++] = num % base + '0';
		else
			str[i++] = (num % base) % DEC + 'a';
		num /= base;
	}
	str[len] = '\0';
	reverse_str(str, len);
}