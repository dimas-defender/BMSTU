#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>

#define SOCKET_NAME "socket.soc"
#define SOCKET_PATH "/home/dmitriy/OS/lab8/unix/"
#define CLIENT_PREFIX ".client"
#define BUF_SIZE 256

static int sockfd;

void cleanup(void)
{
	close(sockfd);
    char full_path[BUF_SIZE] = {0};
    strcpy(full_path, SOCKET_PATH);
    strcat(full_path, SOCKET_NAME);
    strcat(full_path, CLIENT_PREFIX);
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
	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sockfd == -1)
	{
		perror("socket(AF_UNIX, SOCK_DGRAM, 0)");
		return EXIT_FAILURE;
	}
	
	if (signal(SIGINT, sighandler) == SIG_ERR)
		perror_exit("signal");
    
    struct sockaddr_un cl_addr;
	cl_addr.sun_family = AF_UNIX;
    
    char full_path_client[BUF_SIZE] = {0};
    strcpy(full_path_client, SOCKET_PATH);
    strcat(full_path_client, SOCKET_NAME);
    strcat(full_path_client, CLIENT_PREFIX);
    strcpy(cl_addr.sun_path, full_path_client);
    
    if (bind(sockfd, (struct sockaddr *)&cl_addr, sizeof(cl_addr)) == -1)
		perror_exit("bind");
    
    struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	
    char full_path[BUF_SIZE] = {0};
    strcpy(full_path, SOCKET_PATH);
    strcat(full_path, SOCKET_NAME);
    strcpy(addr.sun_path, full_path);

	char msg[BUF_SIZE];
	snprintf(msg, BUF_SIZE, "pid %d", getpid());
	if (sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		perror_exit("sendto");
    
    time_t timer = time(NULL);
    printf("[%.19s] sent message: %s\n", ctime(&timer), msg);
        
    char reply[BUF_SIZE] = {0};
    socklen_t len;
    size_t bytes = recvfrom(sockfd, reply, sizeof(reply), 0, (struct sockaddr *)&addr, &len);
    if (bytes == -1)
        perror_exit("recvfrom");
    
    msg[bytes] = '\0';
    timer = time(NULL);
    printf("[%.19s] received message: %s\n", ctime(&timer), reply);
    sleep(5);
	cleanup();
	return 0;
}
