from hashlib import sha256
import rsa

def hashFile(file):
    hash = sha256()
    data = file.read()
    hash.update(data)
    return hash.digest()

def createSignature(file, privateKey):
    hash = hashFile(file)
    signature = rsa.encrypt(hash, privateKey)
    return signature

def checkSignature(file, signature, publicKey):
    hash = hashFile(file)
    try:
        hashMessage = rsa.decrypt(signature, publicKey)
    except rsa.pkcs1.DecryptionError:
        return False
    return hash == hashMessage

def writeKeys(public, private, prefix):
    pubKeyFile = prefix + 'public.pem'
    with open(pubKeyFile, "wb") as pub:
        pub.write(public.save_pkcs1('PEM'))

    privKeyFile = prefix + 'private.pem'
    with open(privKeyFile, "wb") as priv:
        priv.write(private.save_pkcs1('PEM'))

def main():
    choice = None

    while choice != '3':
        print('\n1. Создать электронную подпись')
        print('2. Проверить электронную подпись')
        print('3. Выход\n')
        choice = input('Выберите пункт меню: ')

        if choice == '1':           
            fileToSign = input('Введите имя файла для создания подписи: ')
            try:
                file = open(fileToSign, 'rb')
            except FileNotFoundError:
                print("Файл не найден!")
                continue

            prefix = fileToSign + '_'

            privateKey, publicKey = rsa.newkeys(1024)
            writeKeys(publicKey, privateKey, prefix)
            print("\nКлючи созданы и записаны в файл.")

            signature = createSignature(file, privateKey)
            file.close()

            signatureFile = prefix + 'signature'
            with open(signatureFile, "wb") as f:
                f.write(signature)
            print("Электронная подпись успешно создана.")

        elif choice == '2':
            fileToCheck = input('Введите имя файла для проверки подписи: ')
            try:
                file = open(fileToCheck, 'rb')
            except FileNotFoundError:
                print("Файл не найден!")
                continue

            prefix = fileToCheck + '_'

            pubKeyFile = prefix + 'public.pem'
            try:
                pubKey = open(pubKeyFile, 'rb')
            except FileNotFoundError:
                print("Файл c публичным ключом не найден!")
                continue
            data = pubKey.read()
            pubKey.close()
            publicKey = rsa.PrivateKey.load_pkcs1(data)

            signatureFile = prefix + 'signature'
            try:
                s = open(signatureFile, 'rb')
            except FileNotFoundError:
                print("Файл c электронной подписью не найден!")
                continue
            signature = s.read()
            s.close()

            isValid = checkSignature(file, signature, publicKey)
            file.close()

            print()
            if isValid:
                print("Электронная подпись корректна!")
            else:
                print("Электронная подпись некорректна!")
        
        elif choice != '3':
            print("Выберите корректный пункт меню!")

if __name__ == "__main__":
    main()