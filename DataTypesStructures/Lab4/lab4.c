#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_STACK_SIZE 50

#define SUCCESS 0
#define EMPTY_STACK 1
#define MEMORY_ERR 2
#define STACK_OVERFLOW 3

typedef struct sNode{
	float value;
	struct sNode *next;
} tNode;

typedef struct{
	tNode *p_begin;
	int size;
} tStack;

tNode *empty_adr[MAX_STACK_SIZE] = {NULL};
int arr_len1 = 0, arr_len2 = 0;

void print_empty_adr(void);
void print_list(tNode *p_begin);
void stack_print(const tStack *s);
int stack_push(tStack *s, float value);
int stack_pop(tStack *s, float *value);
void delete_list(tNode *p_begin);
void stack_clear(tStack *s);
tStack stack_create(void);
int menu_select(void);
int arr_push(float **stack, int num, float value);
int arr_pop(float **stack, int num, float *value);
void arr_print(float *stack, int num);
unsigned long long tick(void);
void test_stack_push(tStack *s, float value);
void test_stack_pop(tStack *s);
int choose_stack(void);

int main(void)
{
	setbuf(stdout, NULL);
	float arr_stack[2 * MAX_STACK_SIZE];
	float *stack1_top = arr_stack - 1, *stack2_top = arr_stack + 2 * MAX_STACK_SIZE;
	tStack list = stack_create();
	int choice, ch1, ch2, flag = 1;
	float pop_val, push_val;
	unsigned long tb, te, t_1, t_2;
	unsigned long size_1, size_2;
	printf("This program allows to perform actions on the stack, which is implemented using both array and list.\n\n");
	while (flag)
	{
		choice = menu_select();
		switch (choice)
		{
			case 1:
			    printf("1. From stack on list\n");
				printf("2. From 1st stack on array\n");
				printf("3. From 2nd stack on array\n");
				ch1 = choose_stack();
				switch(ch1)
				{
					case 1:
						if (!stack_pop(&list, &pop_val))
							printf("Element %f was popped\n\n", pop_val);
						break;
					case 2:
						if (!arr_pop(&stack1_top, 1, &pop_val))
							printf("Element %f was popped\n\n", pop_val);
						break;
					case 3:
						if (!arr_pop(&stack2_top, 2, &pop_val))
							printf("Element %f was popped\n\n", pop_val);
						break;
				}
				break;
			case 2:
				printf("1. To stack on list\n");
				printf("2. To 1st stack on array\n");
				printf("3. To 2nd stack on array\n");
				ch2 = choose_stack();
				
				printf("Input float value to push: ");
				if (scanf("%f", &push_val) != 1)
					printf("Wrong input\n\n");
				else
					switch(ch2)
					{
						case 1:
							if (!stack_push(&list, push_val))
								printf("Element %f was pushed\n\n", push_val);
							break;
						case 2:
							if (!arr_push(&stack1_top, 1, push_val))
								printf("Element %f was pushed\n\n", push_val);
							break;
						case 3:
							if (!arr_push(&stack2_top, 2, push_val))
								printf("Element %f was pushed\n\n", push_val);
							break;
					}
				break;
			case 3:
				printf("Stack on list:\n");
				stack_print(&list);
				printf("Stack on array:\n");
				arr_print(stack1_top, 1);
				arr_print(stack2_top, 2);
				printf("\n");
				break;
			case 4:
				print_empty_adr();
				break;
			case 5:
				if (list.size < MAX_STACK_SIZE && (arr_len1 + arr_len2 < 2 * MAX_STACK_SIZE))
				{
					float push = 5.555, pop;
					tb = tick();
					test_stack_push(&list, push);
					te = tick();
					t_1 = te - tb;
					tb = tick();
					arr_push(&stack1_top, 1, push);
					te = tick();
					t_2 = te - tb;
					printf("List push time: %lu\n", t_1);
					printf("Array push time: %lu\n\n", t_2);
					tb = tick();
					test_stack_pop(&list);
					te = tick();
					t_1 = te - tb;
					tb = tick();
					arr_pop(&stack1_top, 1, &pop);
					te = tick();
					t_2 = te - tb;
					printf("List pop time: %lu\n", t_1);
					printf("Array pop time: %lu\n\n", t_2);
					size_1 = sizeof(tStack) + list.size * sizeof(tNode);
					size_2 = MAX_STACK_SIZE * sizeof(float);
					printf("Memory size needed for list: %lu\n", size_1);
					printf("Memory size needed for array: %lu\n\n", size_2);
				}
				else
					printf("Stack overflow, can't compare efficiency\n\n");
				break;
			case 0:
				flag = 0;
				stack_clear(&list);
				break;
		}
	}
	return SUCCESS;
}

int choose_stack(void)
{
    int c;
    do {
        printf("\nYour choice: ");
        if (scanf("%d", &c) != 1)
        {
            printf("\nWrong input\n");
            scanf("%*s");
            c = -1;
        }
    } while (c < 1 || c > 3);

    printf("\n");
    return c;
}

