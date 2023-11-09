from random import random
from numpy import linalg
from prettytable import PrettyTable

deltaT = 1e-3
eps = 1e-3

def randMatrix(N):
    matrix = [[0.0 for j in range(N)] for i in range(N)]
    for i in range(N):
        for j in range(N):
            if i != j:
                matrix[i][j] = round(random(), 4)
    return matrix

def getCoeffs(matrix):
    n = len(matrix)
    lst = [[None for j in range(n)] for i in range(n)]

    for i in range(n):
        if i != (n - 1):
            for j in range(n):
                if j != i:
                    lst[i][j] = matrix[j][i]
                else:
                    lst[i][j] = -sum(matrix[i])
        else:
            for j in range(n):
                lst[i][j] = 1
    return lst

def getLimitProbs(matrix):
    coeffs = getCoeffs(matrix)
    N = len(matrix)
    return linalg.solve(coeffs, [0 if i != (N - 1) else 1 for i in range(N)]).tolist()

def getDeltaProbs(matrix, start_probs):
    n = len(matrix)
    return [deltaT * sum([-sum(matrix[i]) * start_probs[j] if i == j
            else matrix[j][i] * start_probs[j] for j in range(n)]) for i in range(n)]

def getLimitTimes(matrix, limit_probs):
    n = len(matrix)
    limit_times = [0.0] * n
    current_time = 0.0
    start_probs = [1.0 / n] * n
    current_probs = start_probs.copy()

    while not all(limit_times):
        deltaP = getDeltaProbs(matrix, current_probs)
        for i in range(n):
            if not limit_times[i] and abs(current_probs[i] - limit_probs[i]) <= eps:
                limit_times[i] = current_time
            current_probs[i] += deltaP[i]
            current_time += deltaT
    return limit_times

def calculate(matrix):
    probs = [round(x, 4) for x in getLimitProbs(matrix)]
    times = [round(x, 4) for x in getLimitTimes(matrix, probs)]
    return probs, times

def output(matrix, res_p, res_t):
    table_matrix = PrettyTable()
    cols = ["Состояния"]
    cols.extend([str(i + 1) for i in range(len(matrix))])
    table_matrix.field_names = cols
    for i in range(len(matrix)):
        tmp = [item for item in matrix[i]]
        tmp.insert(0, i + 1)
        table_matrix.add_row(tmp)
    print(table_matrix)
    print()

    table_res = PrettyTable()
    table_res.add_column("Состояния", [i + 1 for i in range(len(res_p))])
    table_res.add_column("Предельная вероятность", res_p)
    table_res.add_column("Время стабилизации", res_t)
    print(table_res)

def main():
    N = int(input("Введите количество состояний системы: "))
    matrix = randMatrix(N)
    probs, times = calculate(matrix)
    output(matrix, probs, times)

if __name__ == '__main__':
    main()