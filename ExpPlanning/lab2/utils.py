from modeller import Modeller
from generator import Generator
from distributions import EvenDistribution, ExponentialDistribution
from processor import Processor
from math import sqrt

BG_COLOR = "#FF9B00"
ITEMS_COLOR = "#82D49B"

def modelling(clients_number, clients_proccessed, lambda_gen, sigma_gen, lambda_proc):
    timeGen = 1 / lambda_gen
    a = timeGen - sqrt(3) * sigma_gen
    b = timeGen + sqrt(3) * sigma_gen

    generators = [
        Generator(EvenDistribution(a, b), clients_number), 
    ]

    operators = [
        Processor(ExponentialDistribution(1 / lambda_proc)),
    ]

    for gen in generators: 
        gen.receivers = operators.copy()

    model = Modeller(generators, operators)
    result = model.event_mode(clients_proccessed)
    '''print("Загрузка системы (расчетная): ", lambda_gen / lambda_proc, 
    "\nВремя работы:", result['time'], 
    "\nСреднее время ожидания: ", result['avg_wait_time'], 
    "\nКоличество обработанных заявок", clients_proccessed)'''
    return result