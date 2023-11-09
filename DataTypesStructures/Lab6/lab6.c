#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STR_SIZE 128
#define SPACING 10
#define NOT_FOUND 0
#define OK 0
#define FILE_ERROR 1
#define VAL_ERROR 2

typedef struct tree_node
{
    char *val;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

typedef struct
{
    tree_node **arr;
    int size;
    int mem_size;
} dynarr_t;

typedef struct list
{
    char *value;
    struct list *next;
} list_t;

unsigned long long tick(void);
char **fill_words(FILE *f);
void free_words(char **words, unsigned long flen);
int search_tree(char *to_find, tree_node root);
double search_tree_avg(tree_node root, char **words, unsigned long flen, double *cmp_avg);
int search_hashtable(char *to_find, list_t *hash_list, int n, int (*hash)(char *, int));
double search_hashtable_avg(list_t *hash_list, int n, int (*hash)(char *, int), char **words, unsigned long flen, double *cmp_avg);
int search_file(char *to_find, FILE *f);
double search_file_avg(FILE *f, char **words, unsigned long flen);
tree_node *create_node(char *val);
tree_node *insert(tree_node *node, char *val);
void fill_tree(tree_node **root, FILE *f);
void print_tree(tree_node *root, int space);
void push_back(dynarr_t *arr, tree_node *to_add);
void store_nodes(tree_node *root, dynarr_t *arr);
tree_node *build_tree(dynarr_t *arr, int start, int end);
tree_node *balance_tree(tree_node *root, int *unique);
void free_tree(tree_node *node);
int is_prime(unsigned int num);
unsigned int next_prime(int n);
int file_len(FILE *f);
int fbytes(FILE *f);
int compl_hash(char *s, int n);
int simple_hash(char *s, int n);
int build_hash_table(list_t **arr, int size, FILE *f, int (*hash)(char *, int));
void print_hash_table(list_t *arr, int size);
int linsert(list_t *arr, char *buffer);
int deepcmp(char *s, list_t l);
int list_filling(list_t *arr, int size);
void free_list(list_t *node);
void free_list_arr(list_t *arr, int size);

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
    printf("%s",
           "Данная программа позволяет построить ДДП, сбалансированное двоичное дерево \n"
           "и хеш-таблицу по данным из файла, вывести на экран построенные деревья, \n"
           "провести реструктуризацию хеш-таблицы, если количество сравнений больше указанного,\n"
           "вывести информацию для сравнения эффективности используемых структур данных по времени и памяти.\n\n");
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        printf("Неверное имя файла!\n");
        return FILE_ERROR;
    }
    if (fgetc(f) == EOF)
    {
        printf("Файл пуст!\n");
        return FILE_ERROR;
    }
    rewind(f);

    char **words = fill_words(f);
    unsigned long flen = file_len(f);
    tree_node *root = NULL;

    unsigned long time = tick();
    fill_tree(&root, f);
    time = tick() - time;

    printf("ДВОИЧНОЕ ДЕРЕВО ПОИСКА:\n");
    print_tree(root, 0);
    printf("\nДерево построено за = %ld тактов процессора (с учетом времени чтения из файла).\n\n", time);
    int unique;
    time = tick();
    root = balance_tree(root, &unique);
    time = tick() - time;
    printf("СБАЛАНСИРОВАННОЕ ДЕРЕВО:\n");
    print_tree(root, 0);
    printf("\nДерево перестроено за = %ld тактов процессора.\n\n", time);

    int n = next_prime(file_len(f));
    list_t *hash_list = calloc(n, sizeof(list_t));
    int collisions, max_collisions;
    int (*hash)(char *, int);

    hash = &simple_hash;
    time = tick();
    collisions = build_hash_table(&hash_list, n, f, hash);
    time = tick() - time;

    printf("\nХЕШ-ТАБЛИЦА НА ОСНОВЕ ПРОСТОЙ ФУНКЦИИ:");
    print_hash_table(hash_list, n);
    printf("Максимальное количество коллизий = %d.\n", collisions);
    printf("Хеш-таблица построена за = %ld тактов процессора (с учетом времени чтения из файла).\n\n", time);

    printf("\nВведите допустимое количество коллизий: ");
    int ec = scanf("%d", &max_collisions);
    if (ec != 1 || max_collisions < 1)
    {
        printf("Введено некорректное значение!\n");
        return VAL_ERROR;
    }
    if (collisions > max_collisions)
    {
        while (collisions != max_collisions)
        {
            free_list_arr(hash_list, n);
            n = next_prime(n);
            hash_list = calloc(n, sizeof(list_t));

            hash = &compl_hash;
            time = tick();
            collisions = build_hash_table(&hash_list, n, f, hash);
            time = tick() - time;
        }
        printf("\nХЕШ-ТАБЛИЦА НА ОСНОВЕ УЛУЧШЕННОЙ ФУНКЦИИ:");
        print_hash_table(hash_list, n);
        printf("Максимальное количество коллизий = %d.\n", collisions);
        printf("Хеш-таблица построена за = %ld тактов процессора (с учетом времени чтения из файла).\n\n", time);
    }
    else
        printf("Максимальное количество коллизий не больше введенного.\nРеструктуризация таблицы не требуется.\n");

    char to_find[STR_SIZE];
    printf("\nВведите слово, которое необходимо найти: ");
    scanf("%s", to_find);

	double avg_search_time, avg_cmprs;
    tree_node *root2 = NULL;
    fill_tree(&root2, f);
    time = tick();
    ec = search_tree(to_find, *root2);
    time = tick() - time;
    if (ec > 0)
    {
		avg_search_time = search_tree_avg(*root2, words, flen, &avg_cmprs);
        printf("\nПОИСК В ДЕРЕВЕ\n");
        printf("Слово \"%s\" найдено за %lu тактов процессора.\n"
               "Среднее время поиска в дереве - %lf тактов процессора.\n"
               "Двоичное дерево поиска занимает - %lu байт.\n"
               "Количество сравнений для достижения результата - %d.\n"
               "Среднее количество сравнений - %f.\n",
               to_find, time, avg_search_time, (unsigned long) (unique * sizeof(tree_node)), ec, avg_cmprs);
    }
    else
    {
        printf("Слово \"%s\" не найдено.\n", to_find);
        return VAL_ERROR;
    }
    free_tree(root2);

    time = tick();
    ec = search_tree(to_find, *root);
    time = tick() - time;
    if (ec > 0)
    {
		avg_search_time = search_tree_avg(*root, words, flen, &avg_cmprs);
        printf("\nПОИСК В СБАЛАНСИРОВАННОМ ДЕРЕВЕ\n");
        printf("Слово \"%s\" найдено за %lu тактов процессора.\n"
               "Среднее время поиска в сбалансированном дереве - %lf тактов процессора.\n"
               "Сбалансированное дерево занимает - %lu байт.\n"
               "Количество сравнений для достижения результата - %d.\n"
               "Среднее количество сравнений - %f.\n",
               to_find, time, avg_search_time, (unsigned long) (unique * sizeof(tree_node)), ec, avg_cmprs);
    }
    else
    {
        printf("Слово \"%s\" не найдено.\n", to_find);
        return VAL_ERROR;
    }
    free_tree(root);

    time = tick();
    ec = search_hashtable(to_find, hash_list, n, hash);
    time = tick() - time;
    if (ec > 0)
    {
		avg_search_time = search_hashtable_avg(hash_list, n, hash, words, flen, &avg_cmprs);
        printf("\nПОИСК В ХЕШ-ТАБЛИЦЕ\n");
        printf("Слово \"%s\" найдено за %lu тактов процессора.\n"
               "Среднее время поиска в хеш-таблице - %lf тактов процессора.\n"
               "Хеш-таблица занимает - %lu байт.\n"
               "Количество сравнений для достижения результата - %d.\n"
               "Среднее количество сравнений - %f.\n",
               to_find, time / 4, avg_search_time, (unsigned long) ((n + unique - list_filling(hash_list, n)) * sizeof(list_t) + sizeof(list_t *)), ec, avg_cmprs);
    }
    else
    {
        printf("Слово \"%s\" не найдено.\n", to_find);
        return VAL_ERROR;
    }
    free_list_arr(hash_list, n);

    time = tick();
    ec = search_file(to_find, f);
    time = tick() - time;
    if (ec > 0)
    {
        printf("\nПОИСК В ФАЙЛЕ\n");
        printf("Слово \"%s\" найдено за %lu тактов процессора.\n"
               "Среднее время поиска в файле - %lf тактов процессора.\n"
               "Файл занимает - %u байт.\n"
               "Количество сравнений для достижения результата - %d.\n"
               "Среднее количество сравнений - %f.\n",
               to_find, time, search_file_avg(f, words, flen), fbytes(f), ec, (double)(file_len(f) + 1) / 2);
    }
    else
    {
        printf("Слово \"%s\" не найдено.\n", to_find);
        return VAL_ERROR;
    }
    fclose(f);
    return OK;
}

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );
    return d;
}

