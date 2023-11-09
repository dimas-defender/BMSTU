#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define YES 1
#define NO 0
#define MINUS 1
#define PLUS 0
#define OK 0
#define ZERO_DIV_ERROR 1 // ошибка: деление на нуль
#define TOO_BIG_E_ERROR 2 // ошибка: число выходит за диапазон
#define INPUT_ERROR 3 // ошибка: неверный ввод числа
#define DATA_LEN 2 // количество входных данных
#define MAXLEN 40 // максимальная длина вводимой строки (учитывая знаки, "Е" и прочее)
#define MANT_SIZE 32 // размер массива для хранения мантиссы (с учетом места под точку и '\0')
#define MANT_LEN 30 // длина мантиссы
#define OPERANDS 3 // кол-во операндов (делимое, делитель, частное)

typedef char raw_data_arr[DATA_LEN][MAXLEN + 1]; // массив для введенных пользователем строк
typedef char num30[MANT_SIZE]; // Тип "строка" для мантиссы
typedef struct // структура для хранения больших чисел
{
	short int sign_m; // знак мантиссы
	num30 mant; // значение мантиссы
	int exp_num; // порядок числа
	unsigned int error_code; // ненулевое значение, если деление не произошло успешно
} big_num;
typedef big_num big_nums_arr[OPERANDS]; 

int input_num(char *str, int max_len); // Функция ввода данных 
int is_digit(char c); // Функция проверки, является ли символ числом
int check_data(char *str); // Функция проверки корректности ввода
int parse_raw_data(char *str, big_num *data); // Функция приведения строки к виду структуры хранения больших чисел
int mant_more_eq(char *mant1, char *mant2); // Функция ">=" для мантисс больших чисел
int mant_diff(char *mant1, char *mant2); // Функция вычитания мантисс больших чисел
void move_mant(big_num *num, int k); // Функция, выполняющая сдвиг мантиссы
int is_zero(char *num); // Функция проверки, является ли мантисса числа чистым нулем
int div_iteration(big_num *a, big_num b); // Функция выполнения одной итерации деления
big_num big_division(big_num a, big_num b); // Функция деления больших чисел
void remove_zeros(char *str); // Функция, убирающая незначащие нули
void print_number(big_num num); // Функция печати большого числа

int main(void)
{
	setbuf(stdout, NULL); // отключение буферизации
	raw_data_arr str_nums; 
	big_nums_arr data;

	// Запрашиваем входные данные и проверяем их корректность
	printf("Input first number (dividend) in format with point (.00025, +123001, -123.456) or \
in exponential format (1234567E-20, 1234567E20 or -123.4567E23) without spaces:\n\
    5    10   15   20   25   30   35   40\n\
|---|----|----|----|----|----|----|----|--\n");
	input_num(str_nums[0], MAXLEN);
	while (check_data(str_nums[0]))
	{
		printf("Wrong format, input number again:\n\
    5    10   15   20   25   30   35   40\n\
|---|----|----|----|----|----|----|----|--\n");
		input_num(str_nums[0], MAXLEN);
	}
	printf("Input second number (divider) in the same format:\n\
    5    10   15   20   25   30   35   40\n\
|---|----|----|----|----|----|----|----|--\n");
	input_num(str_nums[1], MAXLEN);
	while (check_data(str_nums[1]))
	{
		printf("Wrong format, input number again:\n\
    5    10   15   20   25   30   35   40\n\
|---|----|----|----|----|----|----|----|--\n");
		input_num(str_nums[1], MAXLEN);
	}
	// Приводим данные к нужной структуре
	for (int i = 0; i < 2; i++)
		parse_raw_data(str_nums[i], &data[i]);
	// Выполняем деление и выводим результат
	printf("Result: %s / %s = ", str_nums[0], str_nums[1]);
	data[2] = big_division(data[0], data[1]);
	if (!data[2].error_code)
		print_number(data[2]);
	else
	{
		printf("Error. Got incorrect number as a result of division (out of range) or division by zero happened.\n");
		return data[2].error_code;
	}
	return OK;
}

