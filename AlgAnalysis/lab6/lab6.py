from random import random, randint
from time import process_time

eps = 0.01

def nextSet(set_cur, length):
    is_next_set = True
    j = length - 2
    while j != -1 and set_cur[j] >= set_cur[j + 1]:
        j -= 1
    if j == -1:
        is_next_set = False
    else:
        k = length - 1
        while set_cur[j] >= set_cur[k]:
            k -= 1
        set_cur[j], set_cur[k] = set_cur[k], set_cur[j]
        l = j + 1
        r = length - 1
        while l < r:
            set_cur[l], set_cur[r] = set_cur[r], set_cur[l]
            l += 1
            r -= 1
    return is_next_set

def lenBySet(graph, set_cur, size):
    result = graph[set_cur[size-1]][set_cur[0]]
    for i in range(size - 1):
        result += graph[set_cur[i]][set_cur[i + 1]]
    return result

def bruteForce(graph, size):
    t1 = process_time()
    result = 0
    if size == 2:
        result = 2 * graph[0][1]
    elif size == 3:
        result = graph[0][1] + graph[0][2] + graph[1][2]
    elif size > 3:
        set_cur = [i for i in range(size)]
        result = lenBySet(graph, set_cur, size)
        while nextSet(set_cur, size):
            temp = lenBySet(graph, set_cur, size)
            if temp < result:
                result = temp
    t2 = process_time()
    return result, t2 - t1

class Params:
    def __init__(self, a, b, Q, p, pheromon, lifetime):
        self.alpha = a
        self.beta = b
        self.opt_len = Q
        self.vape = p
        self.pheromone = pheromon
        self.life_time = lifetime

class Ant:
    def __init__(self, pos, size):
        self.position = pos
        self.to_visit = []
        for _ in range(size):
            self.to_visit.append(1)

def antForce(graph, size, params):
    t1 = process_time()
    vision = []
    pheromon = []
    pheromon_cur = []
    ants = []
    best_len = graph[size-1][0]
    for i in range(size):
        if i != size - 1:
            best_len += graph[i][i+1]
        ants.append(Ant(i, size))
        vision.append(size*[0])
        pheromon.append(size*[params.pheromone])
        pheromon_cur.append(size*[0.0])
        for j in range(size):
            if i != j:
                vision[i][j] = 1 / graph[i][j]
    for _ in range(params.life_time):
        for k in range(size):
            path_length = 0
            for i in range(size - 1):
                ants[k].to_visit[ants[k].position] = 0
                probs = size*[0]
                for j in range(size):
                    if not ants[k].to_visit[j]:
                        probs[j] = [0, j]
                    else:
                        temp = 0.0
                        for z in range(size):
                            if ants[k].to_visit[z]:
                                temp += pow(pheromon[ants[k].position][z], params.alpha)*\
                                        pow(vision[ants[k].position][z], params.beta)
                        temp = pow(pheromon[ants[k].position][j], params.alpha)*\
                               pow(vision[ants[k].position][j], params.beta) / temp
                        probs[j] = [temp, j]
                probs.sort(key=lambda x:x[0])
                summ = 0.0
                for j in range(size):
                    if probs[j][0]:
                        temp = summ
                        summ += probs[j][0]
                        probs[j][0] += temp
                new_pos = newPos(probs, size)
                path_length += graph[ants[k].position][new_pos]
                pheromon_cur[ants[k].position][new_pos] += params.opt_len/path_length
                pheromon_cur[new_pos][ants[k].position] += params.opt_len/path_length
                ants[k].position = new_pos
            path_length += graph[ants[k].position][k]
            pheromon_cur[ants[k].position][k] += params.opt_len/path_length
            pheromon_cur[k][ants[k].position] += params.opt_len/path_length
            ants[k].position = k
            for j in range(size):
                ants[k].to_visit[j] = 1
            if best_len > path_length:
                best_len = path_length
        for i in range(size):
            for j in range(size):
                if i < j:
                    pheromon[i][j] = (1 - params.vape) * pheromon[i][j] + pheromon_cur[i][j]
                    if pheromon[i][j] < eps:
                        pheromon[i][j] = eps
                    pheromon[j][i] = pheromon[i][j]
                    pheromon_cur[i][j] = 0.0
                    pheromon_cur[j][i] = 0.0
    t2 = process_time()
    return best_len, t2 - t1

