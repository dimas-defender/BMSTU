#include <stdio.h>
#include "func.h"

int main(int argc, char **argv)
{
	int ret_key = 0;
	FILE *f;
	if (argc != 2)
	{
		fprintf(stderr, "Error\n");
		ret_key = WRONG_LAUNCH;
	}
	else
	{
		f = fopen(argv[1], "r");
		if (f == NULL)
		{
			fprintf(stderr, "I/O error\n");
			ret_key = NO_FILE;
		}
		else
		{
			float avg;
			ret_key = get_avg(f, &avg);
			if (ret_key)
				fprintf(stderr, "Not enough data\n");
			else
			{
				rewind(f);
				printf("%d", count_numbers(f, avg));
			}
		}
		fclose(f);
	}
	return ret_key;
}