int input_num(char *str, int max_len)
{
	int q = 0, zero_num = 0;
	char c;
	while ((c = getchar()) == ' ' || c == '0') // игнорируем пробелы и незначащие нули
		if (c == '0')
			zero_num = 1;
	if ((c == EOF || c == '\n' || c == '\0') && zero_num) // если строка кончилась, а у нас был нуль, то введенное число - нуль
	{
		strcpy(str, "0");
		return OK;
	}
	while (q < max_len && c != '\n' && c != EOF && c != '\0') // пока не конец, не Enter и не достигнуто ограничение
	{
		str[q++] = c;
		c = getchar();
	}
	str[q] = '\0';
	if (q == max_len) // если достигнут лимит длины строки, то до конца строки (файла) пропускаем остальные символы
		while ((c = getchar()) != '\n' && c != EOF && c != '\0');
	return OK;
}

int is_digit(char c)
{
	return (c - '0' >= 0 && c - '9' <= 0) ? YES : NO;
}

int check_data(char *str)
{
	int stage = 1; // стадии чтения числа (+_11_._111_Е_-_111 - стадии чтения ограничены символом "_")
	int point = 0; // переменная-флаг, указывающая была ли точка в мантиссе или нет (1 - была, 0 - нет)
	int sign = 0; // переменная-флаг, указывающая был ли знак в мантиссе или нет (1 - был, 0 - нет)
	int e_cnt = 0; // счетчик количества цифр в порядке числа (их должно быть не больше пяти)
	int i;
	for (i = 0; str[i]; i++)
	{
		switch (stage) 
		{
			case 1:
				if (str[i] == '+' || str[i] == '-' || is_digit(str[i])) // прочитан знак, следующая цифра (или точка)
				{
					if (!is_digit(str[i]))
						sign = 1;
					stage = 2;
				}
				else if (str[i] == '.') // прочитана точка, пропускаем первую стадию с цифрой
					stage = 3;
				else if (str[i] == 'E') // мантиссы не ожидается, переход к стадии ввода знака и значения порядка
					stage = 5;
				else
					return INPUT_ERROR; // (не произошёл ни один из возможных вариантов ввода)
				break;

			case 2:
				if (i >= sign + MANT_LEN && str[i] != 'E' && str[i] != '.') // проверка на длину мантиссы
					return INPUT_ERROR;
				if (str[i] == 'E')
					stage = 5;
				else if (str[i] == '.')
					stage = 3;
				else if (!is_digit(str[i]))
					return INPUT_ERROR;
				break;

			case 3: // стадия "после точки". Ожидается число. Если число получено, то переход к стадии ожидания "Е"
				point = 1; // фиксируем факт получения точки
				if (!is_digit(str[i]) && str[i] != 'E')
					return INPUT_ERROR; 
				stage = 4;
				break;

			case 4: // стадия ожидания "Е"
				if (i >= sign + point + MANT_LEN && str[i] != 'E' && str[i] != '.') // проверка длины мантиссы с учетом точки
					return INPUT_ERROR;
				if (str[i] == 'E') 
					stage = 5;
				else if (!is_digit(str[i]))
					return INPUT_ERROR;
				break;

			case 5: // стадия ожидания знака или перехода к проверке значения "Е"
				if (str[i] == '+' || str[i] == '-' || (e_cnt = is_digit(str[i])))
					stage = 6;
				else
					return INPUT_ERROR;
				break;

			case 6: // стадия окончательной проверки "Е"
				if (!is_digit(str[i]))
					return INPUT_ERROR;
				else
					e_cnt++;
				if (e_cnt == 6)
					return INPUT_ERROR;
				break;

			default:
				return INPUT_ERROR;
		}
	}
	return (i == 0) ? INPUT_ERROR : OK;
} 


