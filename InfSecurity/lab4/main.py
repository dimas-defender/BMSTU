from random import randrange, getrandbits
from math import log
from struct import pack

# R = A^K mod N
def fast_pow(a, k, n):
    r = 1
    while k > 0:
        if k % 2:
            r = (r * a) % n
        k //= 2
        a = (a * a) % n
    return r

def get_public_key(phi):
    while True:
        e = randrange(phi)
        if euclid(phi, e) == 1:
            return e

def get_private_key(phi, e):
    d = adv_euclid(phi, e)[2]
    if d < 0:
        d += phi
    return d

def euclid(a, b):
    if a < b:
        a, b = b, a
    while b != 0:
        a, b = b, a % b
    return a

# a*x + b*y = gcd(a,b)
# a*phi + b*e = 1
# (e*d) mod phi = 1 -> d = b
def adv_euclid(a, b):
    s_prev, s_cur, t_prev, t_cur = 1, 0, 0, 1

    while b != 0:
        q = a // b 
        r = a - q * b

        #R(k) = S(k)*A + T(k)*B
        #R(0) = 1 * A + 0 * B
        #R(1) = 0 * A + 1 * B
        s_next = s_prev - q * s_cur
        t_next = t_prev - q * t_cur
        s_prev = s_cur
        s_cur = s_next
        t_prev = t_cur
        t_cur = t_next

        a = b
        b = r

    gcd = a
    x = s_prev
    y = t_prev

    return gcd, x, y

def miller_rabin(n):
    rounds = int(log(n, 2))
    t = n - 1
    s = 0

    # представим n - 1 как 2^s * t
    while t % 2 == 0:
        t //= 2
        s += 1

    for i in range(rounds):
        flag = False
        a = randrange(2, n - 1)
        x = fast_pow(a, t, n)
        if x == 1 or x == n - 1:
            continue #найден свидетель простоты
        for j in range(s - 1):
            x = fast_pow(x, 2, n)
            if x == 1:
                return False #составное
            if x == n - 1:
                flag = True #найден свидетель простоты
                break
        if not flag:
            return False #составное
    return True #найдено rounds свидетелей простоты

def prime_number(bits):
    while True:
        num = getrandbits(bits)
        if num % 2 and miller_rabin(num):
            return num

class RSA:
    def __init__(self, bit_count):
        self.bit_count = bit_count
        self.e, self.d, self.n = self.get_keys()

    def get_keys(self):
        p = prime_number(self.bit_count)
        q = prime_number(self.bit_count)
        n = p * q
        phi = (p - 1) * (q - 1)
        e = get_public_key(phi)
        d = get_private_key(phi, e)
        return e, d, n

    def encrypt(self, src, enc):
        data = src.read()
        for item in data:
            enc_item = fast_pow(item, self.e, self.n)
            enc.write(str(enc_item) + "\n")

    def decrypt(self, enc, dec):
        line = enc.readline()
        while line:
            num = int(line)
            byte = fast_pow(num, self.d, self.n)
            dec.write(pack('B', byte))
            line = enc.readline()

def main():
    src_file = input('Введите имя файла для шифрования: ')
    try:
        src = open(src_file, 'rb')
    except FileNotFoundError:
        print("Файл не найден!")
        return

    enc_file = 'enc_' + src_file
    dec_file = 'dec_' + src_file
    rsa = RSA(256)

    enc = open(enc_file, "w")
    rsa.encrypt(src, enc)
    enc.close()
    src.close()

    enc = open(enc_file, "r")
    dec = open(dec_file, "wb")
    rsa.decrypt(enc, dec)
    dec.close()
    enc.close()

if __name__ == "__main__":
    main()