#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "output.h"

void write_numbers(char *str, int *pbeg, int *pend)
{
	FILE *f = fopen(str, "w");
	for (int *pcur = pbeg; pcur < pend; pcur++)
		fprintf(f, "%d ", *pcur);
	fclose(f);		
}