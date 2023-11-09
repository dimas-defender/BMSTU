#ifndef _DEFINES_H_
#define _DEFINES_H_

#define SUCCESS 0
#define MEMORY_ERR 1
#define INPUT_ERR 2
#define TYPE_LEN 3

typedef struct node
{
	int coef;
	struct node *next;
} node_t;

#endif