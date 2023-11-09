#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

void catalog(char *pathname);

typedef struct
{
    char *name;
    int depth;
} Path;

typedef struct
{
    int size;
    Path **path_arr;
} Stack;

Path *create_path(char *name, int depth);
void free_path(Path *path);
void expand_path(Path *path, char *filename, int cur_len);
void print_path(ino_t inode, char *name, int depth, const char symbol);
void print_pathh(char *name, int depth, const char symbol);

Stack *create_stack();
void free_stack(Stack *stack);
void push(Stack *stack, char *name, int depth);
Path *pop(Stack *stack);

void err_sys(const char *msg);
void error_lstat();


int main(int argc, char *argv[])
{
    if (argc != 2)
        err_sys("Укажите каталог в качестве аргумента командной строки!");
    catalog(argv[1]);
    printf("\nSUCCESS\n");
    return 0;
}

void catalog(char *pathname)
{
    Stack *stack = create_stack();
    Path *cur_path;
    int cur_len;
    char *name;

    struct stat statbuf;
    DIR *dir;
    struct dirent *entry;

    if (lstat(pathname, &statbuf) == -1)
        error_lstat();
    if (!S_ISDIR(statbuf.st_mode))
        err_sys("Введенный путь не является каталогом.");

    push(stack, pathname, 1);

    while (stack->size)
    {
        cur_path = pop(stack);

        if ((name = strrchr(cur_path->name, '/')) == NULL)
            name = cur_path->name;
        else
            name++;

        print_pathh(name, cur_path->depth - 1, '-');

        dir = opendir(cur_path->name);
        if (!dir)
            err_sys("Ошибка opendir");

        cur_len = strlen(cur_path->name);
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 &&
                strcmp(entry->d_name, "..") != 0)
            {
                expand_path(cur_path, entry->d_name, cur_len);

                if (lstat(cur_path->name, &statbuf) == -1)
                    error_lstat();

                if (S_ISDIR(statbuf.st_mode))
                    push(stack, cur_path->name, cur_path->depth + 1);
                else
                    print_path(entry->d_ino, entry->d_name, cur_path->depth, '-');
            }
        }
        if (closedir(dir) < 0)
            exit(1);
    }
    free_stack(stack);
}


// PATH

Path *create_path(char *name, int depth)
{
    Path *res = (Path *)malloc(sizeof(Path));
    res->name = malloc(sizeof(char) * strlen(name) + 1);
    strcpy(res->name, name);
    res->depth = depth;
    return res;
}

void free_path(Path *path)
{
    if (path->name)
        free(path->name);
    if (path)
        free(path);
}

void expand_path(Path *path, char *filename, int cur_len)
{
    path->name = (char *)realloc(path->name, cur_len + strlen(filename) + 2);
    path->name[cur_len] = '/';
    strcpy(&path->name[cur_len + 1], filename);
}

void print_path(ino_t inode, char *name, int depth, const char symbol)
{
    for (int i = 0; i < depth * 4; i++)
        putchar(symbol);
    printf(" %ld %s\n", inode, name);
}

void print_pathh(char *name, int depth, const char symbol)
{
    for (int i = 0; i < depth * 4; i++)
        putchar(symbol);
    printf(" %s\n", name);
}


// STACK

Stack *create_stack()
{
    Stack *res = (Stack *)malloc(sizeof(Stack));
    res->size = 0;
    res->path_arr = NULL;
    return res;
}

void free_stack(Stack *stack)
{
    for (int i = 0; i < stack->size; i++)
        free_path(stack->path_arr[i]);
    if (stack->path_arr)
        free(stack->path_arr);
    if (stack)
        free(stack);
}

void push(Stack *stack, char *name, int depth)
{
    (stack->size)++;
    if (stack->path_arr)
        stack->path_arr = realloc(stack->path_arr, sizeof(Path *) * stack->size);
    else
        stack->path_arr = malloc(sizeof(Path *));

    stack->path_arr[stack->size - 1] = create_path(name, depth);
}

Path *pop(Stack *stack)
{
    if (stack->size < 1)
        return NULL;

    Path *res = stack->path_arr[stack->size - 1];
    (stack->size)--;
    stack->path_arr = realloc(stack->path_arr, sizeof(Path *) * stack->size);
    return res;
}


// ERRORS

void err_sys(const char *msg)
{
    perror(msg);
    exit(1);
}

void error_lstat()
{
    switch (errno)
    {
    case EBADF:
        err_sys("Неверный файловый описатель filedes.");
        break;
    case ENOENT:
        err_sys("Компонент полного имени файла file_name не существует или полное имя является пустой строкой.");
        break;
    case ENOTDIR:
        err_sys("Компонент пути не является каталогом.");
        break;
    case ELOOP:
        err_sys("При поиске файла встретилось слишком много символьных ссылок.");
        break;
    case EFAULT:
        err_sys("Некорректный адрес.");
        break;
    case EACCES:
        err_sys("Запрещен доступ.");
        break;
    case ENOMEM:
        err_sys("Недостаточно памяти в системе.");
        break;
    case ENAMETOOLONG:
        err_sys("Слишком длинное имя файла.");
        break;
    default:
        err_sys("Ошибка lstat.");
        break;
    }
}
