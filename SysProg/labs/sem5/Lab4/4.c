#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define msg1 "gjkrf"
#define msg2 "ABNCNASDTERBLFGWRETQBLR"

int main()
{
    int status;
    pid_t childpid1, childpid2;
    char *buf;
    int channel[2];

    if (pipe(channel) == -1)
    {
        perror("Can't pipe.\n");
        return 1;
    }

    if ((childpid1 = fork()) == -1)
    {
        perror("Can’t fork.\n");
        return 1;
    }
    else if (childpid1 == 0)
    {
        close(channel[0]);
        write(channel[1], msg1, sizeof(msg1));
        exit(0);
    }

    if ((childpid2 = fork()) == -1)
    {
        perror("Can't fork.\n");
        return 1;
    }
    else if (childpid2 == 0)
    {
        close(channel[0]);
        write(channel[1], msg2, sizeof(msg2));
        exit(0);
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

        close(channel[1]);
        buf = malloc(sizeof(msg1) + sizeof(msg2));
        read(channel[0], buf, sizeof(msg1));
        printf("\nmessage1 = %s\n", buf);

        read(channel[0], buf + sizeof(msg1), sizeof(msg2));
        printf("message2 = %s\n", buf + sizeof(msg1));
        close(channel[0]);
    }
    return 0;
}
