#include "apue.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>>
#include <sys/stat.h>>
#include <unistd.h>
#include <pthread.h>
#include <time.h>>

#include <sys/time.h>
#include <sys/resource.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

sigset_t mask;

int lockfile(int fd)
{
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0; // смещение
	fl.l_whence = SEEK_SET; // с начала файла
	fl.l_len = 0; // до конца файла
	return (fcntl(fd, F_SETLK, &fl));
}


int already_running(void)
{
	int fd;
	char buf[16];

	fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
	if (fd < 0)
	{
		syslog(LOG_ERR, "невозможно открыть %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
	if (lockfile(fd) < 0)
	{
		if (errno == EACCES || errno == EAGAIN)
		{
			close(fd);
			return (1);
		}
		syslog(LOG_ERR, "невозможно установить блокировку на %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf) + 1);
	return (0);
}

void daemonize(const char *cmd)
{
	int i, fd0, fd1, fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

	umask(0); // Сбросить маску режима создания файла

	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		err_quit("%s: невозможно получить максимальный номер дескриптора ", cmd);

	if ((pid = fork()) < 0)
		err_quit("%s: ошибка вызова функции fork", cmd);
	else if (pid != 0)
		exit(0); // завершить родительский процесс
	setsid();

	// Обеспечить невозможность обретения управляющего терминала в будущем.
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		err_quit("%s: невозможно игнорировать сигнал SIGHUP", cmd);

	// ! Демон не является лидером сеанса, а поэтому не имеет возможности обрести управляющий терминал.!
	// if ((pid = fork()) < 0)
	// err_quit("%s: ошибка вызова функции fork", cmd);
	// else if (pid != 0) // родительский процесс
	// exit(0);

	if (chdir("/") < 0)
		err_quit("%s: невозможно сделать текущим рабочим каталогом /", cmd);
    
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (int i = 0; i < rl.rlim_max; i++)
		close(i);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	//Инициализировать файл журнала
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "ошибочные файловые дескрипторы %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}

void *thr_fn(void *arg)
{
	int err, signo;
	for (;;)
	{
		err = sigwait(&mask, &signo);
		if (err)
		{
			syslog(LOG_ERR, "ошибка вызова функции sigwait");
			exit(1);
		}
		switch (signo)
		{
		case SIGHUP:
			syslog(LOG_INFO, "Получен сигнал SIGHUP getlogin = %s", getlogin());
			break;
		case SIGTERM:
			syslog(LOG_INFO, "Получен сигнал SIGTERM; выход");
			exit(0);
		default:
			syslog(LOG_INFO, "получен непредвиденный сигнал %d\n", signo);
		}
	}
	return (0);
}

int main(int argc, char *argv[])
{
	int err;
	pthread_t tid;
	char *cmd;
	struct sigaction sa;

	// char *strrchr(const char *s, int c);
	// if ((cmd = strrchr(argv[0], '/')) == NULL)
	// cmd = argv[0];
	// else
	// cmd++;
	// daemonize(cmd);

	daemonize("my_daemon");
	
	if (already_running())
	{
		syslog(LOG_ERR, "Демон уже запущен!\n");
		exit(1);
	}

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		err_quit("%s: невозможно восстановить действие SIG_DFL для SIGHUP");
	sigfillset(&mask);

	if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
		err_exit(err, "ошибка выполнения операции SIG_BLOCK");

	err = pthread_create(&tid, NULL, thr_fn, 0);
	if (err)
		err_exit(err, "невозможно создать поток");

	syslog(LOG_WARNING, "Проверка пройдена!");

	long int ttime;
	ttime = time(NULL);

	while (1)
	{
		syslog(LOG_INFO, "Демон! Время: %s", ctime(&ttime));
		sleep(3);
	}
}