#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t childpid1, childpid2;

    if ((childpid1 = fork()) == -1)
    {
        perror("Can’t fork.\n");
        exit(1);
    }
    else if (childpid1 == 0)
    {
        printf("CHILD1\npid: %d; ppid: %d; gid: %d\n\n", getpid(), getppid(), getgid());
        return 0;
    }

    if ((childpid2 = fork()) == -1)
    {
        perror("Can't fork.\n");
        exit(1);
    }
    else if (childpid2 == 0)
    {
        sleep(3);
        printf("CHILD2\npid: %d; ppid: %d; gid: %d\n\n", getpid(), getppid(), getgid());
        return 0;
    }
    else
    {
        printf("PARENT\npid: %d; gid: %d; child1: %d; child2: %d\n\n", getpid(), getgid(), childpid1, childpid2);
    }
    return 0;
}
