#include <stdlib.h>
#include <stdio.h>
#include "func.h"
#include "input.h"
#include "output.h"
#include "defines.h"

//Прочитать из файла данные о городах (название и год основания), отсортировать записи по году и записать в другой файл

int main(int argc, char **argv)
{
	int rc = SUCCESS;
	if (argc != 3)
		rc = LAUNCH_ERR;
	else
	{
		town_t towns[MAX_RECORDS];
		int quantity;
		rc = read_file(argv[1], towns, &quantity);
		if (!rc)
		{
			node_t *head;
			rc = create_list(&head, quantity);
			if (!rc)
			{
				fill_list(head, towns);
				head = sort(head, compare_by_year);
				print_towns(argv[2], head);
			}
			free_list(head);
		}
	}
	return rc;
}