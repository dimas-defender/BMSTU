#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <linux/limits.h>

#define SUCCESS 0
#define ERROR -1

#define BUF_SIZE 0x100

void print_jump();
void print_environ(char *pid);
void print_stat(char *pid);
void print_statm(char *pid);
void print_cmdline(char *pid);
void print_fd(char *pid);
void print_cwd(char *pid);
void print_exe(char *pid);
void print_root(char *pid);
void print_maps(char *pid);
void print_io(char *pid);
void print_comm(char *pid);
void print_task(char *pid);
void print_smaps(char *pid);
void print_wchan(char *pid);
void print_pagemap(char *pid);

int main(int argc, char *argv[])
{
    int rc = SUCCESS;
    if (argc != 2)
        printf("Pass the PID as a command line argument.\n");
    else
    {
        print_jump();
        print_environ(argv[1]);
        print_jump();
        print_stat(argv[1]);
        print_jump();
        print_statm(argv[1]);
        print_jump();
        print_cmdline(argv[1]);
        print_jump();
        print_fd(argv[1]);
        print_jump();
        print_cwd(argv[1]);
        print_jump();
        print_exe(argv[1]);
        print_jump();
        print_root(argv[1]);
        print_jump();
        print_maps(argv[1]);
        print_jump();
        print_io(argv[1]);
        print_jump();
        print_comm(argv[1]);
        print_jump();
        print_task(argv[1]);
        print_jump();
        print_smaps(argv[1]);
        print_jump();
        print_wchan(argv[1]);
        print_jump();
        print_pagemap(argv[1]);
        print_jump();
    }
    return rc;
}

