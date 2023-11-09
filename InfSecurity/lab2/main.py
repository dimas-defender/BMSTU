import os
import random

ALPH_LEN = 256

def init_rotor():
    rotor = [None for i in range(ALPH_LEN)]
    num = 0

    while None in rotor:
        ind = random.randint(0, ALPH_LEN - 1)
        if rotor[ind] == None:
            rotor[ind] = num
            num += 1
    
    return rotor

def init_reflector():
    reflector = [None for i in range(ALPH_LEN)]
    arr = [x for x in range(ALPH_LEN)]

    for i in range(ALPH_LEN):
        if reflector[i] == None:
            num = random.choice(arr)
            while num == i:
                num = random.choice(arr)
            arr.remove(num)
            arr.remove(i)
            reflector[i] = num
            reflector[num] = i
            
    return reflector

def encrypt_symbol(s, rotor1, rotor2, rotor3, reflector):
    s1 = rotor1.index(s)
    s2 = rotor2.index(s1)
    s3 = rotor3.index(s2)
    s4 = reflector.index(s3)
    s5 = rotor3[s4]
    s6 = rotor2[s5]
    s7 = rotor1[s6]
    return s7

def encrypt_message(msg, rotor1, rotor2, rotor3, reflector):
    nums = [ord(c) for c in msg]
    res_msg = []
    shift1 = 0
    shift2 = 0
    shift3 = 0
    for s in nums:
        res_msg.append(encrypt_symbol(s, rotor1, rotor2, rotor3, reflector))
        if shift1 < ALPH_LEN:
            rotor1 = rotor1[1:] + rotor1[:1]
            shift1 += 1
        elif shift2 < ALPH_LEN:
            rotor1 = rotor1[1:] + rotor1[:1]
            rotor2 = rotor2[1:] + rotor2[:1]
            shift1 = 0
            shift2 += 1
        elif shift3 < ALPH_LEN:
            rotor1 = rotor1[1:] + rotor1[:1]
            rotor2 = rotor2[1:] + rotor2[:1]
            rotor3 = rotor3[1:] + rotor3[:1]
            shift1 = 0
            shift2 = 0
            shift3 += 1
        else:
            rotor1 = rotor1[1:] + rotor1[:1]
            rotor2 = rotor2[1:] + rotor2[:1]
            rotor3 = rotor3[1:] + rotor3[:1]
            shift1 = 0
            shift2 = 0
            shift3 = 0

    return ''.join([chr(c) for c in res_msg])

def encrypt_file(in_file, out_file, rotor1, rotor2, rotor3, reflector):
    try:
        input_file = open(in_file, 'rb')
    except FileNotFoundError:
        print("Такого файла не существует!\n")
        return

    output_file = open(out_file, 'wb')
    read_byte = input_file.read(1000)

    while read_byte:
        byte_encrypt = b""
        for s in read_byte:
            ch = chr(s)
            ch = encrypt_message(ch, rotor1, rotor2, rotor3, reflector)
            byte_encrypt += bytes([ord(ch)])
        output_file.write(byte_encrypt)
        read_byte = input_file.read(1000)

    input_file.close()
    output_file.close()

def main():
    rotor1 = init_rotor()
    rotor2 = init_rotor()
    rotor3 = init_rotor()
    reflector = init_reflector()

    choice = None

    while choice != '0':
        print("1. Зашифровать сообщение из терминала")
        print("2. Зашифровать сообщение из файла")
        print("0. Выход\n")
        choice = input("Выберите пункт меню: ")

        if choice == '1':
            msg = input("\nВведите сообщение: ")
            msg_encrypt = encrypt_message(msg, rotor1, rotor2, rotor3, reflector)
            msg_decrypt = encrypt_message(msg_encrypt, rotor1, rotor2, rotor3, reflector)
            print("Зашифрованное сообщение: ", msg_encrypt)
            print("Расшифрованное сообщение: ", msg_decrypt)

        elif choice == '2':
            input_file = input("\nВведите имя файла: ")
            filename = os.path.splitext(input_file)
            enc_file = filename[0] + '_encrypt' + filename[1]
            dec_file = filename[0] + '_decrypt' + filename[1]
            encrypt_file(input_file, enc_file, rotor1, rotor2, rotor3, reflector)
            encrypt_file(enc_file, dec_file, rotor1, rotor2, rotor3, reflector)
        
        elif choice != '0':
            print("Выберите корректный пункт меню!\n")
    
if __name__ == '__main__':
    main()