void move_mant(big_num *num, int k)
{
	for (int i = 0; i < MANT_SIZE - 1 - k; i++)
		num->mant[i] = num->mant[i + k];
	for (int j = MANT_SIZE - 1 - k; j < MANT_SIZE - 1; j++)
		num->mant[j] = '0';
}

int parse_raw_data(char *str, big_num *data)
{
	int i = 0, j = 0;
	int inc = 0; // для обработки ситуации, когда вводится одна "Е"
	int point = 0; 
	data->exp_num = 0;
	data->sign_m = (str[i] == '-') ? MINUS : PLUS; // знак мантиссы (если есть)
	if (str[i] == '+' || str[i] == '-') // знак учтен, переходим к значению
		i++;
	else if (str[i] == 'E') // если знака и значения не было, а была "Е", то наше значение - 1
	{
		strcpy(data->mant, ".1000000000000000000000000000000\0");
		inc = 1;
	}
	while ((is_digit(str[i]) || str[i] == '.') && j < MANT_SIZE - 1) // записываем значения
	{
		if (str[i] == '.') // проверяем, есть ли в числе точка
			point = 1;
		data->mant[j++] = str[i++];
	}
	// если в числе нет точки, то вставляем ее (после прочитанной целой части)
	if (!point)
		data->mant[j++] = '.'; 
	while (j < MANT_SIZE - 1) // дополняем мантиссу нулями до размера в 30 значащих цифр
		data->mant[j++] = '0';
	data->mant[j] = '\0';
	int sign = 1; // переменная, отвечающая за знак порядка
	if (str[i] == 'E') // переходим к считыванию значения порядка
		i++;
	if (str[i] == '-') // проверяем знак
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+') // пропускаем, если "+"
		i++;
	while (str[i]) // пока считываются цифры, собираем значение порядка
		data->exp_num = data->exp_num * 10 + (int)(str[i++] - '0');
	data->exp_num += inc;
	data->exp_num *= sign;
	// преобразование к виду ".xxxxxxxxx"
	int count = 0;
	while (data->mant[count] != '.')
		count++; // ищем позицию точки
	data->exp_num += count; // увеличиваем значение порядка
	while (count > 0)
	{
		data->mant[count] = data->mant[count - 1]; // смещаем точку в начало
		count--;
	}
	// заменим точку в мантиссе на доп. разряд, чтобы следить за переполнением ("0*" вместо ".*")
	data->mant[count] = '0';
	if (!is_zero(data->mant) && data->mant[1] == '0') // нормализация мантиссы
	{
		for (i = 1; data->mant[i] == '0' && data->mant[i];)
			i++;
		move_mant(data, i - 1);
		data->exp_num -= i - 1;
	}
	return OK;
}

int mant_more_eq(char *mant1, char *mant2)
{
	int i, diff;
	for (i = 0; mant1[i]; i++) // проходим по мантиссам
	{
		if ((diff = (int)(mant1[i] - mant2[i])) < 0) // если цифра во второй мантиссе больше, значит второе число больше
			return NO;
		else if (diff > 0) // иначе больше первое число
			return YES;
	}
	// числа равны
	return YES;
}

int mant_diff(char *mant1, char *mant2)
{
	if (!mant_more_eq(mant1, mant2)) // невозможно сделать вычитание, так как второе число больше
		return NO;
	for (int i = strlen(mant2) - 1; i >= 0; i--) // вычитание в столбик
	{
		if (mant1[i] - mant2[i] >= 0) // меньшему разряду не нужно занимать из более высокого разряда десятку
			mant1[i] -= mant2[i] - '0';
		else
		{
			int j = i - 1; // ищем, из какого разряда можно занять
			while (mant1[j] - '0' == 0) 
				j--;
			mant1[j]--;
			j++;
			for (; j < i; j++) //обновляем разряды
				mant1[j] += 9;
			mant1[i] += 10 - (mant2[i] - '0'); // делаем вычитание в разряде-заемщике
		}
	}
	return YES;
}

