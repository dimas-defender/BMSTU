from random import random

EPS = 1e-5
dt = 0.01

class EvenDistribution:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def generate(self):
        return self.a + (self.b - self.a) * random()

class Client:
    cur_id = 0
    def __init__(self):
        self.id = Client.cur_id
        Client.cur_id += 1

class Generator:
    def __init__(self, distribution):
        self.distrib = distribution
        self.time_left = 0

    def update(self):
        self.time_left -= dt
        if self.time_left <= EPS:
            self.time_left = self.distrib.generate()
            return Client()
        return None

class Manager:
    def __init__(self, queue, distribution):
        self.distrib = distribution
        self.busy = False
        self.queue = queue
        self.current_client = None
        self.time_left = 0

    def accept_client(self, client):
        self.busy = True
        self.current_client = client
        self.time_left = self.distrib.generate()

    def serve_client(self):
        self.queue.append(self.current_client)
        self.busy = False
        self.current_client = None

    def update(self):
        if self.busy:
            self.time_left -= dt
            if self.time_left <= EPS:
                self.serve_client()

class Mechanic:
    def __init__(self, client_queue, distribution):
        self.distrib = distribution
        self.busy = False
        self.client_queue = client_queue
        self.current_client = None
        self.time_left = 0

    def update(self):
        res = ''
        if self.busy:
            self.time_left -= dt
            if self.time_left <= EPS:
                self.busy = False
                self.current_client = None
                res = 'served'

        if not self.busy and len(self.client_queue) > 0:
            self.current_client = self.client_queue.pop(0)
            self.time_left = self.distrib.generate()
            self.busy = True

        return res

def choose_manager(managers):
    for i in range(len(managers)):
        if not managers[i].busy:
            return i
    return -1

def time_step(generator, managers, mechanics, clients_info, gen_flag = True):
    if gen_flag:
        client = generator.update()
        if client:
            clients_info['visited'] += 1
            man_ind = choose_manager(managers)
            if man_ind == -1:
                clients_info['denied'] += 1
            else:
                managers[man_ind].accept_client(client)

    for man in managers:
        man.update()

    for mech in mechanics:
        res = mech.update()
        if res == 'served':
            clients_info['served'] += 1

def model(generator, managers, mechanics, total_clients):
    clients_info = {'visited': 0, 'denied': 0, 'served': 0}
    time = 0

    while clients_info['visited'] < total_clients:
        time_step(generator, managers, mechanics, clients_info)
        time += dt

    while clients_info['denied'] + clients_info['served'] < total_clients:
        time_step(generator, managers, mechanics, clients_info, False)
        time += dt

    return clients_info, time

def main():
    queue1 = []
    queue2 = []
    
    clients = Generator(EvenDistribution(14, 20))

    managers = [
        Manager(queue1, EvenDistribution(26, 34)),
        Manager(queue2, EvenDistribution(28, 38))
    ]

    mechanics = [
        Mechanic(queue1, EvenDistribution(10, 18)),
        Mechanic(queue1, EvenDistribution(10, 20)),
        Mechanic(queue1, EvenDistribution(15, 19)),
        Mechanic(queue2, EvenDistribution(16, 20)),
        Mechanic(queue2, EvenDistribution(18, 24))
    ]

    total_clients = 1000
    res, time = model(clients, managers, mechanics, total_clients)

    print('Клиентов посетило:', res['visited'])
    print('Клиентов обслужено:', res['served'])
    print('Клиентов отклонено:', res['denied'])
    print('Модельное время:', round(time, 2))
    print('Вероятность отказа:', round(res['denied'] / total_clients, 3))

if __name__ == '__main__':
    main()