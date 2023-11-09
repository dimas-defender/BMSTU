#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100
#define SEP " ,.\n"
#define N 100

struct data
{
	int d;
	int m;
	int y;
};

struct office
{
	char position[20];
	char organization[20];
};

typedef struct
{
	char surname[30];
	char name[20];
	int phone;
	char address[40];
	enum 
	{
		personal,
		work
	}st;
	union
	{
		struct data dofb;
		struct office ofc;
	} type;
} listp;

typedef struct
{
    int index_src;
    char surname[30];
} key_listp;

listp list[MAX], list_q[MAX], list_m[MAX];
key_listp list_surname[MAX], list_surname_q[MAX], list_surname_m[MAX];

// Функции инизиализации и поиска первого несвободного/свободного элемента, функция удаления
void init_list(void)
{
    for(int i = 0; i < MAX; i++)
        list[i].surname[0] = '\0';
}

void init_key_list(void)
{
    for (int i = 0; i < MAX; i++)
        list_surname[i].index_src = -1;
}

int find_not_free_1(void)
{
	int t;
    for (t = 0; list[t].surname[0] == '\0' && t < MAX; t++);

    if (t == MAX)
        return -1;

    return t;
}

int find_not_free_2(void)
{
	int t;
    for (t = 0; list_surname[t].index_src == -1 && t < MAX; t++);

    if (t == MAX)
        return -1;

    return t;
}

int find_free(void)
{
	int t;
    for (t = 0; list[t].surname[0] && t < MAX; t++);

    if (t == MAX)
        return -1;

    return t;
}

void del(int *cnt)
{
    int slot;
    printf("Input index of record to delete: ");
    if (scanf("%d", &slot) != 1)
    {
        printf("\nWrong input data\n");
        scanf("%*s");
        slot = -1;
    }
	int len = *cnt;
    if (slot >= 1 && slot <= len)
    {
        for (int i = slot - 1; i < len; i++)
           list[i] = list[i + 1];
		*cnt = *cnt - 1;
    }
    else
        printf("That record doesn't exist\n");
    
    printf("\n");
}

