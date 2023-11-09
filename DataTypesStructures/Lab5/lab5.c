#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUCCESS 0
#define FAIL -1
#define ELEM 10000

#define T1_MIN 0
#define T1_MAX 6
#define T2_MIN 0
#define T2_MAX 1

#define CHANCE_MAX 100
#define CHANCE_MIN 0

#define TIME 1000
#define MOD 100

typedef struct queue
{
    float* data_store;   //массив для хранения элементов очереди
    int front;           //индекс последнего элемента очереди
    int num_of_elems;    //текущее количество элементов в очереди
    int summ_of_elems;   //общее количество элементов в очереди
} queue_t;

typedef struct alarm
{
    float time;
}alarm_t;

typedef struct automat
{
    float now;
    float work_time;
    int left1;
    float downtime;
    float work;
    int different_elems;
} automat;

typedef struct qnode
{
    struct qnode *next; //указатель на следующий элемент списка
    float time;         //время обработки
} node_t;

typedef struct automat_list
{
    node_t *now;
    float work_time;
    int left1;
    float downtime;
    float work;
    int different_elems;
} automat_list;

typedef struct queue_list
{
    struct qnode *pin;   //указатель на конец списка
    struct qnode *pout;  //указатель на начало списка
    int num_of_elems;    //текущее количество элементов в очереди
    int summ_of_elems;   //общее количество элементов в очереди
} queue_t_list;

node_t *add_adr[ELEM] = {NULL};
node_t *del_adr[ELEM] = {NULL};
int cnt1 = 0, cnt2 = 0;

automat* init_auto(void);
automat_list *init_auto_list(void);
float randfrom(int min, int max);
void refresh_automat(queue_t *q1, automat *a);
void refresh_automat_list(queue_t_list *q1, automat_list *a);
float get_time(queue_t *q1, automat *a);
float get_time_list(queue_t_list *q1, automat_list *a);
queue_t* new_queue(int max_size);
int add_elem(queue_t *queue, float item);
float delete_elem(queue_t *queue);
void queue_free(queue_t* queue);
void queue_print(queue_t* q);
node_t *get_new_node_list(float time);
queue_t_list *init_queue_list(void);
int add_elem_list(queue_t_list *queue, float time);
float delete_elem_list(queue_t_list *queue);
void print_queue_list(queue_t_list *queue);
void free_queue_list(queue_t_list *queue);
unsigned long long tick(void);

