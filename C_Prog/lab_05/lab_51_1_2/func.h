#ifndef _FUNC_H_
#define _FUNC_H_

#define WRONG_LAUNCH -1
#define NO_FILE -2
#define NO_DATA -3

int get_avg(FILE *f, float *avg);
int count_numbers(FILE *f, float avg);

#endif