// Файловый ввод/вывод
void fscan_list(int *cnt)
{
    char name[30];

    printf("Input source file name (with extension): ");
    scanf("%s", name);

	FILE *f_in;
	f_in = fopen(name, "r");
	if (f_in == NULL)
    {
        printf("Can't open file\n\n");
        return;
    }

    char str[100];
    char *estr;
    int slot = 0;
    int flag = 0;

    while (1)
    {
        estr = fgets(str, sizeof(str), f_in);
        if (estr == NULL)
        {
            if (feof(f_in))
            {
                flag = 1;
                break;
            }
            else
            {
                printf("Error while reading file\n");
                break;
            }
        }

        if (slot == MAX)
            break;

		char* fields[10];
		int k = 0;
		int num;
        fields[k] = strtok(str, SEP);
		while (fields[k] != NULL)
			fields[++k] = strtok(NULL, SEP);
        if (k)
        {
	        for (int i = 0; i < k; i++)
	        {
	            if (i == 0)
                {
					if (strlen(fields[i]) > 30)
					{
						printf("\nWrong input data\n\n");
						return;
					}
	                strcpy(list[slot].surname, fields[i]);
                    printf("%s\n", list[slot].surname);
                }
	            else if (i == 1)
                {
					if (strlen(fields[i]) > 20)
					{
						printf("\nWrong input data\n\n");
						return;
					}
	                strcpy(list[slot].name, fields[i]);
                    printf("%s\n", list[slot].name);
                }
	            else if (i == 2)
                {
					if (sscanf(fields[i], "%d", &num) != 1)
					{
						printf("\nWrong input data\n\n");
						return;
					}
	                list[slot].phone = atoi(fields[i]);
                    printf("%d\n", list[slot].phone);
                }
	            else if (i == 3)
                {
					if (strlen(fields[i]) > 40)
					{
						printf("\nWrong input data\n\n");
						return;
					}
	                strcpy(list[slot].address, fields[i]);
                    printf("%s\n", list[slot].address);
                }
	            else if (i == 4)
	            {
	                if (!strcmp("personal", fields[i]))
                    {
	                    list[slot].st = personal;
                        printf("personal\n");
                    }
	                else if (!strcmp("work", fields[i]))
                    {
	                    list[slot].st = work;
                        printf("work\n");
                    }
					else
					{
						printf("\nWrong input data\n\n");
						return;
					}
	            }
	            else if (i == 5)
	            {
	                switch (list[slot].st)
	                {
	                case 0:
						if (sscanf(fields[i], "%d", &num) != 1)
						{
							printf("\nWrong input data\n\n");
							return;
						}
	                    list[slot].type.dofb.d = atoi(fields[i]);
                        printf("%d.", list[slot].type.dofb.d);
	                    break;

	                case 1:
						if (strlen(fields[i]) > 20)
						{
							printf("\nWrong input data\n\n");
							return;
						}
	                    strcpy(list[slot].type.ofc.position, fields[i]);
                        printf("%s\n", list[slot].type.ofc.position);
	                    break;
	                }
	            }
	            else if (i == 6)
	            {
	                switch (list[slot].st)
	                {
	                case 0:
						if (sscanf(fields[i], "%d", &num) != 1)
						{
							printf("\nWrong input data\n\n");
							return;
						}
	                    list[slot].type.dofb.m = atoi(fields[i]);
                        printf("%d.", list[slot].type.dofb.m);
	                    break;

	                case 1:
						if (strlen(fields[i]) > 20)
						{
							printf("\nWrong input data\n\n");
							return;
						}
	                    strcpy(list[slot].type.ofc.organization, fields[i]);
                        printf("%s\n", list[slot].type.ofc.organization);
	                    break;
	                }
	            }
	            else if (i == 7)
                {
					if (sscanf(fields[i], "%d", &num) != 1)
					{
						printf("\nWrong input data\n\n");
						return;
					}
	                list[slot].type.dofb.y = atoi(fields[i]);
                    printf("%d\n", list[slot].type.dofb.y);
                }
	        }
        }
        printf("Index = %d\n\n", slot + 1);
		slot++;
		*cnt = *cnt + 1;
    }
    if (slot == MAX && flag == 0)
        printf("\nThere is no more space left in table\n");
	else if (!slot)
		printf("File is empty\n");
    else
        printf("OK\n");

    printf("\n");
    fclose(f_in);
}

void fprint_list(void)
{
    char name[30];

    printf("Input destination file name (with extension): ");
    scanf("%s", name);

    FILE *f_out;
    f_out = fopen(name, "w");

    if (f_out == NULL)
    {
        printf("Can't open file\n");
        return;
    }

    for (int i = 0; i < MAX; i++)
    {
        if (list[i].surname[0] != '\0')
        {
            fprintf(f_out, "%s,", list[i].surname);
            fprintf(f_out, "%s,", list[i].name);
            fprintf(f_out, "%d,", list[i].phone);
            fprintf(f_out, "%s,", list[i].address);

            switch (list[i].st)
            {
            case 0:
            	fprintf(f_out, "personal,");
                fprintf(f_out, "%d.%d.%d\n", list[i].type.dofb.d, list[i].type.dofb.m, list[i].type.dofb.y);
                break;

            case 1:
            	fprintf(f_out, "work,");
                fprintf(f_out, "%s,", list[i].type.ofc.position);
                fprintf(f_out, "%s\n", list[i].type.ofc.organization);
                break;
            }
        }
    }
    printf("OK\n\n");
    fclose(f_out);
}

// Ввод/вывод для таблицы ключей
void scan_key_list(void)
{
    for (int i = 0; i < MAX; i++)
    {
        if (list[i].surname[0] != '\0')
        {
            list_surname[i].index_src = i + 1;
            strcpy(list_surname[i].surname, list[i].surname);
        }
    }
}

void print_key_list(void)
{
    int flag = find_not_free_2();

    if (flag != -1)
        for (int i = 0; i < MAX; i++)
        {
            if (list_surname[i].index_src != -1)
            {
                printf("Source index: %d\n", list_surname[i].index_src);
                printf("Surname: %s\n", list_surname[i].surname);
                printf("\n");
            }
        }
    else
        printf("Table of keys is empty\n\n");
}

