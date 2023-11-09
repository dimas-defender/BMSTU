import matplotlib.pyplot as plt

def linear_search(dic : dict, key : str):
    keyarr = list(dic.keys())
    cmpcnt = 0
    while cmpcnt < len(keyarr):
        if keyarr[cmpcnt] == key:
            return cmpcnt + 1, dic.get(key)
        cmpcnt += 1
    return cmpcnt, 0

def binary_search(dic : dict, key : str):
    keyarr = list(dic.keys())
    keyarr.sort()
    first = 0
    last = len(keyarr)-1
    res = -1
    cmpcnt = 0
    while (first <= last) and (res == -1):
        mid = (first + last) // 2
        cmpcnt += 1
        if keyarr[mid] == key:
            return cmpcnt, dic.get(key)
        else:
            cmpcnt += 1
            if key < keyarr[mid]:
                last = mid - 1
            else:
                first = mid + 1
    return cmpcnt, 0

def sumvaluesdic(dic : dict):
    return sum(list(dic.values()))

def segm_search(dic : dict, key : str):
    segms = []
    for i in range(13):
        segms.append(dict())
    for elem in dic:
        value = dic.get(elem)
        if value > 40:
            segms[0].update([(elem, value)])
        elif value > 19:
            segms[1].update([(elem, value)])
        elif value > 10:
            segms[2].update([(elem, value)])
        else:
            for i in range(1, 11):
                if value == i:
                    segms[13 - i].update([(elem, value)])
    #segms.sort(key = sumvaluesdic, reverse = True)
    counter = 0   
    j = 0
    while j < 13:
        res = binary_search(segms[j], key)
        counter += res[0]
        if res[1]:
            return counter, res[1]
        j += 1
    
    return counter, 0

def print_menu():
    print("\nMenu:\n  1. Find value with key by brute force.\n\
  2. Find value with key by binary search.\n\
  3. Find value with key by segmentation search.\n\
  4. Experiment mode.\n  5. Exit.\nYour choice:", end=' ')

def testing(dic : dict):
    lendic = len(list(dic.keys()))
    y = [0]*lendic
    res1brute = [0]*lendic
    res1bin = [0]*lendic
    res1segm = [0]*lendic
    k = 0
    for key in dic:
        res1brute[k] = linear_search(dic, key)[0]
        res1bin[k] = binary_search(dic, key)[0]
        res1segm[k] = segm_search(dic, key)[0]
        y[k] = k
        k += 1
    res2brute = []
    res2bin = []
    res2segm = []
    for j in range(lendic):
        res2brute.append((y[j], res1brute[j]))
        res2bin.append((y[j], res1bin[j]))
        res2segm.append((y[j], res1segm[j]))
    res2brute.sort(key=lambda x:x[1], reverse=True)
    res2bin.sort(key=lambda x:x[1], reverse=True)
    res2segm.sort(key=lambda x:x[1], reverse=True)
    x1 = []
    x2 = []
    x3 = []
    y1 = []
    y2 = []
    y3 = []
    for i in range(lendic):
        x1.append(str(res2brute[i][0]))
        y1.append(res2brute[i][1])
        x2.append(str(res2bin[i][0]))
        y2.append(res2bin[i][1])
        x3.append(str(res2segm[i][0]))
        y3.append(res2segm[i][1])
    fig, ax = plt.subplots()
    fig.set_size_inches(8, 4.5)
    ax.bar(y, res1brute, label="Линейный поиск")
    ax.set_xlabel("Индекс ключа в исходном словаре")
    ax.set_ylabel("Количество сравнений")
    ax.set_title("Зависимость количества сравнений от положения ключа")
    ax.legend()
    fig.show()
    fig, ax = plt.subplots()
    fig.set_size_inches(8, 4.5)
    ax.bar(y, res1bin, label="Бинарный поиск", color='r')
    ax.set_xlabel("Индекс ключа в исходном словаре")
    ax.set_ylabel("Количество сравнений")
    ax.set_title("Зависимость количества сравнений от положения ключа")
    ax.legend()
    fig.show()
    fig, ax = plt.subplots()
    fig.set_size_inches(8, 4.5)
    ax.bar(y, res1segm, label="Поиск по сегментам", color='g')
    ax.set_xlabel("Индекс ключа в исходном словаре")
    ax.set_ylabel("Количество сравнений")
    ax.set_title("Зависимость количества сравнений от положения ключа")
    ax.legend()
    fig.show()
    fig, ax = plt.subplots()
    fig.set_size_inches(10, 5)
    ax.bar(x1, y1, label="Линейный поиск")
    ax.set_xlabel("Индекс ключа в исходном словаре")
    ax.set_ylabel("Количество сравнений")
    ax.set_title("Зависимость количества сравнений от положения ключа")
    ax.legend()
    fig.show()
    fig, ax = plt.subplots()
    fig.set_size_inches(10, 5)
    ax.bar(x2, y2, label="Бинарный поиск", color='r')
    ax.set_xlabel("Индекс ключа в исходном словаре")
    ax.set_ylabel("Количество сравнений")
    ax.set_title("Зависимость количества сравнений от положения ключа")
    ax.legend()
    fig.show()
    fig, ax = plt.subplots()
    fig.set_size_inches(10, 5)
    ax.bar(x3, y3, label="Поиск по сегментам", color='g')
    ax.set_xlabel("Индекс ключа в исходном словаре")
    ax.set_ylabel("Количество сравнений")
    ax.set_title("Зависимость количества сравнений от положения ключа")
    ax.legend()
    fig.show()

def read_data(d : dict):
    unsupported = [',', '.', '?', ':', ';', '—', '!', '(', ')', '«', '»']
    file = open('data.txt', 'r', encoding="utf-8")
    for line in file:
        for symb in unsupported:
            line = line.replace(symb, '')
        line = line.lower()
        splittedline = line[:-1].split(' ')
        for i in range(len(splittedline)):
            word = splittedline[i]
            if len(word):
                d.update([(word, d.get(word, 0) + 1)])
    file.close()

def main():
    d = dict()
    read_data(d)
    mode = 1
    while mode:
        print_menu()
        mode = int(input())
        if mode == 1:
            print('Input search key: ', end='')
            key = input()
            res = linear_search(d, key.lower())
            if res[1]:
                print('With key = ', key, ' value = ', res[1], ' was found by ', res[0], ' compares', sep='')
            else:
                print('With key = ', key, ' value was not found by ', res[0], ' compares', sep='')
        elif mode == 2:
            print('Input search key: ', end='')
            key = input()
            res = binary_search(d, key.lower())
            if res[1]:
                print('With key = ', key, ' value = ', res[1], ' was found by ', res[0], ' compares', sep='')
            else:
                print('With key = ', key, ' value was not found by ', res[0], ' compares', sep='')
        elif mode == 3:
            print('Input search key: ', end='')
            key = input()
            res = segm_search(d, key.lower())
            if res[1]:
                print('With key = ', key, ' value = ', res[1], ' was found by ', res[0], ' compares', sep='')
            else:
                print('With key = ', key, ' value was not found by ', res[0], ' compares', sep='')
        elif mode == 4:
            testing(d)
        elif mode == 5:
            mode = 0
            print("Exit.\n")
        else:
            mode = 1
            print("Wrong input!\n")

main()