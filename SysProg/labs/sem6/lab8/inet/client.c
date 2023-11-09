#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define SOCKET_PORT 8888
#define BUF_SIZE 256

int main(void)
{
	const int master_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (master_sd == -1)
	{
		perror("socket");
		return EXIT_FAILURE;
	}

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons(SOCKET_PORT)
	};

	if (connect(master_sd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("connect");
		return EXIT_FAILURE;
	}
	
	char msg[BUF_SIZE];
	do {
		snprintf(msg, BUF_SIZE, "pid %d", getpid());
		if (send(master_sd, msg, strlen(msg), 0) == -1)
		{
			perror("sendto");
			return EXIT_FAILURE;
		}
		const time_t timer = time(NULL);
		printf("[%.19s] sent message: %s\n", ctime(&timer), msg);
		printf("Press Enter to resend message...");
		getchar();
	} while (1);
}