void print_jump()
{
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

void print_environ(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/environ", pid);
    printf("%s:\n\n", path);
    FILE *f = fopen(path, "r");
    int len;
    char buf[BUF_SIZE];
    while ((len = fread(buf, 1, BUF_SIZE, f)) > 0)
    {
        for (int i = 0; i < len; i++)
            if (buf[i] == 0)
                buf[i] = 10;
        buf[len - 1] = 0;
        printf("%s\n", buf);
    }
    fclose(f);
}

void print_stat(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/stat", pid);
    printf("%s:\n\n", path);
    FILE *f = fopen(path, "r");
    char buf[BUF_SIZE];
    fread(buf, 1, BUF_SIZE, f);
    char *pch = strtok(buf, " ");
    int i = 0;
    char *comment[] = {
        "pid - ID процесса",
        "comm - Имя файла",
        "state - Состояние процесса",
        "ppid - ID родительского процесса",
        "pgrp - ID группы процесса",
        "session - ID сессии процесса",
        "tty_nr - управляющий терминал процесса",
        "tpgid - ID внешней группы процессов управляющего терминала",
        "flags - Флаги ядра процесса",
        "minflt - Количество минорных ошибок процесса (Минорные ошибки не включают ошибки загрузки страниц памяти с "
        "диска)",
        "cminflt - Количество минорных ошибок дочерних процессов (Минорные ошибки не включают ошибки загрузки страниц "
        "памяти с диска)",
        "majflt - Количество Мажорных ошибок процесса",
        "cmajflt - Количество Мажорных ошибок дочерних процессов процесса",
        "utime - Количество времени, в течение которого этот процесс исполнялся в пользовательском режиме",
        "stime - Количество времени, в течение которого этот процесс исполнялся в режиме ядра",
        "cutime - Количество времени, в течение которого дети этого процесса исполнялись в "
        "пользовательском режиме",
        "cstime - Количество времени, в течение которого дети этого процесса были запланированы в режиме "
        "ядра",
        "priority - Приоритет процесса",
        "nice - уровень nice процесса, то есть пользовательский приоритет процесса (-20 - высокий, 19 - низкий)",
        "num_threads - Количество потоков",
        "itrealvalue - Время в тиках до следующего SIGALRM отправленного в процесс из-за интервального таймера",
        "starttime - Время, прошедшее с момента загрузки системы, когда процесс был запущен",
        "vsize - Объем виртуальной памяти в байтах",
        "rss - Resident Set (Memory) Size: Количество страниц процесса в физической памяти",
        "rsslim - Текущий предел в байтах для RSS процесса",
        "startcode - Адрес, над которым может работать текст программы",
        "endcode - Адрес, до которого может работать текст программы",
        "startstack - Адрес начала стека",
        "kstkesp - Текущее значение ESP (Stack pointer)",
        "kstkeip - Текущее значение EIP (instruction pointer)",
        "pending - Битовая карта отложенных сигналов, отображаемое в виде десятичного числа",
        "blocked - Битовая карта заблокированных сигналов, отображаемое в виде десятичного числа",
        "sigign - Битовая карта игнорированных сигналов, отображаемое в виде десятичного числа",
        "sigcatch - Битовая карта пойманных сигналов, отображаемое в виде десятичного числа",
        "wchan - Адрес ядрёной функции, где процесс находится в состоянии сна",
        "nswap - Количество страниц, поменявшихся местами",
        "cnswap - Накопительный своп для дочерних процессов",
        "exit_signal - Сигнал, который будет послан родителю, когда процесс будет завершен",
        "processor - Номер процессора, на котором происходило последнее выполнение",
        "rt_priority - Приоритет планирования в реальном времени - число в диапазоне от 1 до 99 для процессов, "
        "запланированных в соответствии с политикой реального времени",
        "policy - Политика планирования",
        "blkio_ticks - Общие блочные задержки ввода/вывода",
        "gtime - Гостевое время процесса",
        "cgtime - Гостевое время дочерних процессов",
        "start_data - Адрес, над которым размещаются инициализированные и неинициализированные данные программы (BSS)",
        "end_data - Адрес, под которым размещаются инициализированные и неинициализированные данные программы (BSS)",
        "start_brk - Адрес, выше которого куча программ может быть расширена с помощью brk (станавливает конец "
        "сегмента данных в значение, указанное в аргументе end_data_segment, когда это значение является приемлимым, "
        "система симулирует нехватку памяти и процесс не достигает своего максимально возможного размера сегмента "
        "данных)",
        "arg_start - Адрес, над которым размещаются аргументы командной строки программы (argv)",
        "arg_end - Адрес, под которым размещаются аргументы командной строки программы (argv)",
        "env_start - Адрес, над которым размещена программная среда",
        "env_end - Адрес, под которым размещена программная среда",
        "exit_code - Состояние выхода потока в форме, сообщаемой waitpid"};
    while(pch != NULL)
    {
        printf("%s %s\n", pch, comment[i++]);
        pch = strtok(NULL, " ");
    }
    fclose(f);
}

void print_statm(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/statm", pid);
    printf("%s:\n\n", path);
    FILE *f = fopen(path, "r");
    char buf[BUF_SIZE];
    fread(buf, 1, BUF_SIZE, f);
    char *pch = strtok(buf, " ");
    int i = 0;
    char *comment[] = {"size - общее число страниц выделенное процессу в виртуальной памяти",
                             "resident - количество страниц, загруженных в физическую память",
                             "shared - количество общих резедентных страниц",
                             "trs - количество страниц кода",
                             "lrs - количество страниц библиотеки",
                             "drs - количество страниц данных/стека",
                             "dt - dirty pages - данные для записи находятся в кэше страниц, но требуется к "
                             "первоначальной записи на носитель"};
    while(pch != NULL)
    {
        printf("%s %s\n", pch, comment[i++]);
        pch = strtok(NULL, " ");
    }
    fclose(f);
}

void print_cmdline(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/cmdline", pid);
    printf("%s:\n\n", path);
    FILE *f = fopen(path, "r");
    int len;
    char buf[BUF_SIZE];
    while ((len = fread(buf, 1, BUF_SIZE, f)) > 0)
    {
        for (int i = 0; i < len; i++)
            if (buf[i] == 0)
                buf[i] = ' ';
        buf[len - 1] = 0;
        printf("%s\n", buf);
    }
    fclose(f);
}

void print_fd(char *pid)
{
    char path1[50];
    snprintf(path1, 50, "/proc/%s/fd/", pid);
    printf("%s:\n\n", path1);
    struct dirent *dirp;
    DIR *dp;
    char str[BUF_SIZE+10];
    char path[BUF_SIZE+10];
    path[BUF_SIZE+9] = '\0';
    dp = opendir(path1);
    while ((dirp = readdir(dp)) != NULL)
    {
        if ((strcmp(dirp->d_name, ".") != 0) && (strcmp(dirp->d_name, "..") != 0))
        {
            sprintf(path, "%s%s", path1, dirp->d_name);
            readlink(path, str, BUF_SIZE);
            printf("%s -> %s\n", dirp->d_name, str);
        }
    }
    closedir(dp);
}

void print_cwd(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/cwd", pid);
    char str[BUF_SIZE] = {'\0'};
    readlink(path, str, BUF_SIZE);
    printf("%s -> %s\n", path, str);
}

void print_exe(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/exe", pid);
    char str[BUF_SIZE] = {'\0'};
    readlink(path, str, BUF_SIZE);
    printf("%s -> %s\n", path, str);
}

void print_root(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/root", pid);
    char str[BUF_SIZE] = {'\0'};
    readlink(path, str, BUF_SIZE);
    printf("%s -> %s\n", path, str);
}

void print_maps(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/maps", pid);
    printf("%s:\n\n", path);
    FILE *f = fopen(path, "r");
    char c, buf[BUF_SIZE];
    int i = 0;
    while ((c = getc(f)) != EOF)
    {
        buf[i++] = c;
        if (c == '\n')
        {
            buf[i] = '\0';
            i = 0;
            printf("%s", buf);
        }
    }
    fclose(f);
}

void print_io(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/io", pid);
    printf("%s:\n\n", path);
    char *comment[] = {"символы для чтения - количество байтов, которые этот процесс запрашивал считать из хранилища (сумма байтов, которые процесс запрашивал у read(2) и аналогичных системных вызовов)",
    "символы для записи - количество байтов, которые этот процесс передал или должен передать для записи",
    "количество системных вызовов чтения (read(2), pread(2) и т.д.)",
    "количество системных вызовов записи (write(2), pwrite(2) и т.д.)",
    "прочитанные байты - количество байтов, которые этот процесс действительно считал",
    "записанные байты - количество байтов, которые этот процесс действительно записал",
    "байты, отмененные на запись - (дело в усечении). Если процесс записывает 1 МБ в файл, а затем удаляет файл, он фактически не будет выполнять запись. Но это будет учтено как отправка 1 МБ для записи. Другими словами: это поле представляет количество байтов для записи, которые не были фактически записаны из-за усечения кэша. Процесс может привести к 'отрицательному' вводу-выводу. Если процесс усекает некоторый 'грязный' кэш страниц, некоторый ввод-вывод, который был учтен другой задачей (в ее write_bytes), не будет выполняться."};
    FILE *f = fopen(path, "r");
    char c, buf[BUF_SIZE];
    int i = 0, ind = 0;
    while ((c = getc(f)) != EOF)
    {
        buf[i++] = c;
        if (c == '\n')
        {
            buf[i-1] = '\0';
            i = 0;
            printf("%s %s\n", buf, comment[ind++]);
        }
    }
    fclose(f);
}

void print_comm(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/comm", pid);
    printf("%s:\n\n", path);
    FILE *f = fopen(path, "r");
    char c, buf[BUF_SIZE];
    int i = 0;
    while ((c = getc(f)) != EOF)
    {
        buf[i++] = c;
        if (c == '\n')
        {
            buf[i] = '\0';
            i = 0;
            printf("%s", buf);
        }
    }
    fclose(f);
}

void print_task(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/task/", pid);
    printf("%s:\n\n", path);
    struct dirent *dirp;
    DIR *dp;
    dp = opendir(path);
    while ((dirp = readdir(dp)) != NULL)
    {
        if ((strcmp(dirp->d_name, ".") != 0) && (strcmp(dirp->d_name, "..") != 0))
        {
            char path1[PATH_MAX];
            snprintf(path1, PATH_MAX, "/proc/%s/task/%s/comm", pid, dirp->d_name);
            printf("%s:\n", path1);
            FILE *f = fopen(path1, "r");
            char c, buf[BUF_SIZE];
            int i = 0;
            while ((c = getc(f)) != EOF)
            {
                buf[i++] = c;
                if (c == '\n')
                {
                    buf[i] = '\0';
                    i = 0;
                    printf("%s\n", buf);
                }
            }
            fclose(f);
        }
    }
    closedir(dp);
}

void print_smaps(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/smaps", pid);
    printf("%s:\n\n", path);
    FILE *f = fopen(path, "r");
    char c, buf[BUF_SIZE];
    buf[0] = '\0';
    int i = 0;
    while ((c = getc(f)) != EOF)
    {
        buf[i++] = c;
        if (c == '\n')
        {
            buf[i] = '\0';
            i = 0;
            printf("%s", buf);
        }
    }
    fclose(f);
}

void print_wchan(char *pid)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/wchan", pid);
    printf("%s:\n\n", path);
    FILE *f = fopen(path, "r");
    char c, buf[BUF_SIZE];
    buf[0] = '\0';
    int i = 0;
    while ((c = getc(f)) != EOF)
    {
        buf[i++] = c;
        if (c == '\n')
        {
            buf[i] = '\0';
            i = 0;
            printf("%s", buf);
        }
    }
    fclose(f);
}