// Выбор типа 
int type(void)
{
    int c;
    printf("1. Personal\n");
    printf("2. Work\n");

    do {
        printf("\nYour choice: ");
        if (scanf("%d", &c) != 1)
        {
            printf("\nWrong input data\n");
            scanf("%*s");
            c = -1;
        }
    } while (c < 0 || c > 2);

    printf("\n");
    return c;
}

// Ввод/вывод на экран
void scan_list(int *cnt)
{
    int slot;
    slot = find_free();

    if (slot == -1)
    {
        printf("\nThere is no more space left in table\n\n");
        return;
    }

    printf("Input surname(max 30 symbols): ");
    if (scanf("%s", list[slot].surname) != 1 || strlen(list[slot].surname) > 30)
    {
        printf("\nWrong input data\n\n");
        list[slot].surname[0] = '\0';
        return;
    }

    printf("Input name(max 20 symbols): ");
    if (scanf("%s", list[slot].name) != 1 || strlen(list[slot].name) > 20)
    {
        printf("\nWrong input data\n\n");
        list[slot].surname[0] = '\0';
        return;
    }

    printf("Input phone number(integer): ");
    if (scanf("%d", &list[slot].phone) != 1)
    {
        printf("\nWrong input data\n\n");
        scanf("%*s");
        list[slot].surname[0] = '\0';
        return;
    }

    printf("Input address(max 40 symbols): ");
    if (scanf("%s", list[slot].address) != 1 || strlen(list[slot].address) > 40)
    {
        printf("\nWrong input data\n\n");
        list[slot].surname[0] = '\0';
        return;
    }

    printf("\nChoose information type:\n");
    int choice = type();
    switch(choice)
    {
        case 1: list[slot].st = personal;

            printf("Input date of birth (dd mm yyyy): ");
            if (scanf("%d %d %d", &list[slot].type.dofb.d, &list[slot].type.dofb.m, &list[slot].type.dofb.y) != 3)
            {
                printf("\nWrong input data\n\n");
                list[slot].surname[0] = '\0';
                return;
            }
            else if ((list[slot].type.dofb.d < 0) || (list[slot].type.dofb.d > 31) || (list[slot].type.dofb.m < 0) || (list[slot].type.dofb.m > 12) || (list[slot].type.dofb.y < 0))
            {
                printf("\nWrong input data\n\n");
                list[slot].surname[0] = '\0';
                return;
            }
			printf("\n");
            break;

        case 2: list[slot].st = work;

		    printf("Input position(max 20 symbols): ");
		    if (scanf("%s", list[slot].type.ofc.position) != 1 || strlen(list[slot].type.ofc.position) > 20)
		    {
		        printf("\nWrong input data\n\n");
		        list[slot].surname[0] = '\0';
		        return;
		    }

		    printf("Input organization(max 20 symbols): ");
		    if (scanf("%s", list[slot].type.ofc.organization) != 1 || strlen(list[slot].type.ofc.organization) > 20)
		    {
		        printf("\nWrong input data\n\n");
		        list[slot].surname[0] = '\0';
		        return;
		    }
            printf("\n");
            break;
    }
	*cnt = *cnt + 1;
}

void print_list(void)
{
    int flag = find_not_free_1();

    if (flag != -1)
        for (int slot = 0; slot < MAX; ++slot)
        {
            if (list[slot].surname[0])
            {
                printf("Index: %d\n", slot + 1);
                printf("Surname: %s\n", list[slot].surname);
                printf("Name: %s\n", list[slot].name);
                printf("Phone number: %d\n", list[slot].phone);
                printf("Address: %s\n", list[slot].address);

                switch (list[slot].st)
                {
                case 0:
                    printf("Date of birth: %d.%d.%d\n", list[slot].type.dofb.d, list[slot].type.dofb.m, list[slot].type.dofb.y);
                    break;
                case 1:
                    printf("Position: %s\n", list[slot].type.ofc.position);
                    printf("Organization: %s\n", list[slot].type.ofc.organization);
                    break;
                }
                printf("\n");
            }
        }
    else
        printf("Table of subscribers is empty\n\n");
}