char **fill_words(FILE *f)
{
    rewind(f);
    unsigned long flen = file_len(f);

    int ind = 0;
    char buffer[STR_SIZE];
    char **words = (char **)malloc(sizeof(char *) * flen);
    for (int i = 0; i < flen; i++)
        words[i] = (char *)malloc(sizeof(char) * STR_SIZE);

    while (fgets(buffer, STR_SIZE, f) != NULL)
    {
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
        strcpy(words[ind], buffer);
        ind++;
    }
    rewind(f);
    return words;
}

void free_words(char **words, unsigned long flen)
{
    for (int i = 0; i < flen; ++i)
        free(words[i]);
    free(words);
}

int search_tree(char *to_find, tree_node root)
{
    int i = 0;
    tree_node *p = &root;
    while (p != NULL)
    {
        i++;
        if (strcmp(p->val, to_find) == 0)
            return i;
        if (strcmp(p->val, to_find) < 0)
            p = p->right;
        else
            p = p->left;
    }
    return NOT_FOUND;
}

double search_tree_avg(tree_node root, char **words, unsigned long flen, double *cmp_avg)
{
	int cmprs = 0;
    unsigned long time = tick();
    for (int i = 0; i < flen; i++)
        cmprs += search_tree(words[i], root);
    time = tick() - time;
	*cmp_avg = (double)cmprs / flen;
    return (double)time / flen;
}

