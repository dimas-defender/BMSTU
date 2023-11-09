from subprocess import check_output
from hashlib import sha256

def get_unique_id():
    uuid = check_output('wmic csproduct get uuid').decode().split('\n')[1].strip()

    disks = check_output('wmic diskdrive get Name, SerialNumber').decode().split('\n')[1:]
    for disk in disks:
        if 'DRIVE0' in disk:
            hdd_id = disk.split('DRIVE0')[-1].strip()
            break
    return uuid + ':' + hdd_id

def hash(str):
    return sha256(str.encode()).hexdigest()

if __name__ == "__main__":
    hash_unique_id = hash(get_unique_id())
    key = open('unique.id', 'w')
    key.write(hash_unique_id)
    key.close()
    print('Программа успешно установлена.')