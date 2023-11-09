#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "defines.h"

typedef int (__cdecl *fn_read_t)(char *, int **, int **);
typedef void (__cdecl *fn_write_t)(char *, int *, int *);
typedef int (__cdecl *fn_key_t)(const int *, const int *, int **, int **);
typedef void (__cdecl *fn_sort_t)(void *, size_t, size_t, int (*compare)(const void *, const void *));
typedef int (__cdecl *fn_cmp_t)(const void *, const void *);
typedef void (__cdecl *fn_free_t)(int *);

int main(int argc, char **argv)
{
	int ret_key = SUCCESS;
	HMODULE hlib;
	fn_read_t read_file;
	fn_write_t write_numbers;
	fn_key_t key;
	fn_sort_t mysort;
	fn_cmp_t compare_int;
	fn_free_t free_data;
	
	hlib = LoadLibrary("lib.dll");
	if (!hlib)
		ret_key = LIBLOAD_ERR;
	if (!ret_key)
	{
		read_file = (fn_read_t) GetProcAddress(hlib, "read_file");
		if (!read_file)
			ret_key = LOAD_ERR;
	}
	if (!ret_key)
	{
		write_numbers = (fn_write_t) GetProcAddress(hlib, "write_numbers");
		if (!write_numbers)
			ret_key = LOAD_ERR;
	}
	if (!ret_key)
	{
		key = (fn_key_t) GetProcAddress(hlib, "key");
		if (!key)
			ret_key = LOAD_ERR;
	}
	if (!ret_key)
	{
		mysort = (fn_sort_t) GetProcAddress(hlib, "mysort");
		if (!mysort)
			ret_key = LOAD_ERR;
	}
	if (!ret_key)
	{
		compare_int = (fn_cmp_t) GetProcAddress(hlib, "compare_int");
		if (!compare_int)
			ret_key = LOAD_ERR;
	}
	if (!ret_key)
	{
		free_data = (fn_free_t) GetProcAddress(hlib, "free_data");
		if (!free_data)
			ret_key = LOAD_ERR;
	}
	if (!ret_key)
	{
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
	}
	if (ret_key != LIBLOAD_ERR)
		FreeLibrary(hlib);
	return ret_key;
}