void print_list_using_keys(void)
{
    int flag = find_not_free_2();

    if (flag != -1)
	{
        for (int i = 0; i < MAX; i++)
            if (list_surname[i].index_src != -1)
			{
				int j = list_surname[i].index_src - 1;
				printf("Index: %d\n", j + 1);
				printf("Surname: %s\n", list[j].surname);
				printf("Name: %s\n", list[j].name);
				printf("Phone number: %d\n", list[j].phone);
				printf("Address: %s\n", list[j].address);
				
				switch (list[j].st)
				{
				case 0:
					printf("Date of birth: %d.%d.%d\n", list[j].type.dofb.d, list[j].type.dofb.m, list[j].type.dofb.y);
					break;
				case 1:
					printf("Position: %s\n", list[j].type.ofc.position);
					printf("Organization: %s\n", list[j].type.ofc.organization);
					break;
				}
				printf("\n");
			}
	}
    else
        printf("Table of keys is empty\n\n");
}

// Сортировка
int compare_list(const void *p, const void *q)
{
    const listp* a = (const listp*)p;
    const listp* b = (const listp*)q;

    return strcmp(a->surname, b->surname);
}

void swap(void *x, void *y, int size)
{
    char *a = x;
    char *b = y;
    char temp;

    for (int i = 0; i < size; i++)
    {
		temp = *a;
		*a = *b;
		*b = temp;
		a++;
		b++;
    }
}

void bubble_sort(void *a, int n, int size, int (*compare)(const void *, const void *))
{
    char *pb = a;
    char *pe = pb + n * size;
    for (char *pi = pe - size; pi >= pb + size; pi = pi - size) 
    {
        char *max_i = pi;
        for (char *pj = pi - size; pb <= pj; pj = pj - size) 
        {
            if (compare(pj, max_i) > 0) 
                max_i = pj;
        }
        swap(pi, max_i, size);
    }
}

void sort_list(int cnt)
{
    int f = find_not_free_1();
    if (f != -1)
    {
        qsort(list, cnt, sizeof(listp), compare_list);
        printf("OK\n");
    }
    else
        printf("List is empty\n");

    printf("\n");
}

int compare_key_list(const void *p, const void *q)
{
    const key_listp* a = (const key_listp*)p;
    const key_listp* b = (const key_listp*)q;

    return strcmp(a->surname, b->surname);
}

void sort_key_list(int cnt)
{
    int f = find_not_free_2();
    if (f != -1)
    {
        qsort(list_surname, cnt, sizeof(key_listp), compare_key_list);
        printf("OK\n");
    }
    else
        printf("Key list is empty\n");

    printf("\n");
}

// Функция для поиска абонентов с ДР в ближайшую неделю
void spec(int cnt)
{
    int flag = 0;
    int day[7];
    int month[7];

    printf("Input today's date (dd mm): ");
    scanf("%d %d", day, month);
    for (int i = 1; i < 8; i++)
    {
	    day[i] = day[i-1] + 1;
	    month[i] = month[i-1];
	    if ((day[i]  > 31) && ((month[i] == 1) || (month[i] == 3) || (month[i] == 5) || (month[i] == 7) || (month[i] == 8) || (month[i] == 10)))
	    {
	    	month[i] = month[i] + 1;
	    	day[i] = day[i] - 31;
	    }
	    else if ((day[i] > 31) && (month[i] == 12))
	    {
	    	month[i] = 1;
	    	day[i] = day[i] - 31;
	    }
	    else if ((day[i]  > 30) && ((month[i] == 4) || (month[i] == 6) || (month[i] == 9) || (month[i] == 11) ))
	    {
	    	month[i] = month[i] + 1;
	    	day[i] = day[i] - 30;
	    }
	    else if ((day[i] > 28) && (month[i] == 2))
	    {
	    	month[i] = month[i] + 1;
	    	day[i] = day[i] - 28;
	    }
	}
    for (int i = 0; i < cnt; i++)
    {
    	for (int j = 0; j < 8; j++)
			if (list[i].surname[0] && list[i].st == personal && list[i].type.dofb.d == day[j] && list[i].type.dofb.m == month[j])
			{
				printf("%s\n", list[i].surname);
				flag = 1;
			}
    }
    if (flag == 0)
        printf("There are no birthdays next week\n");
    printf("\n");
}

//Функции анализа эффективности видов работы с данными и типов сортировок

