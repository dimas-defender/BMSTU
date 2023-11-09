#ifndef _FUNC_H_
#define _FUNC_H_

#ifdef EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#define SUCCESS 0
#define NOT_ENOUGH_SPACE 1

DLL void __cdecl fill_fibon(int *a, int n);
DLL int __cdecl get_unique_elems(int *src, int *dst, int src_len, int *dst_len);

#endif