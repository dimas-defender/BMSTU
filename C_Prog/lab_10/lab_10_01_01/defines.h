#ifndef _DEFINES_H_
#define _DEFINES_H_

#define SUCCESS 0
#define MEMORY_ERR 1
#define PTR_ERR 2
#define LAUNCH_ERR 3
#define WRONG_FILE 4
#define READ_ERR 5
#define DATA_ERR 6

#define MAX_RECORDS 30
#define NAME_LEN 20

typedef struct node
{
	void *data;
	struct node *next;
} node_t;

typedef struct town
{
	char name[NAME_LEN + 1];
	int year;
} town_t;

#endif