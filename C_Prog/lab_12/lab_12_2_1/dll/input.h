#ifndef _INPUT_H_
#define _INPUT_H_

#ifdef EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

DLL int __cdecl read_file(char *str, int **pbeg, int **pend);

#endif