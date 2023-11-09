from math import sqrt
from install import get_unique_id, hash

def equation(a, b, c):
    if a == 0:
        if b == 0:
            if c == 0:
                print('x - любое действительное число')
            else:
                print('Нет решений')
        else:
            x = -c / b
            print('{:.4}'.format(x))
    else:
        D = b**2 - 4 * a * c
        if D > 0:
            x1 = (-b + sqrt(D)) / (2 * a)
            x2 = (-b - sqrt(D)) / (2 * a)
            print('Два действительных корня: {:.4} и {:.4}'.format(x1, x2))
        elif D == 0:
            x = -b / (2 * a)
            print('Два одинаковых корня {:.4}'.format(x))
        else:
            x1 = complex(-b / (2 * a), sqrt(-D) / (2 * a))
            x2 = complex(-b / (2 * a),-sqrt(-D) / (2 * a))
            print('Комплексные корни: {:.4} и {:.4}'.format(x1, x2))

def check_key():
    try:
        file = open('unique.id', 'r')
    except FileNotFoundError:
        print('На данном устройстве необходимо запустить инсталлятор!')
        exit()
    key = file.read()
    return key == hash(get_unique_id())

if __name__ == "__main__":
    if check_key():
        a, b, c = map(float, input('Введите коэффициенты квадратного уравнения: ').split())
        equation(a, b, c)
    else:
        print("Идентификатор не совпадает, запустите программу на доверенном устройстве!")
