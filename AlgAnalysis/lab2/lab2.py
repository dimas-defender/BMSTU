from time import process_time
from random import randint
import matplotlib.pyplot as plt

def dotMatrix(a, b):
    if (len(b) != len(a[0])):
        raise ValueError
    
    m = len(a)
    n = len(a[0])
    q = len(b[0])
    c = [[0] * q for i in range(m)]

    t_start = process_time()
    for i in range(m):
        for j in range(q):
            for k in range(n):
                c[i][j] = c[i][j] + a[i][k] * b[k][j]
    t_end = process_time()

    return c, t_end - t_start

def dotMatrixVinograd(a, b):
    if (len(b) != len(a[0])):
        raise ValueError
    
    m = len(a)
    n = len(a[0])
    q = len(b[0])
    c = [[0] * q for i in range(m)]

    row = [0] * m
    col = [0] * q
    
    t_start = process_time()
    temp = n // 2
    for i in range(m):
        for j in range(temp):
            row[i] = row[i] + a[i][2*j] * a[i][2*j + 1]
    
    for j in range(q):
        for i in range(temp):
            col[j] = col[j] + b[2*i][j] * b[2*i + 1][j]

    for i in range(m):
        for j in range(q):
            c[i][j] = -row[i] - col[j]
            for k in range(temp):
                c[i][j] = c[i][j] + (a[i][2*k+1] + b[2*k][j]) * (a[i][2*k] + b[2*k+1][j])

    if n % 2:
        temp = n - 1
        for i in range(m):
            for j in range(q):
                c[i][j] = c[i][j] + a[i][temp] * b[temp][j]
    t_end = process_time()

    return c, t_end - t_start

def dotMatrixVinogradOpt(a, b):
    if (len(b) != len(a[0])):
        raise ValueError
    
    m = len(a)
    n = len(a[0])
    q = len(b[0])
    c = [[0] * q for i in range(m)]

    row = [0] * m
    col = [0] * q
    
    t_start = process_time()
    for i in range(m):
        for j in range(1, n, 2):
            row[i] -= a[i][j] * a[i][j - 1]
    
    for j in range(q):
        for i in range(1, n, 2):
            col[j] -= b[i][j] * b[i - 1][j]

    for i in range(m):
        for j in range(q):
            buf = row[i] + col[j]
            for k in range(1, n, 2):
                buf += (a[i][k - 1] + b[k][j]) * (a[i][k] + b[k-1][j])
            c[i][j] = buf

    if n % 2:
        temp = n - 1
        for i in range(m):
            for j in range(q):
                c[i][j] += a[i][temp] * b[temp][j]
    t_end = process_time()

    return c, t_end - t_start


def tryDot(dot, a, b):
    m = [[]]
    try:
        m, t = dot(a, b)
        print("Результат: ")
        printMatrix(m)
    except ValueError as e:
        print("Матрицы нельзя умножить!")
    return m

def testDot(dot, range_n, N = 1):
    data = []
    for n in range_n:
        m = [[randint(-1000, 1000) for j in range(n)] for i in range(n)]
        time = 0
        for i in range(N):
            tmp, t = dot(m, m)
            time += t
        time /= N
        data.append(time)
    return data      

def inputMatrix(msg):
    print(f"Введите {msg} матрицу")
    n = int(input("Введите кол-во строк: "))
    m = int(input("Введите кол-во столбцов: "))
    print('Введите элементы матрицы построчно:')
    matr = [[] for i in range(n)]
    for i in range(n):
        matr[i] = list(map(float, input().split()))
    print()
    return matr

def printMatrix(matr):
    for row in matr:
        for elem in row:
            print(elem, end=" ")
        print()
    print()

def mode_input():
    a = inputMatrix("первую")
    b = inputMatrix("вторую")

    print("1)Стандартный алгоритм")
    print("2)Алгоритм Винограда")
    print("3)Оптимизированный алгоритм Винограда")

    ch = input('\nВыберите алгоритм умножения матриц: ')
    if ch == '1':
        alg = dotMatrix
    elif ch == '2':
        alg = dotMatrixVinograd
    elif ch == '3':
        alg = dotMatrixVinogradOpt
    else:
        print('Некорректный ввод')

    if ch in ('1','2','3'):
        tryDot(alg, a, b)

def graph(std, vin, vinOpt, range_n, title):

    fig, ax = plt.subplots(figsize=(6, 4))
    ax.set_title(title)
    fig.canvas.set_window_title('График зависимости времени умножения матриц от их размера')
    ax.set_xlabel("Размерность")
    ax.set_ylabel("Время (сек)")
    plt.grid()
    ax.plot(range_n, std, label = 'Стандартный алгоритм')
    ax.plot(range_n, vin, label = 'Алгоритм Винограда')
    ax.plot(range_n, vinOpt, label = 'Оптим. алгоритм Винограда')

    ax.legend()
    plt.show()
    

def mode_test():
    best_case = range(100, 1001, 100)
    worst_case = range(101, 1002, 100)

    test = best_case
    std = testDot(dotMatrix, test)
    vin = testDot(dotMatrixVinograd, test)
    vinOpt = testDot(dotMatrixVinogradOpt, test)

    print('            Лучший случай')
    print("Размер Стандартный Винограда Винограда оптим.")
    for i, n in enumerate(test):
        print(f"{n:4} {std[i]:10.5f} {vin[i]:10.5f} {vinOpt[i]:10.5f}")

    graph(std, vin, vinOpt, test, 'Лучший случай')

    test = worst_case
    std = testDot(dotMatrix, test)
    vin = testDot(dotMatrixVinograd, test)
    vinOpt = testDot(dotMatrixVinogradOpt, test)

    print('\n            Худший случай')
    print("Размер Стандартный Винограда Винограда оптим.")
    for i, n in enumerate(test):
        print(f"{n:4} {std[i]:10.5f} {vin[i]:10.5f} {vinOpt[i]:10.5f}")

    graph(std, vin, vinOpt, test, 'Худший случай')

def main():
    print('1)Умножение матриц')
    print('2)Сравнение алгоритмов умножения матриц')
    ch = input('\nВыберите пункт меню: ')
    if ch == '1':
        mode_input()
    elif ch == '2':
        mode_test()
    else:
        print('Некорректный ввод')

if __name__ == "__main__":
    main()


