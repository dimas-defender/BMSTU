#include <stdio.h>
#include "func.h"

int process(FILE *f, int *res)
{
	int prev_num;
	int ret_key = 0;
	if (fscanf(f, "%d", &prev_num) != 1)
		ret_key = NO_DATA;
	else
	{		
		int cur_num, cur_length = 0;
		int sequence_state = 0;
		int check_sequence = 0;
		while (fscanf(f, "%d", &cur_num) == 1)
		{
			check_sequence++;
			if (cur_num > prev_num)
			{
				if (sequence_state == ASCENDING_SEQUENCE)
					cur_length++;
				else
				{
					cur_length = 2;
					sequence_state = ASCENDING_SEQUENCE;
				}
			}
			if (cur_num < prev_num)
			{
				if (sequence_state == DESCENDING_SEQUENCE)
					cur_length++;
				else
				{
					cur_length = 2;
					sequence_state = DESCENDING_SEQUENCE;
				}
			}
			if (cur_num == prev_num)
			{
				cur_length = 0;
				sequence_state = CONST_SEQUENCE;
			}
			prev_num = cur_num;
			if (cur_length > *res)
				*res = cur_length;
		}
		if (!check_sequence)
			ret_key = NO_SEQUENCE;
	}
	return ret_key;
}