int main(void)
{
	setbuf(stdout, NULL);
    int type, choice, c;
	unsigned long tb, te, t_1, t_2;
	unsigned long size_1, size_2, size_3, size_4;
	printf("Выберите способ реализации очереди:\n");
    printf("1.Массив\n2.Список\n");
    scanf("%d", &type);
    if (type == 1)
    {
		tb = tick();
		automat *a = init_auto();
        queue_t *q1 = new_queue(ELEM);
        int len1 = 0, j = 1;
        float stay1 = 0, t = 0, i = 0;
        double duration = 0;
        clock_t t1, t2;

        srand((unsigned int) time(NULL));
        printf("|--------------МОДЕЛИРОВАНИЕ----------------|\n");
        while (a->different_elems < TIME)
        {
            t1 = clock();
            t = get_time(q1, a);
            t2 = clock();
            if (t == 0)
                return 0;
            duration = duration + (double) (t2 - t1) / CLOCKS_PER_SEC;

            if (a->different_elems % MOD == 0 && i != a->different_elems)
            {
                i = a->different_elems;
                len1 += q1->num_of_elems;
                stay1 += q1->data_store[q1->front - 1];
                printf("|--------------ПРОМЕЖУТОЧНОЕ----------------|\n");
                printf("|Текущее время: %28.3f|\n", t);
                printf("|Количество ушедших из очереди: %12d|\n", a->left1);
                j++;
                printf("|Текущая длина очереди:  %19d|\n", q1->num_of_elems);
                printf("|Средняя длина очереди:  %19d|\n", len1 /j);
                printf("|Среднее время простоя очереди:  %11.6f|\n", stay1 /j);
                printf("|-------------------------------------------|\n");
            }
        }
		te = tick();
		t_1 = te - tb;
        double t1min = T1_MIN, t1max = T1_MAX, t2min = T2_MIN, t2max = T2_MAX;
        double av_t_in1 = (t1min + t1max) / 2, av_t_out1 = (t2min + t2max) / 2;
        double total_t_in1 = TIME * av_t_in1;
        double total_t_out1 = TIME * av_t_out1;

        printf("Среднее время прихода: %f\n"
               "Среднее время обработки: %f\n"
               "Расчётное время моделирования: %.3f\n"
               "Расчётное время моделирования работы ОА: %.3f\n",
               av_t_in1, av_t_out1, total_t_in1, total_t_out1);
        printf("\n\n|----------------ИТОГОВЫЙ РЕЗУЛЬТАТ-------------------|\n"
               "|Время моделирования: %32f|\n"
               "|Время работы ОА: %36f|\n"
               "|Время простоя: %38f|\n"
               "|Количество выведенных различных заявок: %13d|\n"
               "|Количество заявок, введённых в очередь: %13d|\n"
               "|Количество обработанных заявок из очереди:%11d|\n", t, a->work, a->downtime,
                a->different_elems, q1->summ_of_elems, a->left1);
        printf("|-----------------------------------------------------|\n\n");

        double error1 = fabs(((a->different_elems - t/av_t_in1) / t/av_t_in1) * 100);
        double error2 = fabs(((t - a->work - a->downtime) / (a->work + a->downtime)) * 100);
        printf("Процент различия оценочных изменений и реальных по входу: %lf %%.\n", error1);
        printf("Процент различия оценочных изменений и реальных по выходу: %lf %%.\n", error2);
        printf("Погрешность работы системы: %lf %%.\n", error1 + error2);
		
		size_1 = sizeof(float) * ELEM + sizeof(queue_t);
		printf("Время работы программы: %lu\n", t_1);
		printf("Требуемый объем памяти: %lu\n", size_1);

        queue_free(q1);
    }
    else if (type == 2)
    {
		tb = tick();
        automat_list *a = init_auto_list();
        srand((unsigned int) time(NULL));
        queue_t_list *q1 = init_queue_list();
        int len1 = 0, j = 1, max_len = 0;
        float stay1 = 0, t = 0, i = 0;
        double duration = 0;
        clock_t t1, t2;

        printf("|--------------МОДЕЛИРОВАНИЕ----------------|\n");
        while (a->different_elems < TIME)
        {
            t1 = clock();
            t = get_time_list(q1, a);
            t2 = clock();
            if (t == 0)
                return 0;
            duration = duration + (double) (t2 - t1) / CLOCKS_PER_SEC;
			if (q1->num_of_elems > max_len)
				max_len = q1->num_of_elems;

            if (a->different_elems % MOD == 0 && i != a->different_elems)
            {
                i = a->different_elems;
                len1 += q1->num_of_elems;
                stay1 += q1->pout->time;
                printf("|--------------ПРОМЕЖУТОЧНОЕ----------------|\n");
                printf("|Текущее время: %28.3f|\n", t);
                printf("|Количество ушедших из очереди: %12d|\n", a->left1);
                j++;
                printf("|Текущая длина очереди:  %19d|\n", q1->num_of_elems);
                printf("|Средняя длина очереди:  %19d|\n", len1 /j);
                printf("|Среднее время простоя очереди:  %11.6f|\n", stay1 /j);
                printf("|-------------------------------------------|\n");
            }
        }
		te = tick();
		t_1 = te - tb;
        double t1min = T1_MIN, t1max = T1_MAX, t2min = T2_MIN, t2max = T2_MAX;
        double av_t_in1 = (t1min + t1max) / 2, av_t_out1 = (t2min + t2max) / 2;
        double total_t_in1 = TIME * av_t_in1;
        double total_t_out1 = TIME * av_t_out1;

        printf("Среднее время прихода: %f\n"
               "Среднее время обработки: %f\n"
               "Расчётное время моделирования: %.3f\n"
               "Расчётное время моделирования работы ОА: %.3f\n",
               av_t_in1, av_t_out1, total_t_in1, total_t_out1);
        printf("\n\n|----------------ИТОГОВЫЙ РЕЗУЛЬТАТ-------------------|\n"
               "|Время моделирования: %32f|\n"
               "|Время работы ОА: %36f|\n"
               "|Время простоя: %38f|\n"
               "|Количество выведенных различных заявок: %13d|\n"
               "|Количество заявок, введённых в очередь: %13d|\n"
               "|Количество обработанных заявок из очереди:%11d|\n", t, a->work, a->downtime,
                a->different_elems, q1->summ_of_elems, a->left1);
        printf("|-----------------------------------------------------|\n\n");

        double error1 = fabs(((a->different_elems - t/av_t_in1) / t/av_t_in1) * 100);
        double error2 = fabs(((t - a->work - a->downtime) / (a->work + a->downtime)) * 100);
        printf("Процент различия оценочных изменений и реальных по входу: %lf %%.\n", error1);
        printf("Процент различия оценочных изменений и реальных по выходу: %lf %%.\n", error2);
        printf("Погрешность работы системы: %lf %%.\n", error1 + error2);
		
		size_1 = sizeof(node_t) * max_len + sizeof(queue_t_list);
		printf("Время работы программы: %lu\n", t_1);
		printf("Требуемый объем памяти (в пике): %lu\n", size_1);
		
		printf("\nВывести адреса элементов?\n");
		printf("1.Да\n2.Нет\n");
		scanf("%d", &choice);
		if (choice == 1)
		{
			printf("Адреса добавленных элементов:\n");
			for (int i = 2500; i < 2510; i++)
				printf("%p\n", (void *) add_adr[i]);
			printf("\nАдреса удаленных элементов:\n");
			for (int i = 2500; i < 2510; i++)
				printf("%p\n", (void *) del_adr[i]);
		}		
		free_queue_list(q1);
    }
	printf("\nВывести информацию об эффективности двух реализаций очереди?\n");
	printf("1.Да\n2.Нет\n");
	scanf("%d", &c);
	if (c == 1)
	{
		queue_t *q_arr = new_queue(200);
		queue_t_list *q_list = init_queue_list();
		float value = 5.555;
		tb = tick();
		for (int i = 0; i < 10; i++)
			add_elem(q_arr, value);
		te = tick();
		t_1 = (te - tb) / 10;
		tb = tick();
		for (int i = 0; i < 10; i++)
			delete_elem(q_arr);
		te = tick();
		t_2 = (te - tb) / 10;
		printf("Время добавления элемента в очередь на массиве: %lu\n", t_1);
		printf("Время удаления элемента из очереди на массиве: %lu\n\n", t_2);
		tb = tick();
		for (int i = 0; i < 10; i++)
			add_elem_list(q_list, value);
		te = tick();
		t_1 = (te - tb) / 10;
		tb = tick();
		for (int i = 0; i < 10; i++)
			delete_elem_list(q_list);
		te = tick();
		t_2 = (te - tb) / 10;
		printf("Время добавления элемента в очередь на списке: %lu\n", t_1);
		printf("Время удаления элемента из очереди на списке: %lu\n\n", t_2);
		
		size_1 = sizeof(queue_t);
		size_2 = sizeof(float);
		size_3 = sizeof(queue_t_list);
		size_4 = sizeof(node_t);
		printf("Объем памяти, необходимой для хранения дескриптора массива: %lu\n", size_1);
		printf("Объем памяти, необходимой для хранения элемента массива: %lu\n\n", size_2);
		printf("Объем памяти, необходимой для хранения дескриптора списка: %lu\n", size_3);
		printf("Объем памяти, необходимой для хранения узла списка: %lu\n\n", size_4);
		
		queue_free(q_arr);
		free_queue_list(q_list);
	}
	return 0;
}

