#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defines.h"
#include "process.h"
#include "input.h"
#include "output.h"

int main(void)
{
	setbuf(stdout, NULL);
	int rc = SUCCESS;
	char type[TYPE_LEN + 1];
	if (scanf("%s", type) != 1 || (strcmp(type, "val") && strcmp(type, "ddx") && strcmp(type, "sum") && strcmp(type, "dvd")))
		rc = INPUT_ERR;
	else
	{
		node_t *head1 = NULL;
		int deg1;
		rc = input_polynom(&head1, &deg1);
		if (!rc)
		{
			if (!strcmp(type, "val"))
			{
				int a;
				if (scanf("%d", &a) != 1)
					rc = INPUT_ERR;
				else
					printf("%d\n", get_polynom_value(head1, deg1, a));
			}
			else if (!strcmp(type, "ddx"))
			{
				node_t *deriv_head = NULL;
				int deriv_deg = deg1 ? deg1 - 1 : deg1;
				rc = create_list(&deriv_head, deriv_deg + 1);
				if (!rc)
				{
					get_derivative(head1, deriv_head, deg1);
					print_polynom(deriv_head);
				}
				free_list(deriv_head);
			}
			else if (!strcmp(type, "sum"))
			{
				node_t *head2 = NULL;
				int deg2;
				rc = input_polynom(&head2, &deg2);
				if (!rc)
				{
					node_t *sum_head = NULL;
					int sum_deg = deg1 > deg2 ? deg1 : deg2;
					rc = create_list(&sum_head, sum_deg + 1);
					if (!rc)
					{
						if (deg1 > deg2)
							sum_polynom(head1, head2, sum_head, deg1 - deg2);
						else
							sum_polynom(head2, head1, sum_head, deg2 - deg1);
						print_polynom(sum_head);
					}
					free_list(sum_head);
				}
				free_list(head2);
			}
			else if (!strcmp(type, "dvd"))
			{
				node_t *even_head = NULL, *odd_head = NULL;
				if (deg1 == 0)
					rc = create_list(&even_head, deg1 + 1);
				else if (deg1 % 2 == 0)
				{
					rc = create_list(&even_head, deg1 / 2 + 1);
					if (!rc)
						rc = create_list(&odd_head, deg1 / 2);
				}
				else
				{
					rc = create_list(&even_head, (deg1 + 1) / 2);
					if (!rc)
						rc = create_list(&odd_head, (deg1 + 1) / 2);
				}
				if (!rc)
				{
					divide_into_even_and_odd(head1, even_head, odd_head, deg1);
					print_polynom(even_head);
					print_polynom(odd_head);
				}
				free_list(even_head);
				free_list(odd_head);
			}
		}
		free_list(head1);
	}
	return rc;
}