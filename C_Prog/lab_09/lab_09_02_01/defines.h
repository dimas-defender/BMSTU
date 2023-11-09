#ifndef _DEFINES_H_
#define _DEFINES_H_

#define SUCCESS 0
#define WRONG_LAUNCH 53
#define NO_FILE -2
#define READ_ERR -3
#define DATA_ERR -4
#define MEMORY_ERR -5
#define GETLINE_BUFLEN 256

typedef struct
{
    char *name;
    float weight;
    float size;
} item_t;

#endif