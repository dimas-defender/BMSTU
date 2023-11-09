nx = int(input('Введите степень аппроксимирующего полинома по X: '))
ny = int(input('Введите степень аппроксимирующего полинома по Y: '))
x = float(input('Введите значение аргумента X: '))
y = float(input('Введите значение аргумента Y: '))

X = [0, 1, 2, 3, 4]
Y = [0, 1, 2, 3, 4]
F = [[0] * 5 for i in range(5)]

fin = open('lab2in.txt')
i = 0
while True:
    s = fin.readline().strip().split()
    if not s: break
    j = 0
    for a in s:
        F[i][j] = float(a)
        j += 1
    i += 1
fin.close()

def div_diff_x(a, b, ind):
    if b - a > 1:
        return (div_diff_x(a, b - 1, ind) - div_diff_x(a + 1, b, ind)) / (X[a] - X[b])
    else:
        return (F[ind][a] - F[ind][b]) / (X[a] - X[b])

def div_diff_y(a, b, Z, Iy):
    if b - a > 1:
        return (div_diff_y(a, b - 1, Z, Iy) - div_diff_y(a + 1, b, Z, Iy)) / (Y[Iy[a]] - Y[Iy[b]])
    else:
        return (Z[a] - Z[b]) / (Y[Iy[a]] - Y[Iy[b]])
    
def P2(nx, ny):
    Iy = []
    for j in range(ny + 1):
        diff = 100
        for i in range(len(Y)):
            if abs(Y[i] - y) < diff and (i not in Iy):
                diff = abs(Y[i] - y)
                add_i = i
        Iy.append(add_i)
    Iy.sort()

    Z = []
    for b in Iy:
        Z.append(P1(nx, x, b))


    P = Z[0]
    mult = 1
    for i in range(ny):
        mult *= y - Y[Iy[i]]
        P += mult * div_diff_y(0, i + 1, Z, Iy)
    return P

def P1(n, x, ind):
    Ix = []
    for j in range(n + 1):
        diff = 100
        for i in range(len(X)):
            if abs(X[i] - x) < diff and (i not in Ix):
                diff = abs(X[i] - x)
                add_i = i
        Ix.append(add_i)
    Ix.sort()

    P = F[ind][Ix[0]]
    mult = 1
    for i in range(n):
        mult *= x - X[Ix[i]]
        P += mult * div_diff_x(Ix[0], Ix[i + 1], ind)
    return P

print(P2(nx, ny))
x = 1.5
y = 1.5
print(' \nx = ', x, '; y = ', y)
print('┌──────────┬────────────┬────────────┬────────────┐')
print('│ nx \ ny  │      1     │      2     │      3     │') 
print('├──────────┼────────────┼────────────┼────────────┤')
print('│    1     │  {:7.4f}'.format(P2(1,1)),'  │  {:7.4f}'.format(P2(1,2)),'  │  {:7.4f}'.format(P2(1,3)),'  │')
print('├──────────┼────────────┼────────────┼────────────┤')
print('│    2     │  {:7.4f}'.format(P2(2,1)),'  │  {:7.4f}'.format(P2(2,2)),'  │  {:7.4f}'.format(P2(2,3)),'  │')
print('├──────────┼────────────┼────────────┼────────────┤')
print('│    3     │  {:7.4f}'.format(P2(3,1)),'  │  {:7.4f}'.format(P2(3,2)),'  │  {:7.4f}'.format(P2(3,3)),'  │')
print('└──────────┴────────────┴────────────┴────────────┘')
