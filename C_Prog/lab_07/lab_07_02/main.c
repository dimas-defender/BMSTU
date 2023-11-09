#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "input.h"
#include "output.h"
#include "process.h"

int main(int argc, char **argv)
{
	int ret_key = SUCCESS;
	if (argc != 3 && argc != 4)
		ret_key = WRONG_LAUNCH;
	else
	{
		int *arr_pb, *arr_pend;
		ret_key = read_file(argv[1], &arr_pb, &arr_pend);
		if (!ret_key)
		{
			if (argc == 4 && !strcmp(argv[3], "f"))
			{
				int *new_arr_pb, *new_arr_pend;
				ret_key = key(arr_pb, arr_pend, &new_arr_pb, &new_arr_pend);
				if (!ret_key)
				{
					mysort(new_arr_pb, new_arr_pend - new_arr_pb, sizeof(int), compare_int);
					write_numbers(argv[2], new_arr_pb, new_arr_pend);
					free_data(new_arr_pb);
				}
			}
			else if (argc == 3)
			{
				mysort(arr_pb, arr_pend - arr_pb, sizeof(int), compare_int);
				write_numbers(argv[2], arr_pb, arr_pend);
			}
			else
				ret_key = WRONG_LAUNCH;
			free_data(arr_pb);
		}
	}
	return ret_key;
}