int menu_select(void)
{
    int c;
    printf("1. Pop element\n");
	printf("2. Push element\n");
	printf("3. Print current state of the stack\n");
	printf("4. Print empty adresses\n");
	printf("5. Compare efficiency\n");
    printf("\n0. Exit\n");

    do {
        printf("\nYour choice: ");
        if (scanf("%d", &c) != 1)
        {
            printf("\nWrong input\n");
            scanf("%*s");
            c = -1;
        }
    } while (c < 0 || c > 5);

    printf("\n");
    return c;
}

void print_empty_adr(void)
{
	int flag = 0;
	printf("Empty addresses:\n");
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		if (empty_adr[i] != 0)
		{
			flag = 1;
			printf("%p\n", (void *) empty_adr[i]);
		}
	if (!flag)
		printf("[No adresess]\n");
	printf("\n");
}

void print_list(tNode *p_begin)
{
	tNode *p = p_begin;
	while (p != NULL)
	{
		printf("%f %p\n", p->value, (void *) p);
		p = p->next;
	}
}

void stack_print(const tStack *s)
{
	if (s->size == 0)
		printf("[Empty stack]\n");
	print_list(s->p_begin);
	printf("\n");
}

void arr_print(float *stack, int num)
{
	int q = 1;
	int arr_len = arr_len1;
	if (num == 2)
	{
		q = -1;
		arr_len = arr_len2;
	}
	printf("Stack%d: ", num);
	if (!arr_len)
		printf("[Empty stack]\n");
	for (int i = 0; i < arr_len; i++)
	{
		printf("%f ", *stack);
		stack = stack - q;
	}
	printf("\n");
}

int stack_push(tStack *s, float value)
{
	if (s->size >= MAX_STACK_SIZE)
	{
		printf("Stack overflow, can't push element\n\n");
		return STACK_OVERFLOW;
	}
	tNode *p = (tNode *)malloc(sizeof(tNode));
	if (!p)
	{
		printf("Memory error\n");
		return MEMORY_ERR;
	}	
	for (int j = 0; j < MAX_STACK_SIZE; j++)
		if (empty_adr[j] == p)
		{
			for (int k = j; k < MAX_STACK_SIZE - 1; k++)
				empty_adr[k] = empty_adr[k + 1];
			break;
		}
	p->value = value;
	p->next = s->p_begin;
	s->p_begin = p;
	s->size++;
	return SUCCESS;
}

void test_stack_push(tStack *s, float value)
{
	tNode *p = (tNode *)malloc(sizeof(tNode));
	p->value = value;
	p->next = s->p_begin;
	s->p_begin = p;
	s->size++;
}

int arr_push(float **stack, int num, float value)
{
	int q = 1;
	if (num == 2)
		q = -1;
	if (arr_len1 + arr_len2 >= 2 * MAX_STACK_SIZE)
	{
		printf("Array overflow, can't push element\n\n");
		return STACK_OVERFLOW;
	}
	*stack = *stack + q;
	**stack = value;
	if (num == 1)
		arr_len1++;
	else
		arr_len2++;
	return SUCCESS;
}

int stack_pop(tStack *s, float *value)
{
	if (s->size == 0)
	{
		printf("Trying to pop from empty stack!\n\n");
		return EMPTY_STACK;
	}
	tNode *tmp = s->p_begin;
	float tmp_value = tmp->value;
	s->p_begin = s->p_begin->next;
	s->size--;
	for (int j = 0; j < MAX_STACK_SIZE; j++)
		if (!empty_adr[j])
		{
			empty_adr[j] = tmp;
			break;
		}
	free(tmp);
	*value = tmp_value;
	return SUCCESS;
}

void test_stack_pop(tStack *s)
{
	tNode *tmp = s->p_begin;
	s->p_begin = s->p_begin->next;
	s->size--;
	free(tmp);
}

int arr_pop(float **stack, int num, float *value)
{
	int q = 1;
	int arr_len = arr_len1;
	if (num == 2)
	{
		q = -1;
		arr_len = arr_len2;
	}
	if (arr_len == 0)
	{
		printf("Trying to pop from empty stack!\n\n");
		return EMPTY_STACK;
	}
	*value = **stack;
	*stack = *stack - q;
	if (num == 1)
		arr_len1--;
	else
		arr_len2--;
	return SUCCESS;
}

void delete_list(tNode *p_begin)
{
	tNode *p = p_begin;
	while (p != NULL)
	{
		tNode *tmp;
		tmp = p;
		p = p->next;
		free(tmp);
	}
}

void stack_clear(tStack *s)
{
	delete_list(s->p_begin);
	s->p_begin = NULL;
	s->size = 0;
}

tStack stack_create(void)
{
	tStack new_stack = {NULL, 0};
	return new_stack;
}

unsigned long long tick(void)
{
    unsigned long long d;

    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}