def newPos(probs, size):
    number = random()
    l = 0
    while l < size and not probs[l][0]:
        l += 1
    while l < size and number > probs[l][0]:
        l += 1
    return probs[l][1]

def testByBoundaries(left, right):
    print("Size   Tmax   Alpha   Beta   Delta   TimeBrute    TimeAnt")
    sizes = [3, 5, 7, 9]
    lifes = [10, 50, 100]
    for size in sizes:
        for lifetime in lifes:
            alpha = 0
            for alphabeta in range(11):
                consts = Params(alpha, 1-alpha, ((left+right) / 2) * size, 0.5, 0.5, lifetime)
                delta = 0
                timeant = 0
                timebrute = 0
                for _ in range(5):
                    graph = []
                    for i in range(size):
                        graph.append(size*[0])
                    for i in range(size):
                        for j in range(size):
                            if (i != j):
                                graph[i][j] = randint(left, right)
                                graph[j][i] = graph[i][j]
                    bruteres = bruteForce(graph, size)
                    antres = antForce(graph, size, consts)
                    if antres[0] == bruteres[0]:
                        delta += 1
                    timeant += antres[1]
                    timebrute += bruteres[1]
                timebrute /= 5
                timeant /= 5
                print("{:<7}{:<7}{:<8.1f}{:<7.1f}{:<8}{:<13.8f}{:.8f}\n".format(size, lifetime, alpha, 1-alpha, delta, timebrute, timeant))
                alpha += 0.1

mode = 1
while mode:
    print("Menu:\n  1. Solve the TSP by ants.\n\
  2. Solve the TSP by brute force.\n\
  3. Experiment mode.\n  4. Exit.\nYour choice:", end=' ')
    mode = int(input())
    if mode == 1:
        maxlen = -1
        minlen = 0
        print("Input size of graph: ")
        graph_size = int(input())
        if not str(graph_size).isdigit() or graph_size < 2:
            print('Wrong input!\n')
            continue
        graph = []
        for i in range(graph_size):
            graph.append(graph_size*[0])
        print("Input path lengths:")
        for i in range(graph_size - 1):
            for j in range(i + 1, graph_size):
                print(i, " -> ", j, ":", sep='', end=' ')
                graph[i][j] = int(input())
                graph[j][i] = graph[i][j]
                if i == 0 and j == 1:
                    minlen = graph[i][j]
                maxlen = max(maxlen, graph[i][j])
                minlen = min(minlen, graph[i][j])
        print("Input alpha parameter: ")
        alpha = float(input())
        print("Input vaporization parameter: ")
        p = float(input())
        print("Input initial pheromone parameter: ")
        pheromone = float(input())
        print("Input life time of colony: ")
        tmax = int(input())
        consts = Params(alpha, 1 - alpha, graph_size / 2 * (minlen + maxlen), p, pheromone, tmax)
        print("Shortest path:", antForce(graph, graph_size, consts)[0], "\n")
    elif mode == 2:
        print("Input size of graph: ")
        graph_size = int(input())
        if not str(graph_size).isdigit() or graph_size < 2:
            print('Wrong input!\n')
            continue
        graph = []
        for i in range(graph_size):
            graph.append(graph_size*[0])
        print("Input path lengths:")
        for i in range(graph_size - 1):
            for j in range(i + 1, graph_size):
                print(i, " -> ", j, ":", sep='', end=' ')
                graph[i][j] = int(input())
                graph[j][i] = graph[i][j]
        print("Shortest path:", bruteForce(graph, graph_size)[0], "\n")
    elif mode == 3:
        testByBoundaries(10, 50)
        testByBoundaries(10, 500)
    elif mode == 4:
        mode = 0
        print("Exit.\n")
    else:
        mode = 1
        print("Wrong input!\n")