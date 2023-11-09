from numpy.random import normal
from random import random

class EvenDistribution:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def generate(self):
        return self.a + (self.b - self.a) * random()

class NormalDistribution:
    def __init__(self, mu, sigma):
        self.mu = mu
        self.sigma = sigma

    def generate(self):
        return normal(self.mu, self.sigma)

def step_model(generator, processor, total_tasks=0, repeat=0, step=0.001):
    processed_tasks = 0
    t_cur = step
    t_gen = generator.generate()
    t_proc = 0
    cur_qlen = max_qlen = 0
    generated_tasks = 0
    repeated_tasks = 0

    while processed_tasks < total_tasks + repeated_tasks:
        if t_cur > t_gen and generated_tasks <= total_tasks:
            cur_qlen += 1
            generated_tasks += 1
            if cur_qlen > max_qlen:
                max_qlen = cur_qlen
            t_gen += generator.generate()
        if t_cur > t_proc and cur_qlen > 0:
            processed_tasks += 1
            if random() <= repeat:
                repeated_tasks += 1
                cur_qlen += 1
            cur_qlen -= 1
            t_proc += processor.generate()
        t_cur += step

    return max_qlen, processed_tasks, repeated_tasks

def add_event(events, event):
    i = 0
    while i < len(events) and events[i][0] < event[0]:
        i += 1
    events.insert(i, event)

def event_model(generator, processor, total_tasks=0, repeat=0):
    processed_tasks = 0
    cur_qlen = max_qlen = 0
    events = [[generator.generate(), 'g']]
    free, process_flag = True, False
    generated_tasks = 0
    repeated_tasks = 0

    while processed_tasks < total_tasks + repeated_tasks:
        event = events.pop(0)
        if event[1] == 'g' and generated_tasks <= total_tasks:
            cur_qlen += 1
            generated_tasks += 1
            if cur_qlen > max_qlen:
                max_qlen = cur_qlen
            add_event(events, [event[0] + generator.generate(), 'g'])
            if free:
                process_flag = True
        elif event[1] == 'p':
            processed_tasks += 1
            if random() <= repeat:
                repeated_tasks += 1
                cur_qlen += 1
            process_flag = True
        if process_flag:
            if cur_qlen > 0:
                cur_qlen -= 1
                add_event(events, [event[0] + processor.generate(), 'p'])
                free = False
            else:
                free = True
            process_flag = False

    return max_qlen, processed_tasks, repeated_tasks

def main():
    a, b = map(float, input('Введите границы диапазона для равномерного распределения: ').split())
    generator = EvenDistribution(a, b)

    mu, sigma = map(float, input('Введите параметры нормального распределения: ').split())
    processor = NormalDistribution(mu, sigma)

    tasks = int(input('Введите кол-во заявок в системе: '))
    repeat_prob = float(input('Укажите вероятность необходимости повторной обработки заявок: '))
    step = 0.01

    event_res = event_model(generator, processor, tasks, repeat_prob)
    deltaT_res = step_model(generator, processor, tasks, repeat_prob, step)
    print()
    print('-----------Событийный принцип-----------')
    print('Максимальная длина очереди:', event_res[0], '\nОбработано заявок:', event_res[1], '(', event_res[2], 'повторно)')
    print('-------------Принцип deltaT-------------')
    print('Максимальная длина очереди:', deltaT_res[0], '\nОбработано заявок:', deltaT_res[1], '(', deltaT_res[2], 'повторно)')

if __name__ == '__main__':
    main()