int search_hashtable(char *to_find, list_t *hash_list, int n, int (*hash)(char *, int))
{
    int ec = deepcmp(to_find, hash_list[hash(to_find, n)]);
    if (ec > 0)
        return ec;
    return NOT_FOUND;
}

double search_hashtable_avg(list_t *hash_list, int n, int (*hash)(char *, int), char **words, unsigned long flen, double *cmp_avg)
{
	int cmprs = 0;
    unsigned long time = tick();
    for (int i = 0; i < flen; i++)
        cmprs += search_hashtable(words[i], hash_list, n, hash);
    time = tick() - time;
	*cmp_avg = (double)cmprs / flen;
    return (double)time / flen;
}

int search_file(char *to_find, FILE *f)
{
    rewind(f);
    int i = 0;
    char buffer[STR_SIZE];
    while (fgets(buffer, STR_SIZE, f) != NULL)
    {
        i++;
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
        if (strcmp(to_find, buffer) == 0)
            return i;
    }
    rewind(f);
    return NOT_FOUND;
}

double search_file_avg(FILE *f, char **words, unsigned long flen)
{
    unsigned long time = tick();
    for (int i = 0; i < flen; i++)
        search_file(words[i], f);
    time = tick() - time;
    return (double)time / flen;
}

tree_node *create_node(char *val)
{
    tree_node *node = malloc(sizeof(tree_node));
    node->val = malloc(sizeof(char *) * (strlen(val) + 1));
    strcpy(node->val, val);
    node->left = NULL;
    node->right = NULL;
    return node;
}

tree_node *insert(tree_node *node, char *val)
{
    if (!node)
		return create_node(val);
    if (strcmp(val, node->val) < 0)
        node->left = insert(node->left, val);
    else if (strcmp(val, node->val) > 0)
		node->right = insert(node->right, val);
    return node;
}

void fill_tree(tree_node **root, FILE *f)
{
    char buffer[STR_SIZE];
    fgets(buffer, STR_SIZE, f);
    if (buffer[strlen(buffer) - 1] == '\n')
		buffer[strlen(buffer) - 1] = '\0';
    *root = insert(*root, buffer);

    while (fgets(buffer, STR_SIZE, f) != NULL)
    {
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
        insert(*root, buffer);
    }
    rewind(f);
}

void print_tree(tree_node *root, int space)
{
    if (!root)
        return;

    space += SPACING;
    print_tree(root->right, space);
    printf("\n");
    for (int i = SPACING; i < space; i++)
        printf(" ");
    printf("%s\n", root->val);
    print_tree(root->left, space);
}

void push_back(dynarr_t *arr, tree_node *to_add)
{
    arr->size++;
    if (arr->size >= arr->mem_size)
    {
        arr->mem_size *= 2;
        arr->arr = realloc(arr->arr, arr->mem_size * sizeof(tree_node *));
    }
    for (int i = arr->size; i > 0; i--)
        arr->arr[i] = arr->arr[i - 1];
    arr->arr[0] = to_add;
}

