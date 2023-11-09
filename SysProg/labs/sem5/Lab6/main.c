#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <memory.h>
#include <time.h>

#define MYSUCCESS 0
#define MYERROR -1
#define NREADERS 5
#define NWRITERS 3
#define NITER 10
#define RAND_MAX_SLEEP 500

LONG active_readers = 0;
LONG waiting_readers = 0;
LONG waiting_writers = 0;
bool active_writer = false;
int counter = 0;

HANDLE mutex;
HANDLE can_read;
HANDLE can_write;

void start_write()
{
	InterlockedIncrement(&waiting_writers);
	if (active_readers > 0 || active_writer)
		WaitForSingleObject(can_write, INFINITE);
	InterlockedDecrement(&waiting_writers);
	active_writer = true;
}

void stop_write()
{
	active_writer = false;
	if (waiting_readers) 
		SetEvent(can_read);
	else 
		SetEvent(can_write);
}

void start_read()
{
	InterlockedIncrement(&waiting_readers);
	if (active_writer || waiting_writers > 0)
		WaitForSingleObject(can_read, INFINITE);
	WaitForSingleObject(mutex, INFINITE);
	InterlockedDecrement(&waiting_readers);
	InterlockedIncrement(&active_readers);
	SetEvent(can_read);
	ReleaseMutex(mutex);
}

void stop_read()
{
	InterlockedDecrement(&active_readers);
	if (waiting_readers == 0)
	{
		ResetEvent(can_read);
		SetEvent(can_write);
	}
}

DWORD WINAPI write(CONST LPVOID lpParams)
{
	srand(time(NULL) + (int)lpParams);
	for (int i = 0; i < NITER; i++)
	{
		Sleep(rand() % RAND_MAX_SLEEP + 100);
		start_write();
		counter++;
		printf("%d wrote %d\n", (int)lpParams, counter);
		stop_write();
	}
	return MYSUCCESS;
}

DWORD WINAPI read(CONST LPVOID lpParams)
{
	srand(time(NULL) + (int)lpParams);
	for (int i = 0; i < NITER; i++)
	{
		Sleep(rand() % RAND_MAX_SLEEP + 100);
		start_read();
		printf("%d read %d\n", (int)lpParams, counter);
		stop_read();
	}
	return MYSUCCESS;
}

int main()
{
	setbuf(stdout, NULL);
	
	if ((mutex = CreateMutex(NULL, FALSE, NULL)) == NULL)
	{
		perror("Create mutex\n");
		return MYERROR;
	}
	if ((can_read = CreateEvent(NULL, TRUE, TRUE, NULL)) == NULL)
	{
		perror("Create can_read\n");
		return MYERROR;
	}
	if ((can_write = CreateEvent(NULL, FALSE, TRUE, NULL)) == NULL)
	{
		perror("Create can_write\n");
		return MYERROR;
	}

	HANDLE *writers = (HANDLE *)malloc(NWRITERS * sizeof(HANDLE));
	if (!writers)
	{
		perror("malloc writers\n");
		return MYERROR;
	}
	for (int i = 0; i < NWRITERS; i++)
	{
		writers[i] = CreateThread(NULL, 0, write, (LPVOID)i, 0, NULL);
		if (!writers[i])
		{
			free(writers);
			perror("CreateThread writer\n");
			return MYERROR;
		}
	}

	HANDLE *readers = (HANDLE *)malloc(NREADERS * sizeof(HANDLE));
	if (!readers)
	{
		perror("malloc readers\n");
		return MYERROR;
	}
	for (int i = 0; i < NREADERS; i++)
	{
		readers[i] = CreateThread(NULL, 0, read, (LPVOID)i, 0, NULL);
		if (!readers[i])
		{
			free(readers);
			perror("CreateThread reader\n");
			return MYERROR;
		}
	}

	WaitForMultipleObjects(NWRITERS, writers, TRUE, INFINITE);
	WaitForMultipleObjects(NREADERS, readers, TRUE, INFINITE);

	CloseHandle(mutex);
	CloseHandle(can_read);
	CloseHandle(can_write);

	free(readers);
	free(writers);

	return MYSUCCESS;
}