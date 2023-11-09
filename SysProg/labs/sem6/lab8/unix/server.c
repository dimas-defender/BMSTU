#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#define SOCKET_NAME "socket.soc"
#define SOCKET_PATH "/home/dmitriy/OS/lab8/unix/"
#define BUF_SIZE 256

static int sockfd;

void cleanup(void)
{
	close(sockfd);
	char full_path[BUF_SIZE] = {0};
    strcpy(full_path, SOCKET_PATH);
    strcat(full_path, SOCKET_NAME);
    unlink(full_path);
}

void sighandler(__attribute__((unused)) int  signum)
{
	cleanup();
	exit(EXIT_SUCCESS);
}

void perror_exit(const char *str)
{
	cleanup();
	perror(str);
	exit(EXIT_FAILURE);
}

int main(void)
{
	if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket");
		return EXIT_FAILURE;
	}

	struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
    
    char full_path[BUF_SIZE] = {0};
    strcpy(full_path, SOCKET_PATH);
    strcat(full_path, SOCKET_NAME);
    strcpy(addr.sun_path, full_path);
    
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		perror_exit("bind");

	if (signal(SIGINT, sighandler) == SIG_ERR)
		perror_exit("signal");
    
    char msg[BUF_SIZE];
    char reply[BUF_SIZE];

	while (1)
	{
        struct sockaddr_un cl_addr;
        int len;
		size_t bytes = recvfrom(sockfd, msg, sizeof(msg) - 1, 0, (struct sockaddr *)&cl_addr, (socklen_t*)&len);
		if (bytes == -1)
			perror_exit("recvfrom");
        
		msg[bytes] = '\0';
		const time_t timer = time(NULL);
		printf("[%.19s] received message from %s: %s\n", ctime(&timer), cl_addr.sun_path, msg);
        
        strcpy(reply, msg);
        strcat(reply, "//ReplySuccess");
        if (sendto(sockfd, reply, strlen(reply), 0, (struct sockaddr *)&cl_addr, len) == -1)
            perror_exit("sendto");
	}
}
