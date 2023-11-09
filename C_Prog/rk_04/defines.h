#ifndef _DEFINES_H_
#define _DEFINES_H_

#define STR_LEN 200

#define SUCCESS 0
#define MEMORY_ERR 1
#define READ_ERR 2
#define DATA_ERR 3

typedef struct node
{
	char name[STR_LEN + 1];
	char region[STR_LEN + 1];
	char okrug[STR_LEN + 1];
	int population;
	int year;
	struct node *next;
} node_t;

#endif