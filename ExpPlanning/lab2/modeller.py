from processor import Processor

class Modeller:
    def __init__(self, generators, operators):
        self._generators = generators
        self._operators = operators

    def event_mode(self, num_requests):
        refusals = 0
        processed = 0
        created = 0
        wait_times =  []

        for g in self._generators: 
            g.next = g.next_time()

        self._operators[0].next = self._operators[0].next_time()
        blocks = self._generators + self._operators 

        while processed <= num_requests:
            current_time = self._generators[0].next
            for block in blocks:   # находим ближайшее событие
                if 0 < block.next < current_time:
                    current_time = block.next

            for block in blocks:
                if current_time == block.next: # если наступило событие этого блока
                    if not isinstance(block, Processor): # для генератора проверяем, может ли оператор обработать
                        next_generator = block.generate_request(current_time)
                        if next_generator is not None and next_generator.next == 0:
                            next_generator.next = current_time + next_generator.next_time()
                            created += 1
                        else:
                            refusals += 1
                        block.next = current_time + block.next_time()
                    else:
                        wait_time = block.process_request(current_time)
                        wait_times.append(wait_time)
                        processed += 1
                        if len(block.queue) == 0:
                            block.next = 0
                        else:
                            block.next = current_time + block.next_time()

        avg_wait_time = 0
        for time in wait_times:
            if (time != -1):
                avg_wait_time += time
        
        avg_wait_time /= len(wait_times)

        return {'time': current_time, "avg_wait_time": avg_wait_time}