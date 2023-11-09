x = float(input('Введите значение аргумента: '))

X = []
Y = []

fin = open('lab3in.txt')
while True:
    s = fin.readline().strip().split()
    if not s: break
    X.append(float(s[0]))
    Y.append(float(s[1]))
fin.close()

N = len(X) - 1
H = []

for i in range(1, N + 1):
    H.append(X[i] - X[i - 1])

A = []
B = []
C = []
D = []

E = []
T = []
E.append(0)
T.append(0)

F = []

def f(x):
    return x*x

def get_f(i):
    return 3*((Y[i] - Y[i-1])/H[i-1] - (Y[i-1] - Y[i-2])/H[i-2])

def get_E(k):
    i = k - 1
    return -H[i-1]/(H[i-2]*E[i-2] + 2*(H[i-2]+H[i-1]))

def get_T(k):
    i = k - 1
    return (F[i-2] - H[i-2]*T[i-2])/(H[i-2]*E[i-2] + 2*(H[i-2]+H[i-1]))

def get_C(i):
    if i == N:
        return T[i-1]
    else:
        return E[i-1]*C[i] + T[i-1]

def get_A(i):
    return Y[i-1]

def get_B(i):
    if i == N:
        return (Y[N] - Y[N-1])/H[N-1] - H[N-1]*2*C[N-1]/3
    else:
        return (Y[i] - Y[i-1])/H[i-1] - H[i-1]*(C[i] + 2*C[i-1])/3

def get_D(i):
    if i == N:
        return -C[N-1]/(3*H[N-1])
    else:
        return (C[i]-C[i-1])/(3*H[i-1])

def get_func(i, x):
    return A[i] + B[i]*(x - X[i]) + C[i]*(x - X[i])**2 + D[i]*(x - X[i])**3

def spline(x):
    k = -1
    for i in range(N):
        if X[i] <= x and X[i+1] >= x:
            k = i
    if k == -1:
        print('Значение не входит в промежуток заданной таблицы')
        return
    return get_func(k, x)

for i in range(N-1):
    F.append(get_f(i+2))

for i in range(N-1):
    E.append(get_E(i+3))
    T.append(get_T(i+3))
    
for i in range(N):
    A.append(0)
    B.append(0)
    C.append(0)
    D.append(0)
    
for i in range(N-1, -1, -1):
    C[i] = get_C(i+1)

for i in range(N):
    A[i] = get_A(i+1)
    B[i] = get_B(i+1)
    D[i] = get_D(i+1)

print('Результат интерполяции = ', spline(x))
print('Точное значение = ', f(x))
print('Погрешность = ', abs(f(x) - spline(x)))
    
