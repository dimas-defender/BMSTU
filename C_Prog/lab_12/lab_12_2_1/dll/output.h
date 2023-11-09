#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#ifdef EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

DLL void __cdecl write_numbers(char *str, int *pbeg, int *pend);

#endif