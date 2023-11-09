#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <wait.h>
#include <time.h>

#define SUCCESS 0
#define ERROR -1

#define NITER 10
#define NW 3
#define NR 5

#define ACT_READ 0
#define ACT_WRITE 1
#define WAIT_READ 2
#define WAIT_WRITE 3
#define BIN_SEM 4

#define NSEM 5
#define MAX_RAND 3

int start_reading(int isem_descr);
int stop_reading(int isem_descr);
int start_writing(int isem_descr);
int stop_writing(int isem_descr);
int reader(int *cnt, int isem_descr);
int writer(int *cnt, int isem_descr);

struct sembuf start_read[] = {
    {WAIT_READ, 1, 0},
    {ACT_WRITE, 0, 0},
    {WAIT_WRITE, 0, 0},
    {ACT_READ, 1, 0},
    {WAIT_READ, -1, 0},
};

struct sembuf stop_read[] = {
    {ACT_READ, -1, 0},
};

struct sembuf start_write[] = {
    {WAIT_WRITE, 1, 0},
    {ACT_READ, 0, 0},
    {ACT_WRITE, 0, 0},
    {BIN_SEM, -1, 0},
    {ACT_WRITE, 1, 0},
    {WAIT_WRITE, -1, 0},
};

struct sembuf stop_write[] = {
    {ACT_WRITE, -1, 0},
    {BIN_SEM, 1, 0},
};

int main()
{
    int perms = S_IRWXU | S_IRWXG | S_IRWXO;
    int fd = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | perms);
    if (fd == -1)
    {
        perror("shmget");
        return ERROR;
    }

    int *cnt = (int*)shmat(fd, 0, 0);
    if (cnt == (void*) -1)
    {
        perror("shmat");
        return ERROR;
    }
    *cnt = 0;

    int isem_descr = semget(IPC_PRIVATE, NSEM, IPC_CREAT | perms);
    if (isem_descr == -1)
    {
        perror("semget");
        return ERROR;
    }

    if (semctl(isem_descr, ACT_READ, SETVAL, 0) == -1)
    {
        perror("semctl");
        return ERROR;
    }

    if (semctl(isem_descr, ACT_WRITE, SETVAL, 0) == -1)
    {
        perror("semctl");
        return ERROR;
    }

    if (semctl(isem_descr, WAIT_READ, SETVAL, 0) == -1)
    {
        perror("semctl");
        return ERROR;
    }

    if (semctl(isem_descr, WAIT_WRITE, SETVAL, 0) == -1)
    {
        perror("semctl");
        return ERROR;
    }

    if (semctl(isem_descr, BIN_SEM, SETVAL, 1) == -1)
    {
        perror("semctl");
        return ERROR;
    }

    for (int i = 0; i < NW; i++)
    {
        pid_t childpid;
        if ((childpid = fork()) == -1)
        {
            perror("fork");
            return ERROR;
        }
        else if (childpid == 0)
        {
            writer(cnt, isem_descr);
            exit(SUCCESS);
        }
    }

    for (int i = 0; i < NR; i++)
    {
        pid_t childpid;
        if ((childpid = fork()) == -1)
        {
            perror("fork");
            return ERROR;
        }
        else if (childpid == 0)
        {
            reader(cnt, isem_descr);
            exit(SUCCESS);
        }
    }

    for (int i = 0; i < NW + NR; i++)
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

    if (shmdt(cnt) == -1)
    {
        perror("shmdt");
        return ERROR;
    }

    if (shmctl(fd, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        return ERROR;
    }

    if (semctl(isem_descr, 0, IPC_RMID) == -1)
    {
        perror("semctl");
        return ERROR;
    }
    return SUCCESS;
}

int start_reading(int isem_descr)
{
    if (semop(isem_descr, start_read, 5) == -1)
        return ERROR;
    return SUCCESS;
}

int stop_reading(int isem_descr)
{
    if (semop(isem_descr, stop_read, 1) == -1)
        return ERROR;
    return SUCCESS;
}

int start_writing(int isem_descr)
{
    if (semop(isem_descr, start_write, 6) == -1)
        return ERROR;
    return SUCCESS;
}

int stop_writing(int isem_descr)
{
    if (semop(isem_descr, stop_write, 2) == -1)
        return ERROR;
    return SUCCESS;
}

int reader(int *cnt, int isem_descr)
{
    if (!cnt)
        return ERROR;

    srand(time(NULL));

    for (int i = 0; i < NITER; i++)
    {
        sleep(rand() % MAX_RAND + 1);

        if (start_reading(isem_descr) == ERROR)
        {
            perror("start_reading");
            exit(ERROR);
        }
        int read = *cnt;
        
        if (stop_reading(isem_descr) == ERROR)
        {
            perror("stop_reading");
            exit(ERROR);
        }
        printf("\nRead %d -- %d\n", getpid(), read);
    }
    return SUCCESS;
}

int writer(int *cnt, int isem_descr)
{
    if (!cnt)
        return ERROR;

    srand(time(NULL));

    for (int i = 0; i < NITER; i++)
    {
        sleep(rand() % MAX_RAND + 1);

        if (start_writing(isem_descr) == ERROR)
        {
            perror("start_writing");
            exit(ERROR);
        }
        int wrote = ++(*cnt);
        
        if (stop_writing(isem_descr) == ERROR)
        {
            perror("stop_writing");
            exit(ERROR);
        }
        printf("\nWrite %d -- %d\n", getpid(), wrote);
    }
    return SUCCESS;
}