unsigned long long tick(void)
{
    unsigned long long d;

    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

void check_ef(int len)
{
    int f_1 = find_not_free_1();
    int f_2 = find_not_free_2();

    if (f_1 != -1 && f_2 != -1)
    {
        unsigned long tb, te, t_1, t_2, t_3, t_4;

        t_1 = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < len; j++)
                list_q[j] = list[j];
            tb = tick();
            qsort(list_q, len, sizeof(listp), compare_list);
            te = tick();
            t_1 += (te - tb);
        }

        t_2 = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < len; j++)
                list_surname_q[j] = list_surname[j];
            tb = tick();
            qsort(list_surname_q, len, sizeof(key_listp), compare_key_list);
            te = tick();
            t_2 += (te - tb);
        }
		
		t_3 = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < len; j++)
                list_m[j] = list[j];
            tb = tick();
            bubble_sort(list_m, len, sizeof(listp), compare_list);
            te = tick();
            t_3 += (te - tb);
        }
		
		t_4 = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < len; j++)
                list_surname_m[j] = list_surname[j];
            tb = tick();
            bubble_sort(list_surname_m, len, sizeof(key_listp), compare_key_list);
            te = tick();
            t_4 += (te - tb);
        }
		printf("Bubble sort time = %lu\n", t_4);
        printf("Quick sort time = %lu\n", t_2);
        printf("Quick sort is %lu%% faster than bubble sort\n\n", (100 - 100 * t_2 / t_4));

        printf("Source table sort time = %lu\n", t_1);
        printf("Table of keys sort time = %lu\n", t_2);

        printf("It is %lu%% faster to sort table of keys than the source one\n\n", (100 - 100 * t_2 / t_1));
		

        unsigned long size_1, size_2;

        size_1 = sizeof(listp) * len;
        size_2 = (sizeof(listp) + sizeof(key_listp)) * len;

        printf("Size of main table = %lu\n", size_1);
        printf("Size of main table + table of keys = %lu\n", size_2);

        printf("Table of keys needs %lu%% of extra memory\n", (100 * size_2 / size_1 - 100));
        printf("\n");
    }
    else
        printf("Tables are empty\n\n");
}

// Меню
int menu_select(void)
{
    int c;
    printf("1. Input table of subscribers from file\n");
    printf("2. Write table of subscribers in file\n");
    printf("3. Add subscriber to table\n");
    printf("4. Remove subscriber from table\n");
    printf("5. Print table of subscribers\n");
	printf("6. Print table of subscribers using table of keys\n");
    printf("7. Print table of keys\n");
    printf("8. Sort table of subscribers by surname\n");
    printf("9. Sort table of keys by surname\n");
    printf("10. Compare efficiency of data processing and types of sorting\n");
    printf("11. Print all subscribers which have a birthday next week\n");
    printf("\n0. Exit\n");

    do {
        printf("\nYour choice: ");
        if (scanf("%d", &c) != 1)
        {
            printf("\nWrong input data\n");
            scanf("%*s");
            c = -1;
        }
    } while (c < 0 || c > 12);

    printf("\n");
    return c;
}

int main(void)
{
    setbuf(stdout, NULL);
    char choice;
	int cnt = 0;
	
	printf("This program processes the database stored in a txt file in the current directory.\n\
The database contains information about subscribers. The maximum number of records in the source file is 100.\n\n");

    init_list();
    init_key_list();

    for(;;)
    {
        choice = menu_select();
        switch(choice)
        {
        case 1:
			init_list();
            fscan_list(&cnt);
            init_key_list();
           	scan_key_list();
            break;
        case 2:
			fprint_list();
            break;
        case 3:
			scan_list(&cnt);
            init_key_list();
            scan_key_list();
            break;
        case 4:
			del(&cnt);
			init_key_list();
            scan_key_list();
            break;
        case 5:
			print_list();
            break;
		case 6:
			print_list_using_keys();
            break;
        case 7:
			print_key_list();
            break;
        case 8: 
        	sort_list(cnt);
			init_key_list();
           	scan_key_list();
            break;
        case 9: 
        	sort_key_list(cnt);
            break;
        case 10: 
        	check_ef(cnt);
            break;
        case 11: 
        	spec(cnt);
            break;
        case 0: 
        	return 0;
        }
    }
}