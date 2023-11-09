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

#define SUCCESS 0
#define ERROR -1

#define N   64
#define NITER 8

#define NPROD 3
#define NCONS 3
#define PROD_MAX_RAND 2
#define CONS_MAX_RAND 5

#define NSEM 3
#define BIN_SEM 0
#define BUFF_FULL 1
#define BUFF_EMPTY 2

int *shm = NULL;
int *shm_prod = NULL;
int *shm_cons = NULL;

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

int producer(const int sid, const int prodid);
int consumer(const int sid, const int consid);

int main()
{
    int perms = S_IRWXU | S_IRWXG | S_IRWXO;

    int fd = shmget(IPC_PRIVATE, (N + 3) * sizeof(int), perms | IPC_CREAT);
    if (fd == -1)
    {
        perror("shmget");
        return ERROR;
    }

    shm = shmat(fd, 0, 0);
    if (shm == (void *)-1)
    {
        perror("shmat");
        return ERROR;
    }

    shm_prod = shm;
    shm_cons = shm + 1;
    shm = shm + 2;
    *shm_prod = 0;
    *shm_cons = 0;

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
            producer(sid, i);
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
            consumer(sid, i);
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
            printf("Child terminated abnormally.\n");
    }

    if (shmdt((void *)shm) == -1)
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

int producer(const int sid, const int prodid)
{
    srand(time(NULL) + prodid);

    for (int i = 0; i < NITER; i++)
    {
        sleep(rand() % PROD_MAX_RAND + 1);

        if (semop(sid, PROD_LOCK, 2) == -1)
            return ERROR;

        const char symb = (*shm_prod % 26) + 'a';
        *(shm + *shm_prod) = symb;
        (*shm_prod)++;
        (*shm_prod) %= N;

        printf("Producer %d (pid %d) write: %c\n", prodid + 1, getpid(), symb);

        if (semop(sid, PROD_RELEASE, 2) == -1)
            return ERROR;
    }
    return SUCCESS;
}

int consumer(const int sid, const int consid)
{
    srand(time(NULL) + consid + 3);

    for (int i = 0; i < NITER; i++)
    {
        sleep(rand() % CONS_MAX_RAND + 1);

        if (semop(sid, CONS_LOCK, 2) == -1)
            return ERROR;

        char symb = *(shm + *shm_cons) + 'a';
        (*shm_cons)++;
        (*shm_cons) %= N;

        printf("Consumer %d (pid %d) read: %c\n", consid + 1, getpid(), symb);

        if (semop(sid, CONS_RELEASE, 2) == -1)
            return ERROR;
    }
    return SUCCESS;
}