void store_nodes(tree_node *root, dynarr_t *arr)
{
    if (!root)
        return;
    store_nodes(root->left, arr);
    push_back(arr, root);
    store_nodes(root->right, arr);
}

tree_node *build_tree(dynarr_t *arr, int start, int end)
{
    if (start > end)
        return NULL;
    int mid = (start + end) / 2;
    tree_node *root = arr->arr[mid];
    root->right = build_tree(arr, start, mid - 1);
    root->left = build_tree(arr, mid + 1, end);
    return root;
}

tree_node *balance_tree(tree_node *root, int *unique)
{
    dynarr_t arr;
    arr.mem_size = 8;
    arr.size = 0;
    arr.arr = malloc(sizeof(tree_node *) * arr.mem_size);
    store_nodes(root, &arr);

    int n = arr.size;
    *unique = n;
    tree_node *r = build_tree(&arr, 0, n - 1);

    free(arr.arr);
    return r;
}

void free_tree(tree_node *node)
{
    if (!node)
        return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int is_prime(unsigned int num)
{
    if (num <= 1)
        return 0;
    if (num % 2 == 0 && num > 2)
        return 0;
    for (int i = 3; i < floor(sqrt(num)); i += 2)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

unsigned int next_prime(int n)
{
    unsigned int num = n + 1;
    while (!is_prime(num))
        num++;
    return num;
}

int compl_hash(char *s, int n)
{
    unsigned long hash = 0;
    int c;
    while ((c = *s++))
        hash = ((hash << 5) + hash) + c;
    return hash % n;
}

int simple_hash(char *s, int n)
{
	unsigned long hash = 0;
    int c;
    while ((c = *s++))
        hash += c;
    return hash % n;
}

int file_len(FILE *f)
{
    rewind(f);
    int lines = 0;
    char buffer[STR_SIZE];
    while (fgets(buffer, STR_SIZE, f) != NULL)
        lines++;

    rewind(f);
    return lines;
}

int fbytes(FILE *f)
{
    rewind(f);
    fseek(f, 0, SEEK_END);
    unsigned size = ftell(f);
    rewind(f);
    return size;
}

int build_hash_table(list_t **arr, int size, FILE *f, int (*hash)(char *, int))
{
    char buffer[STR_SIZE];
    int max_collisions = 0;

    while (fgets(buffer, STR_SIZE, f) != NULL)
    {
        int cur_collisions = 0;
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
        if (deepcmp(buffer, (*arr)[hash(buffer, size)]) == 0)
            cur_collisions = linsert(&(*arr)[hash(buffer, size)], buffer);
        if (cur_collisions > max_collisions)
            max_collisions = cur_collisions;
    }
    rewind(f);
    return max_collisions;
}

void print_hash_table(list_t *arr, int size)
{
    printf("\n------------------------------------------\n"
           "  ХЕШ | ДАННЫЕ\n"
           "------------------------------------------\n");
    list_t *entry;
    int flag = 0;
    for (int i = 0; i < size; i++)
    {
        flag = 0;
        entry = &arr[i];
        if (entry->value != NULL)
        {
            printf("%5d | ", i);
            flag = 1;
        }
        while (entry != NULL)
        {
            if (entry->value != NULL)
                printf("\"%s\" ", entry->value);
            entry = entry->next;
        }
        if (flag)
            printf("\n------------------------------------------\n");
    }
}

int linsert(list_t *arr, char *buffer)
{
    if (arr->next == NULL)
    {
        arr->next = calloc(1, sizeof(list_t));
        arr->value = malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(arr->value, buffer);
        return 1;
    }
    int depth = 1;
    depth += linsert(arr->next, buffer);
    return depth;
}

int deepcmp(char *s, list_t l)
{
    list_t *lp = &l;
    int i = 0;
    while (lp != NULL && lp->value != NULL)
    {
        i++;
        if (strcmp(s, lp->value) == 0)
            return i;
        lp = lp->next;
    }
    return 0;
}

int list_filling(list_t *arr, int size)
{
    int occupation = 0;
    for (int i = 0; i < size; i++)
    {
        if (arr[i].value != NULL)
            occupation++;
    }
    return occupation;
}

void free_list(list_t *node)
{
    if (node == NULL)
        return;

    free_list(node->next);
    free(node);
}

void free_list_arr(list_t *arr, int size)
{
    for (int i = 0; i < size; i++)
        free_list(arr[i].next);
    free(arr);
}