typedef struct {
    uint64_t pfn : 54; //номер фрейма страницы
    unsigned int soft_dirty : 1; //мягкое загрязнение
    unsigned int file_page : 1; //страница сопоставлена с файлом 
    unsigned int swapped : 1; //находится в разделе подкачки
    unsigned int present : 1; //страница присутствует в оперативной памяти
} PagemapEntry;

int pagemap_get_entry(PagemapEntry *entry, int pagemap_fd, uintptr_t vaddr)
{
    size_t nread;
    ssize_t ret;
    uint64_t data;

    nread = 0;
    while (nread < sizeof(data)) {
        ret = pread(pagemap_fd, ((uint8_t*)&data) + nread, sizeof(data) - nread,
                (vaddr / sysconf(_SC_PAGE_SIZE)) * sizeof(data) + nread);
        nread += ret;
        if (ret <= 0) {
            return 1;
        }
    }
    entry->pfn = data & (((uint64_t)1 << 55) - 1);
    entry->soft_dirty = (data >> 55) & 1;
    entry->file_page = (data >> 61) & 1;
    entry->swapped = (data >> 62) & 1;
    entry->present = (data >> 63) & 1;
    return 0;
}

void print_pagemap(char *pid)
{
    char buffer[BUFSIZ];
    char maps_file[BUFSIZ];
    char pagemap_file[BUFSIZ];
    int maps_fd;
    int offset = 0;
    int pagemap_fd;

    snprintf(maps_file, sizeof(maps_file), "/proc/%s/maps", pid);
    snprintf(pagemap_file, sizeof(pagemap_file), "/proc/%s/pagemap", pid);
    maps_fd = open(maps_file, O_RDONLY);
    if (maps_fd < 0)
    {
        printf("error opening maps");
    }
    pagemap_fd = open(pagemap_file, O_RDONLY);
    if (pagemap_fd < 0)
    {
        printf("error opening pagemap");
    }

    printf("%s:\n\n", pagemap_file);
    //printf("Файл pagemap: отображение каждой из виртуальных страниц процесса в фреймы физических страниц или область подкачки.\n");
    printf("addr pfn(page frame number) soft-dirty file/shared swapped present library\n\n");

    for (;;) {
        ssize_t length = read(maps_fd, buffer + offset, sizeof buffer - offset);
        if (length <= 0) break;
        length += offset;
        for (size_t i = offset; i < (size_t)length; i++) {
            uintptr_t low = 0, high = 0;
            if (buffer[i] == '\n' && i) {
                const char *lib_name;
                size_t y;
                {
                    size_t x = i - 1;
                    while (x && buffer[x] != '\n') x--;
                    if (buffer[x] == '\n') x++;
                    while (buffer[x] != '-' && x < sizeof buffer)
					{
                        char c = buffer[x++];
                        low *= 16;
                        if (c >= '0' && c <= '9')
                            low += c - '0';
						else if (c >= 'a' && c <= 'f')
                            low += c - 'a' + 10;
						else
                            break;
                    }
                    while (buffer[x] != '-' && x < sizeof buffer)
						x++;
                    if (buffer[x] == '-')
						x++;
                    while (buffer[x] != ' ' && x < sizeof buffer)
					{
                        char c = buffer[x++];
                        high *= 16;
                        if (c >= '0' && c <= '9')
                            high += c - '0';
						else if (c >= 'a' && c <= 'f')
                            high += c - 'a' + 10;
						else
                            break;
                    }
                    lib_name = 0;
                    for (int field = 0; field < 4; field++)
					{
                        x++;
                        while(buffer[x] != ' ' && x < sizeof buffer)
							x++;
                    }
                    while (buffer[x] == ' ' && x < sizeof buffer)
						x++;
                    y = x;
                    while (buffer[y] != '\n' && y < sizeof buffer)
						y++;
                    buffer[y] = 0;
                    lib_name = buffer + x;
                }
                {
                    PagemapEntry entry;
                    for (uintptr_t addr = low; addr < high; addr += sysconf(_SC_PAGE_SIZE))
					{
                        if (!pagemap_get_entry(&entry, pagemap_fd, addr))
						{
                            printf("%jx %jx %u %u %u %u %s\n",
                                (uintmax_t)addr,
                                (uintmax_t)entry.pfn,
                                entry.soft_dirty,
                                entry.file_page,
                                entry.swapped,
                                entry.present,
                                lib_name);
                        }
                    }
                }
                buffer[y] = '\n';
            }
        }
    }
    close(maps_fd);
    close(pagemap_fd);
}

