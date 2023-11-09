n = int(input('Введите степень аппроксимирующего полинома: '))
x = float(input('Введите значение аргумента: '))

X = []
Y = []
Yd = []

fin = open('lab1in.txt')
while True:
    s = fin.readline().strip().split()
    if not s: break
    X.append(float(s[0]))
    Y.append(float(s[1]))
    Yd.append(float(s[2]))
fin.close()

def div_diff(a, b):
    if b - a > 1:
        return (div_diff(a, b - 1) - div_diff(a + 1, b)) / (X[a] - X[b])
    else:
        return (Y[a] - Y[b]) / (X[a] - X[b])

def P(n):
    C = []
    for j in range(n + 1):
        diff = 100
        for i in range (len(X)):
            if abs(X[i] - x) < diff and (i not in C):
                diff = abs(X[i] - x)
                add_i = i
        C.append(add_i)
    C.sort()

    P = Y[C[0]]
    mult = 1
    for i in range(n):
        mult *= x - X[C[i]]
        P += mult * div_diff(C[0], C[i + 1])
    return P

print(' x = ', x)
print(' P(x) = ', P(n))
print('┌────────┬────────────┬────────────┬────────────┬────────────┐')
print('│   n    │     1      │     2      │     3      │     4      │') 
print('├────────┼────────────┼────────────┼────────────┼────────────┤')
print('│  P(x)  │  {:7.4f}'.format(P(1)),'  │  {:7.4f}'.format(P(2)),'  │  {:7.4f}'.format(P(3)),'  │  {:7.4f}'.format(P(4)),'  │')
print('└────────┴────────────┴────────────┴────────────┴────────────┘')

X, Y = Y, X
x = 0
print(' y = 0')
print('┌──────────┬────────────┬────────────┬────────────┬────────────┐')
print('│   n      │     1      │     2      │     3      │     4      │') 
print('├──────────┼────────────┼────────────┼────────────┼────────────┤')
print('│  Корень  │  {:7.4f}'.format(P(1)),'  │  {:7.4f}'.format(P(2)),'  │  {:7.4f}'.format(P(3)),'  │  {:7.4f}'.format(P(4)),'  │')
print('└──────────┴────────────┴────────────┴────────────┴────────────┘')