unsigned long long tick(void)
{
    unsigned long long d;

    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

automat* init_auto(void)
{
    automat *my_auto = malloc(sizeof(automat));
    if (!my_auto)
        return NULL;
    else
    {
        my_auto->now = -1;
        my_auto->work_time = 0;
        my_auto->left1 = 0;
        my_auto->downtime = 0;
        my_auto->work = 0;
        my_auto->different_elems = 0;
        return my_auto;
    }
}

automat_list *init_auto_list(void)
{
    automat_list *my_auto = malloc(sizeof(automat_list));
    if (!my_auto)
        return NULL;
    else
    {
        my_auto->now = NULL;
        my_auto->work_time = 0;
        my_auto->left1 = 0;
        my_auto->downtime = 0;
        my_auto->work = 0;
        my_auto->different_elems = 0;
        return my_auto;
    }
}

float randfrom(int min, int max)
{
    int range = max - min;
    int div = RAND_MAX / range;
    return  min + ((float) rand() / div);
}

void refresh_automat(queue_t *q1, automat *a)
{
    if (a->now != -1)
    {
        delete_elem(q1);
        a->now = -1;
        if (q1->num_of_elems > 0)
            a->now = q1->data_store[0];
        a->left1++;
        a->work_time = randfrom(T2_MIN, T2_MAX);
        a->work += a->work_time;
    }
    else
    {
        if (q1->num_of_elems == 0)
        {
            a->downtime += a->work_time;
            return;
        }
        else
        {
            a->now = q1->data_store[0];
            a->work_time = randfrom(T2_MIN, T2_MAX);
            a->work += a->work_time;
        }
    }
    return;
}

void refresh_automat_list(queue_t_list *q1, automat_list *a)
{
    if (a->now)
    {
        delete_elem_list(q1);
        a->now = NULL;
        if (q1->num_of_elems > 0)
            a->now = q1->pin;
        a->left1++;
        a->work_time = randfrom(T2_MIN, T2_MAX);
        a->work += a->work_time;
    }
    else
    {
        if (q1->num_of_elems == 0)
        {
            a->downtime += a->work_time;
            return;
        }
        else
        {
            a->now = q1->pin;
            a->work_time = randfrom(T2_MIN, T2_MAX);
            a->work += a->work_time;
        }
    }
    return;
}

queue_t* new_queue(int max_size)
{
    queue_t* queue = malloc(sizeof(queue_t));
    if (queue)
    {
        float* data_store = malloc(sizeof(float) * max_size);
        if (data_store)
        {
            queue->data_store = data_store;
            queue->front = 0;
            queue->num_of_elems = 0;
            queue->summ_of_elems = 0;
        }
        else
            return NULL;
    }
    else
        return NULL;

    return queue;
}

int add_elem(queue_t *queue, float item)
{
    if (queue->num_of_elems >= ELEM)
	{
		printf("Can't add element\n");
        return FAIL;
    }
    queue->data_store[queue->front] = item;
    queue->front++;
    queue->num_of_elems++;
    queue->summ_of_elems++;
    return SUCCESS;
}

float delete_elem(queue_t *queue)
{
    if (queue->num_of_elems < 1)
    {
		printf("Can't delete element\n");
        return FAIL;
    }
    float result = queue->data_store[0], tmp;
    queue->num_of_elems--;
    queue->front--;
    for (int i = 0; i < queue->front; i++)
    {
        tmp = queue->data_store[i + 1];
        queue->data_store[i] = tmp;
        queue->data_store[i + 1] = 0;
    }
    return result;
}

void queue_free(queue_t* queue)
{
    free(queue->data_store);
    free(queue);
}

void queue_print(queue_t* q)
{
    for (int i = 0; i < q->front; i++)
        printf("%d   %f  \n", i,  q->data_store[i]);
}

node_t *get_new_node_list(float time)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node)
        return NULL;
    new_node->time = time;
    new_node->next = NULL;
    return new_node;
}

