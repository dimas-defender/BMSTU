#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int status;
    pid_t childpid1, childpid2;

    if ((childpid1 = fork()) == -1)
    {
        perror("Can’t fork.\n");
        exit(1);
    }
    else if (childpid1 == 0)
    {
        printf("CHILD1\npid: %d; ppid: %d; gid: %d\n\n", getpid(), getppid(), getgid());
        if (execl("fact.out", "fact.out", NULL) == -1)
        {
            perror("Can't exec.\n");
            exit(1);
        }
    }

    if ((childpid2 = fork()) == -1)
    {
        perror("Can't fork.\n");
        exit(1);
    }
    else if (childpid2 == 0)
    {
        printf("CHILD2\npid: %d; ppid: %d; gid: %d\n\n", getpid(), getppid(), getgid());
        if (execl("exp.out", "exp.out", NULL) == -1)
        {
            perror("Can't exec.\n");
            exit(1);
        }
    }
    else
    {
        pid_t childpid = wait(&status);
        printf("Child has finished: PID = %d\n", childpid);
        if (WIFEXITED(status))
            printf("Child exited with code %d\n", WEXITSTATUS(status));
        else
            printf("Child terminated abnormally\n");

        childpid = wait(&status);
        printf("\nChild has finished: PID = %d\n", childpid);
        if (WIFEXITED(status))
            printf("Child exited with code %d\n", WEXITSTATUS(status));
        else
            printf("Child terminated abnormally\n");
    }
    return 0;
}
