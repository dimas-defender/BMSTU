#ifndef _PROCESS_H_
#define _PROCESS_H_

int key(const int *pb_src, const int *pe_src, int **pb_dst, int **pe_dst);
void mysort(void *base, size_t num, size_t size, int (*compare)(const void *, const void *));
int compare_int(const void *arg1, const void *arg2);
void free_data(int *data);

#endif