queue_t_list *init_queue_list(void)
{
    queue_t_list *new_list = (queue_t_list *)malloc(sizeof(queue_t_list));
    if (!new_list)
        return NULL;
    new_list->pin = new_list->pout = NULL;
    new_list->num_of_elems = 0;
    new_list->summ_of_elems = 0;
    return new_list;
}

int add_elem_list(queue_t_list *queue, float time)
{
    if (!queue)
        return FAIL;
    node_t *new_node = get_new_node_list(time);
    if (!new_node)
	{
        return FAIL;
	}
	if (cnt1 < ELEM)
		add_adr[cnt1++] = new_node;
    if (queue->pin == NULL)
    {
        queue->pout = new_node;
        queue->pin = new_node;
        queue->num_of_elems++;
        queue->summ_of_elems++;
        return SUCCESS;
    }
    queue->pout->next = new_node;
    queue->pout = new_node;
    queue->num_of_elems++;
    queue->summ_of_elems++;
    return SUCCESS;
}

float delete_elem_list(queue_t_list *queue)
{
    if (!queue || !queue->pin)
        return FAIL;
    node_t *temp = queue->pin;
	if (cnt2 < ELEM)
		del_adr[cnt2++] = temp;
    float time = temp->time;
    queue->pin = temp->next;
    free(temp);
    queue->num_of_elems--;
    return time;
}

