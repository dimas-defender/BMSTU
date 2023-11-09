from string import ascii_lowercase
from random import choice
from time import process_time
from copy import deepcopy

def randomStr(strlen = 5):
    letters = ascii_lowercase
    return ''.join(choice(letters) for i in range(strlen))

def printTable(str1, str2, table):
    print("  _ " + " ".join([i for i in str2]))
    for i in range(len(table)):
        if i:
            if len(str1)==0:
                print('\n_', end=' ')
            else:
                print('\n' + str1[i - 1], end=' ')
        else:
            print('_', end=' ')
        for j in range(len(table[i])):
            print(table[i][j], end=' ')
    print('\n')

def lastEqu(str1, str2):
    if not len(str1) or not len(str2):
        return 1
    return 0 if (str1[-1] == str2[-1]) else 1

def levRec(str1, str2, strlen1, strlen2, toprint = False):
    tstart = process_time()
    if not str1 or not str2:
        reshelp = abs(strlen1 - strlen2)
        tend = process_time()
        return reshelp, tend - tstart
    res = min(levRec(str1, str2[:-1], strlen1, strlen2-1)[0] + 1,
              levRec(str1[:-1], str2, strlen1-1, strlen2)[0] + 1,
              levRec(str1[:-1], str2[:-1], strlen1-1, strlen2-1)[0] + lastEqu(str1, str2))
    tend = process_time()
    return res, tend - tstart

def levRecMatr(str1, str2, strlen1, strlen2, toprint = True):
    tstart = process_time()
    matrix = [[i+j if (i == 0 or j == 0) else -1 for j in range(strlen2 + 1)] for i in range(strlen1 + 1)]
    recursion(str1, str2, strlen1, strlen2, matrix)
    tend = process_time()
    if toprint:
        printTable(str1, str2, matrix)
    return matrix[strlen1][strlen2], tend - tstart

def recursion(str1, str2, i, j, matrix):
    if matrix[i][j] != -1:
        return matrix[i][j]
    if not i or not j:
        return abs(len(str1) - len(str2))
    matrix[i][j] = min(recursion(str1, str2, i - 1, j, matrix) + 1,
                       recursion(str1, str2, i, j - 1, matrix) + 1,
                       recursion(str1, str2, i - 1, j - 1, matrix) +
                       lastEqu(str1[:i], str2[:j]))
    return matrix[i][j]

def levTable(str1, str2, strlen1, strlen2, toprint = True):
    tstart = process_time()
    matrix = []
    matrix.append([])
    for j in range(strlen2+1):
        matrix[0].append(j)
    if strlen1:
        matrix.append([])
        for j in range(strlen2+1):
            matrix[1].append(0)
    for i in range(1, strlen1+1):
        matrix[1][0] = i
        for j in range(1, strlen2+1):
            matrix[1][j] = min(matrix[0][j] + 1,
                              matrix[1][j - 1] + 1,
                              matrix[0][j - 1] + lastEqu(str1[:i], str2[:j]))
        matrix[0] = deepcopy(matrix[1])
    tend = process_time()
    if toprint:
        printTable(str1, str2, matrix)
    return matrix[-1][-1], tend - tstart

def damlevRec(str1, str2, strlen1, strlen2, toprint = False):
    tstart = process_time()
    if not str1 or not str2:
        res = abs(strlen1 - strlen2)
        tend = process_time()
        return res, tend - tstart
    res = min(damlevRec(str1, str2[:-1], strlen1, strlen2-1)[0] + 1,
              damlevRec(str1[:-1], str2, strlen1-1, strlen2)[0] + 1,
              damlevRec(str1[:-1], str2[:-1], strlen1-1, strlen2-1)[0] + lastEqu(str1, str2))
    if (strlen1 > 1 and strlen2 > 1 and str1[-1] == str2[-2] and str1[-2] == str2[-1]):
        res = min(res, damlevRec(str1[:-2], str2[:-2], strlen1-2, strlen2-2)[0] + 1)
    tend = process_time()
    return res, tend - tstart

def inputStrs():
    str1 = input("Input first str: ")
    str2 = input("Input second str: ")
    return str1, str2

def printRes(function, str1, str2, text="\n"):
    print(text, "Result distance is ", function(str1, str2, len(str1), len(str2))[0])

def TimeAnalysis(function, nIter, strLen = 5):
    ret_time = 0
    for i in range(nIter):
        str1 = randomStr(strLen)
        str2 = randomStr(strLen)
        ret_time += function(str1, str2, len(str1), len(str2), False)[-1]
    return ret_time / nIter

flag = True
while (flag):
    case = input("\nMenu:\n \
1. Levenshtein distance with 2-row cache\n \
2. Levenshtein distance recursion\n \
3. Levenshtein distance recursion with cache matrix\n \
4. Damerau–Levenshtein distance recursion\n \
5. Time analysis\n ")
    if (case == "1"):
        strs = inputStrs()
        printRes(levTable, strs[0], strs[1])
    elif (case == "2"):
        strs = inputStrs()
        printRes(levRec, strs[0], strs[1])
    elif (case == "3"):
        strs = inputStrs()
        printRes(levRecMatr, strs[0], strs[1])
    elif (case == "4"):
        strs = inputStrs()
        printRes(damlevRec, strs[0], strs[1])
    elif (case == "5"):
        nIter = 100
        for i in range(10, 11):
            print("Strlen: ", i)
            print("   Lev 2-row cache     : ", "{}".format(TimeAnalysis(levTable, nIter, i)))
            print("   Lev recursion       : ", "{}".format(TimeAnalysis(levRec, nIter, i)))
            print("   Lev recursion matrix: ", "{}".format(TimeAnalysis(levRecMatr, nIter, i)))
            print("   DamLev recursion    : ", "{}".format(TimeAnalysis(damlevRec, nIter, i)))
    else:
        flag = False