int div_iteration(big_num *a, big_num b)
{
	int iter = 0;
	while (mant_diff(a->mant, b.mant)) // считаем, сколько раз можно вычесть второе число из первого
		iter++;
	move_mant(a, 1); // сдвигаем мантиссу (как при делении в столбик)
	return iter;
}

int is_zero(char *num)
{
	int i = 0;
	for (i = 0; num[i] == '0'; i++);
	return (!num[i]) ? YES : NO;
}

big_num big_division(big_num a, big_num b)
{
	a.mant[0] = '0';
	b.mant[0] = '0';
	big_num res;
	res.error_code = 0;
	// проверяем делитель на равенство нулю
	if (is_zero(b.mant))
	{
		res.error_code = ZERO_DIV_ERROR;
		return res;
	}
	// проверяем делимое на равенство нулю
	if (is_zero(a.mant))
	{
		strcpy(res.mant, "00\0");
		res.exp_num = 0;
		return res;
	}
	if (abs(a.exp_num - b.exp_num) > 100000) // проверка выхода порядка за пределы области определения
	{
		res.error_code = TOO_BIG_E_ERROR;
		return res;
	}
	// деление в столбик (первая операция до цикла, чтобы не было 0 после точки)
	int temp = 0; // очередная цифра результата
	if (!mant_more_eq(a.mant, b.mant)) // если делимое меньше, чем делитель, то берем из порядка единицу и домножаем мантиссу на 10
	{
		a.exp_num -= 1;
		move_mant(&a, 1);
	}
	res.mant[0] = '0';
	temp = div_iteration(&a, b); // находим первую цифру результата (если мантисса делимого равна нулю, то деление прекращается)
	res.mant[1] = temp + '0';
	int i;
	for (i = 2; i < MANT_LEN + 1; i++) // находим все остальные цифры путем деления в столбик
	{
		if (is_zero(a.mant)) // если остаток после последнего деления = 0, то числа поделились нацело
		{
			res.mant[i] = '\0';
			temp = 0;
			break;
		}
		if (!mant_more_eq(a.mant, b.mant)) // если не вычитается, то берем больше, а в результат идет 0
		{
			move_mant(&a, 1);
			res.mant[i] = '0';
		}
		else // иначе - делим
		{
			temp = div_iteration(&a, b);
			res.mant[i] = '0' + temp;
		}
	}
	res.mant[MANT_LEN + 1] = '\0';
	if (i == MANT_LEN + 1 && (temp = div_iteration(&a, b)) >= 5) // округление
		res.mant[MANT_LEN] += 1;
	for (; i < MANT_LEN + 1; i++) // приведение мантиссы к 30 знакам
		res.mant[i] = '0';
	res.mant[MANT_LEN + 1] = '\0';
	int inc = 0;
	for (i = MANT_LEN; i >= 0; i--) // округление высших разрядов (0.999...)
	{
		res.mant[i] += inc;
		if (res.mant[i] == ':')
		{
			res.mant[i] = '0';
			inc = 1;
		}
		else
			break;
	}
	if (res.mant[0] == '1')
	{
		for (i = MANT_LEN; i > 0; i--)
			res.mant[i] = res.mant[i - 1];
		res.mant[0] = '0';
	}
	else
		inc = 0;
	res.mant[MANT_SIZE - 1] = '\0';
	res.sign_m = (a.sign_m + b.sign_m) % 2;
	res.exp_num = a.exp_num - b.exp_num + 1 + inc;
	if (res.exp_num > 99999) // проверка выхода порядка числа за пределы области определения
		res.error_code = TOO_BIG_E_ERROR;
	return res;
}

void remove_zeros(char *str) 
{
	int i, len = strlen(str); 
	for (i = len - 1; str[i] == '0' && i > 1; i--);
	str[i + 1] = '\0';
}

void print_number(big_num num)
{
	if (num.sign_m == MINUS)
		printf("-");
	remove_zeros(num.mant);
	printf("0.%s", &num.mant[1]);
	if (num.exp_num != 0)
		printf("E%d", num.exp_num);
}