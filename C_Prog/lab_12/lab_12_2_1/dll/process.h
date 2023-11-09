#ifndef _PROCESS_H_
#define _PROCESS_H_

#ifdef EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

DLL int __cdecl key(const int *pb_src, const int *pe_src, int **pb_dst, int **pe_dst);
DLL void __cdecl mysort(void *base, size_t num, size_t size, int (*compare)(const void *, const void *));
DLL int __cdecl compare_int(const void *arg1, const void *arg2);
DLL void __cdecl free_data(int *data);

#endif