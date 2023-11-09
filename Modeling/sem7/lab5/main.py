from random import random

EPS = 1e-5
dt = 0.01

class EvenDistribution:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def generate(self):
        return self.a + (self.b - self.a) * random()  

class Request:
    cur_id = 0
    def __init__(self):
        self.id = Request.cur_id
        Request.cur_id += 1

class Generator:
    def __init__(self, distribution):
        self.distrib = distribution
        self.time_left = 0

    def update(self):
        self.time_left -= dt

        if self.time_left <= EPS:
            self.time_left = self.distrib.generate()
            return Request()
        return None

class Operator:
    def __init__(self, storage, distribution):
        self.distrib = distribution
        self.busy = False
        self.storage = storage
        self.current_req = None
        self.time_left = 0

    def acccept_req(self, request):
        self.busy = True
        self.current_req = request
        self.time_left = self.distrib.generate()

    def complete_req(self):
        self.storage.append(self.current_req)
        self.busy = False
        self.current_req = None

    def update(self):
        self.time_left -= dt
        if self.busy and self.time_left <= EPS:
            self.complete_req()

class Processor:
    def __init__(self, req_queue, distribution):
        self.distrib = distribution
        self.busy = False
        self.req_queue = req_queue
        self.current_req = None
        self.time_left = 0

    def update(self):
        self.time_left -= dt

        if self.busy and self.time_left <= EPS:
            self.busy = False
            self.current_req = None
            return 'completed'

        if not self.busy and len(self.req_queue) > 0:
            self.current_req = self.req_queue.pop(0)
            self.time_left = self.distrib.generate()
            self.busy = True

def choose_operator(ops):
    for i in range(len(ops)):
        if not ops[i].busy:
            return i
    return -1

def time_step(generator, operators, processors, req_info, gen_flag = True):
    if gen_flag:
        request = generator.update()
        if request:
            req_info['generated'] += 1
            op_ind = choose_operator(operators)
            if op_ind == -1:
                req_info['denied'] += 1
            else:
                operators[op_ind].acccept_req(request)

    for op in operators:
        op.update()

    for proc in processors:
        res = proc.update()
        if res == 'completed':
            req_info['processed'] += 1

def model(generator, operators, processors, total_reqs):
    req_info = {'generated': 0, 'denied': 0, 'processed': 0}
    time = 0

    while req_info['generated'] < total_reqs:
        time_step(generator, operators, processors, req_info)
        time += dt

    while req_info['denied'] + req_info['processed'] < total_reqs:
        time_step(generator, operators, processors, req_info, False)
        time += dt

    return req_info, time

def main():
    queue1 = []
    queue2 = []
    
    clients = Generator(EvenDistribution(8, 12))

    operators = [
        Operator(queue1, EvenDistribution(15, 25)),
        Operator(queue1, EvenDistribution(30, 50)),
        Operator(queue2, EvenDistribution(20, 60))
    ]

    processors = [
        Processor(queue1, EvenDistribution(15, 15)),
        Processor(queue2, EvenDistribution(30, 30))
    ]

    total_reqs = 300
    res, time = model(clients, operators, processors, total_reqs)

    print('Заявок сгенерировано:', res['generated'])
    print('Заявок обработано:', res['processed'])
    print('Заявок отклонено:', res['denied'])
    print('Модельное время:', round(time, 2))
    print('Вероятность отказа:', round(res['denied'] / total_reqs, 3))

if __name__ == '__main__':
    main()