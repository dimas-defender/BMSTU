#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>

#define SUCCESS 0
#define ERROR -1

#define N     24
#define NITER 8

#define NPROD 3
#define NCONS 3
#define PROD_MAX_RAND 2
#define CONS_MAX_RAND 5

#define NSEM 3
#define BIN_SEM 0
#define BUFF_FULL 1
#define BUFF_EMPTY 2

typedef struct buf {
    char* read_pos;
    char* write_pos;
    char symb;
    char data[N];
} buffer_t;

struct sembuf PROD_LOCK[] = {
    { BUFF_EMPTY, -1, 0 },
    { BIN_SEM, -1, 0 }
};

struct sembuf PROD_RELEASE[] = {
    { BUFF_FULL, 1, 0 },
    { BIN_SEM, 1, 0 }
};

struct sembuf CONS_LOCK[] = {
    { BUFF_FULL, -1, 0 },
    { BIN_SEM, -1, 0 }
};

struct sembuf CONS_RELEASE[] = {
    { BUFF_EMPTY, 1, 0 },
    { BIN_SEM, 1, 0 }
};

int write_buffer(buffer_t *const buf, const char element);
int read_buffer(buffer_t *buf, char *const element);
int producer(buffer_t *const buf, const int sid, const int prodid);
int consumer(buffer_t *const buf, const int sid, const int consid);

int main()
{
    int perms = S_IRWXU | S_IRWXG | S_IRWXO;

    int fd = shmget(IPC_PRIVATE, sizeof(buffer_t), perms | IPC_CREAT);
    if (fd == -1)
    {
        perror("shmget");
        return ERROR;
    }

    buffer_t *buffer = shmat(fd, 0, 0);
    if (buffer == (void *)-1)
    {
        perror("shmat");
        return ERROR;
    }

    buffer->read_pos = buffer->data;
    buffer->write_pos = buffer->data;
    buffer->symb = 'a';

    int sid = semget(IPC_PRIVATE, NSEM, perms | IPC_CREAT);
    if (sid == -1)
    {
        perror("semget");
        return ERROR;
    }

    if (semctl(sid, BIN_SEM, SETVAL, 1) == -1)
    {
        perror("semctl");
        return ERROR;
    }

    if (semctl(sid, BUFF_FULL, SETVAL, 0) == -1)
    {
        perror("semctl");
        return ERROR;
    }

    if (semctl(sid, BUFF_EMPTY, SETVAL, N) == -1)
    {
        perror("semctl");
        return ERROR;
    }

    for (int i = 0; i < NPROD; i++)
    {
        pid_t childpid;
        if ((childpid = fork()) == -1)
        {
            perror("fork");
            return ERROR;
        }
        else if (childpid == 0)
        {
            producer(buffer, sid, i);
            return SUCCESS;
        }
    }

    for (int i = 0; i < NCONS; i++)
    {
        pid_t childpid;
        if ((childpid = fork()) == -1)
        {
            perror("fork");
            return ERROR;
        }
        else if (childpid == 0)
        {
            consumer(buffer, sid, i);
            return SUCCESS;
        }
    }

    for (int i = 0; i < NPROD + NCONS; i++)
    {
        int status;
        if (wait(&status) == -1)
        {
            perror("wait");
            return(ERROR);
        }
        if (!WIFEXITED(status))
            printf("WIFEXITED failed.\n");
    }

    if (shmdt((void *)buffer) == -1)
    {
        perror("shmdt");
        return ERROR;
    }

    if (shmctl(fd, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        return ERROR;
    }
    
    if (semctl(sid, 0, IPC_RMID) == -1)
    {
        perror("semctl");
        return ERROR;
    }
    return SUCCESS;
}

int write_buffer(buffer_t *const buf, const char element)
{
    if (!buf)
        return ERROR;

    *(buf->write_pos) = element;
    (buf->write_pos)++;
    if (buf->write_pos >= buf->data + N)
        buf->write_pos = buf->data;

    return SUCCESS;
}

int read_buffer(buffer_t *buf, char *const element)
{
    if (!buf || !element)
        return ERROR;

    *element = *(buf->read_pos);
    (buf->read_pos)++;
    if (buf->read_pos >= buf->data + N)
        buf->read_pos = buf->data;

    return SUCCESS;
}

int producer(buffer_t *const buf, const int sid, const int prodid)
{
    srand(time(NULL) + prodid);

    if (!buf)
        return ERROR;

    for (int i = 0; i < NITER; i++)
    {
        sleep(rand() % PROD_MAX_RAND + 1);

        if (semop(sid, PROD_LOCK, 2) == -1)
            return ERROR;

        char c = buf->symb++;
        if (write_buffer(buf, c) == -1)
            return ERROR;

        printf("Producer %d (pid %d) write: %c\n", prodid + 1, getpid(), c);

        if (semop(sid, PROD_RELEASE, 2) == -1)
            return ERROR;
    }
    return SUCCESS;
}

int consumer(buffer_t *const buf, const int sid, const int consid)
{
    srand(time(NULL) + consid);

    if (!buf)
        return ERROR;

    for (int i = 0; i < NITER; i++)
    {
        sleep(rand() % CONS_MAX_RAND + 1);

        if (semop(sid, CONS_LOCK, 2) == -1)
            return ERROR;

        char symb;
        if (read_buffer(buf, &symb) == -1)
            return ERROR;

        printf("Consumer %d (pid %d) read: %c\n", consid + 1, getpid(), symb);

        if (semop(sid, CONS_RELEASE, 2) == -1)
            return ERROR;
    }
    return SUCCESS;
}
