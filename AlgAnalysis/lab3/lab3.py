from time import process_time 
from random import randint
import matplotlib.pyplot as plt

def bubble_sort(a):
    for i in range(len(a) - 1):
        for j in range(len(a)- i - 1):
            if a[j] > a[j + 1]:
                a[j], a[j + 1] = a[j + 1], a[j]
    return a

def insertion_sort(a):
    for i in range(1, len(a)):
        key = a[i]
        j = i-1
        while j >= 0 and a[j] > key:
            a[j+1] = a[j]
            j -= 1
        a[j+1] = key
    return a

def selection_sort(a):
    for i in range(len(a)):
        min_i = i
        for j in range(i+1, len(a)):
            if a[j] < a[min_i]:
                min_i = j
        a[min_i], a[i] = a[i], a[min_i]
    return a


def mode_input():
    arr = list(map(int, input("Введите массив: ").split()))

    print("\n1)Сортировка пузырьком")
    print("2)Сортировка вставками")
    print("3)Сортировка выбором")

    ch = input('\nВыберите вид сортировки: ')
    if ch == '1':
        func = bubble_sort
    elif ch == '2':
        func = insertion_sort
    elif ch == '3':
        func = selection_sort
    else:
        print('Некорректный ввод')

    if ch in ('1','2','3'):
        print('Результат:')
        print(*func(arr))

def getTime(arr, func, N):
    t = 0
    for i in range(N):
        copy_arr = arr.copy()
        t_start = process_time()
        func(copy_arr)
        t_end = process_time()
        t += t_end - t_start
    return t / N


def graph(bubble, insert, select, title):

    range_n = [100, 250, 500, 1000]
    fig, ax = plt.subplots(figsize=(6, 4))

    ax.set_title(title)
    fig.canvas.set_window_title('График зависимости времени сортировки от размера массива')
    ax.set_xlabel("Размерность")
    ax.set_ylabel("Время (сек)")
    plt.grid()
    ax.plot(range_n, bubble, label = 'Сортировка пузырьком')
    ax.plot(range_n, insert, label = 'Сортировка вставками')
    ax.plot(range_n, select, label = 'Сортировка выбором')

    ax.legend()
    plt.show()

def mode_test(N = 10):
    range_n = (100, 250, 500, 1000)

    bubble = []
    insert = []
    select = []

    cnt = 0
    print("\nСлучайный массив:")
    print("  Размер  Пузырьком  Вставками   Выбором")
    for n in range_n:
        arr = [randint(-1000, 1000) for i in range(n)]
        bubble.append(getTime(arr, bubble_sort, N))
        insert.append(getTime(arr, insertion_sort, N))
        select.append(getTime(arr, selection_sort, N))
        print(f"{len(arr):8} {bubble[cnt]:10.5f} {insert[cnt]:10.5f} {select[cnt]:10.5f}")
        cnt += 1
        
    graph(bubble, insert, select, 'Случайный массив')


    bubble = []
    insert = []
    select = []
    
    cnt = 0
    print("\nУпорядоченный массив:")
    print("  Размер  Пузырьком  Вставками   Выбором")
    for n in range_n:
        arr = [randint(-1000, 1000) for i in range(n)]
        arr.sort()
        bubble.append(getTime(arr, bubble_sort, N))
        insert.append(getTime(arr, insertion_sort, N))
        select.append(getTime(arr, selection_sort, N))
        print(f"{len(arr):8} {bubble[cnt]:10.5f} {insert[cnt]:10.5f} {select[cnt]:10.5f}")
        cnt += 1

    graph(bubble, insert, select, 'Упорядоченный массив')

    bubble = []
    insert = []
    select = []
    
    cnt = 0
    print("\nМассив, упорядоченный в обратном порядке:")
    print("  Размер  Пузырьком  Вставками   Выбором")
    for n in range_n:
        arr = [randint(-1000, 1000) for i in range(n)]
        arr.sort(reverse = True)
        bubble.append(getTime(arr, bubble_sort, N))
        insert.append(getTime(arr, insertion_sort, N))
        select.append(getTime(arr, selection_sort, N))
        print(f"{len(arr):8} {bubble[cnt]:10.5f} {insert[cnt]:10.5f} {select[cnt]:10.5f}")
        cnt += 1

    graph(bubble, insert, select, 'Массив, упорядоченный в обратном порядке')


def main():
    print('1)Отсортировать массив')
    print('2)Сравнить сортировки')
    ch = input('\nВыберите пункт меню: ')
    if ch == '1':
        mode_input()
    elif ch == '2':
        mode_test()
    else:
        print('Некорректный ввод')

if __name__ == "__main__":
    main()