void print_queue_list(queue_t_list *queue)
{
    node_t *temp = queue->pin;
    while (temp)
    {
        printf("%g \t", temp->time);
        temp = temp->next;
    }
}

void free_queue_list(queue_t_list *queue)
{
    node_t *temp = queue->pin, *next;
    for (; temp; temp = next)
    {
        next = temp->next;
        free(temp);
    }
	free(queue);
}

static float time_now = 0;
static alarm_t al1;
static alarm_t al2;

float get_time(queue_t *q1, automat *a)
{
    if (!al1.time && !al2.time)
    {
        al1.time = randfrom(T1_MIN, T1_MAX);
        time_now = al1.time;
        if (add_elem(q1, randfrom(T1_MIN, T1_MAX)) != 0)
        {
            printf("Переполнение! Измените параметр ELEM (размер массива)\n");
            return 0;
        }
        a->now = q1->data_store[0];
        a->work_time = randfrom(T2_MIN, T2_MAX);
        al1.time = time_now + q1->data_store[0];
        al2.time = time_now + a->work_time;
    }
    else
    {
        if (al1.time <= al2.time)
        {
            time_now = al1.time;
            if (add_elem(q1, randfrom(T1_MIN, T1_MAX)) != 0)
            {
                printf("Переполнение! Измените параметр ELEM (размер массива)\n");
                return 0;
            }
            al1.time += q1->data_store[q1->front - 1];
        }
        if (al2.time <= al1.time)
        {
            float chance = randfrom(CHANCE_MIN, CHANCE_MAX);
            if (chance <= 80 && q1->num_of_elems > 0)
            {
                float buff = q1->data_store[q1->front - 1];
                if (q1->num_of_elems > 2)
                    q1->data_store[q1->front - 1] = 0;
                if (add_elem(q1, buff) != 0)
                {
                    printf("Переполнение! Измените параметр ELEM (размер массива)\n");
                    return 0;
                }
                a->now = q1->data_store[0];
            }
            if (chance > 80 && q1->num_of_elems > 0)
                a->different_elems++;
            refresh_automat(q1, a);
            time_now = al2.time;
            al2.time += a->work_time;
        }
    }
    return time_now;
}

float get_time_list(queue_t_list *q1, automat_list *a)
{
    if (!al1.time && !al2.time)
    {
        al1.time = randfrom(T1_MIN, T1_MAX);
        time_now = al1.time;
        add_elem_list(q1, randfrom(T1_MIN, T1_MAX));
        a->now = q1->pin;
        a->work_time = randfrom(T2_MIN, T2_MAX);
        al1.time = time_now + q1->pin->time;
        al2.time = time_now + a->work_time;
    }
    else
        {
        if (al1.time <= al2.time)
        {
            time_now = al1.time;
            add_elem_list(q1, randfrom(T1_MIN, T1_MAX));
            al1.time += q1->pout->time;
        }
        if (al2.time <= al1.time)
        {
            float chance = randfrom(CHANCE_MIN, CHANCE_MAX);
            if (chance <= 80 && q1->num_of_elems > 0)
            {
                float buff = q1->pout->time;
                if (q1->num_of_elems > 2)
                    q1->pout->time = 0;
                add_elem_list(q1, buff);
                a->now = q1->pin;
            }
            if (chance > 80 && q1->num_of_elems > 0)
                a->different_elems++;
            refresh_automat_list(q1, a);
            time_now = al2.time;
            al2.time += a->work_time;
        }
    }
    return time_now;
}