void print_page(uint64_t address, uint64_t data, FILE *out)
{
	fprintf(out, "0x%-16lx : %-16lx %-10ld %-10ld %-10ld %-10ld\n", address,
	data & (((uint64_t)1 << 55) - 1), (data >> 55) & 1, (data >> 61) & 1,
	(data >> 62) & 1, (data >> 63) & 1);
}

void get_pagemap_info(const char *proc, FILE *out)
{
	fprintf(out, "PAGEMAP\n");
	fprintf(out, " addr : pfn soft-dirty file/shared swapped present\n");
	
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%s/maps", proc);
	FILE *maps = fopen(path, "r");
	
	snprintf(path, PATH_MAX, "/proc/%s/pagemap", proc);
	int pm_fd = open(path, O_RDONLY);
	
	char buf[BUF_SIZE + 1] = "\0";
	int len;
	
	// чтение maps
	while ((len = fread(buf, 1, BUF_SIZE, maps)) > 0)
	{
		for (int i = 0; i < len; ++i)
			if (buf[i] == 0) buf[i] = '\n ';
		buf[len] = '\0 ';

	// проход по строкам из maps
	// используем strtok_r вместо strtok для возможности разбиения на лексемы разных буферов
		char *save_row;
		char *row = strtok_r(buf, "\n", &save_row);

		while (row)
		{
			// получение столбца участка адресного пространства
			char *addresses = strtok(row, " ");
			char *start_str, *end_str;

			// получение начала и конца участка адресного пространства
			if ((start_str = strtok(addresses, "-")) && (end_str = strtok(NULL, "-")))
			{
				uint64_t start = strtoul(start_str, NULL, 16);
				uint64_t end = strtoul(end_str, NULL, 16);

				for (uint64_t i = start; i < end; i += sysconf(_SC_PAGE_SIZE))
				{
					uint64_t offset;
					// поиск смещения, по которому в pagemap находится информация о текущей странице
					uint64_t index = i / sysconf(_SC_PAGE_SIZE) * sizeof(offset);

					pread(pm_fd, &offset, sizeof(offset), index);
					print_page(i, offset, out);
				}
			}
			row = strtok_r(NULL, "\n", &save_row);
		}
	}
	fclose(maps);
	close(pm_fd);
}