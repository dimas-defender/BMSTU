from bitarray import bitarray
import os
from tables import *

MSG_LEN = 64
ROUNDS = 16

def permutation(msg_bits, table):
    res_bits = bitarray()
    for i in range(len(table)):
        res_bits.append(msg_bits[table[i] - 1])
    
    return res_bits

def S_shift(msg_bits, S):
    msg_bits_str = str(msg_bits.to01())
    i = int(msg_bits_str[0] + msg_bits_str[-1], base = 2)
    j = int(msg_bits_str[1:-1], base = 2)
    res_bits = bitarray(bin(S[i][j])[2:])

    while len(res_bits) < 4:
        res_bits = bitarray('0') + res_bits
    return res_bits

def feistel(msg_bits, key):
    msg_bits = permutation(msg_bits, E)
    msg_bits = msg_bits ^ key

    res_bits = bitarray()
    for i in range(8):
        res_bits += S_shift(msg_bits[(i * 6):((i + 1) * 6)], S[i])
    
    res_bits = permutation(res_bits, P)
    return res_bits

def shift_left(msg_bits):
    return msg_bits[1:] + bitarray(str(msg_bits[0]))

def gen_round_keys(key_bits):
    key_bits = permutation(key_bits, B)
    C = key_bits[:28]
    D = key_bits[28:]

    keys = []
    for i in range(ROUNDS):
        for j in range(Si_shift[i]):
            C = shift_left(C)
            D = shift_left(D)
        new_key = permutation(C + D, CP)
        keys.append(new_key)
    return keys

def DES64(msg_bits, key, decode):
    msg_bits = permutation(msg_bits, IP)
    keys = gen_round_keys(key)
    
    if decode:
        keys.reverse()

    L_old = L = msg_bits[:MSG_LEN//2]
    R_old = R = msg_bits[MSG_LEN//2:]

    for i in range(ROUNDS):
        if not decode:
            L = R
            R = L_old ^ feistel(R, keys[i])
            L_old = L
        else:
            R = L
            L = R_old ^ feistel(L, keys[i])
            R_old = R

    msg_bits = permutation(L + R, IP_rev)
    return msg_bits

def des_algorithm(msg_bits, key, decode = False):
    num = 0
    while len(msg_bits) % MSG_LEN != 0:
        msg_bits += bitarray('0')
        num += 1

    res_bits = bitarray()
    for i in range(len(msg_bits) // MSG_LEN):
        res_bits += DES64(msg_bits[i * MSG_LEN : (i + 1) * MSG_LEN], key, decode)

    return res_bits, num

def encrypt_file(in_file, out_file, key, decode = False, cut_num = 0):
    try:
        input_file = open(in_file, 'rb')
    except FileNotFoundError:
        print("Файл не найден!")
        return

    msg_bits = bitarray()
    input_file = open(in_file, 'rb')
    msg_bits.fromfile(input_file)
    input_file.close()

    res_bits, num = des_algorithm(msg_bits, key, decode)
    if decode:
        res_bits = res_bits[:len(res_bits) - cut_num]

    output_file = open(out_file, 'wb')
    res_bits.tofile(output_file)
    output_file.close()
    return num

def read_key():
    f = open('cipher.key', 'rb')
    bytes = f.read()
    f.close()
    key = bitarray()
    key.frombytes(bytes)
    return key

def main():
    key = read_key()
    choice = None

    while choice != '0':
        print("\n1. Зашифровать сообщение из терминала")
        print("2. Зашифровать сообщение из файла")
        print("0. Выход\n")
        choice = input("Выберите пункт меню: ")

        if choice == '1':
            msg = input("\nВведите сообщение: ")
            
            msg_bits = bitarray()
            msg_bits.frombytes(msg.encode('utf-8'))
            res_bits, num_enc = des_algorithm(msg_bits, key)
            dec_msg = des_algorithm(res_bits, key, True)[0]
            dec_msg = dec_msg[:len(dec_msg) - num_enc]

            print("Зашифрованное сообщение:", bytearray(res_bits.tobytes()).hex().upper())
            print("Расшифрованное сообщение:", str(dec_msg.tobytes().decode()))
    
        elif choice == '2':
            input_file = input("\nВведите имя файла: ")
            filename = os.path.splitext(input_file)
            enc_file = filename[0] + '_encrypt' + filename[1]
            dec_file = filename[0] + '_decrypt' + filename[1]

            cut_num = encrypt_file(input_file, enc_file, key)
            encrypt_file(enc_file, dec_file, key, True, cut_num)
            
        elif choice != '0':
            print("Выберите корректный пункт меню!\n")

if __name__ == '__main__':
    main()