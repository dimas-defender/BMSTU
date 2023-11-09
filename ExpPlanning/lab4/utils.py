from modeller import Modeller
from generator import Generator
from distributions import  EvenDistribution, ExponentialDistribution
from processor import Processor
from math import sqrt

BG_COLOR = "#FF9B00"
ITEMS_COLOR = "#82D49B"

def modelling(clients_number, clients_proccessed, lambda_gen, sigma_gen, lambda_proc, lambda_gen2=None, sigma_gen2=None, lambda_proc2=None):
    timeGen = 1 / lambda_gen
    a = timeGen - sqrt(3) * sigma_gen
    b = timeGen + sqrt(3) * sigma_gen

    a2, b2 = a, b
    if lambda_gen2 and sigma_gen2:
        timeGen = 1 / lambda_gen2
        a2 = timeGen - sqrt(3) * sigma_gen2
        b2 = timeGen + sqrt(3) * sigma_gen2

    lam = 1 / lambda_proc
    lam2 = lam 
    if lambda_proc2:
        lam2 = 1 / lambda_proc2

    generators = [
        Generator(EvenDistribution(a, b), clients_number, 0),
        Generator(EvenDistribution(a2, b2), clients_number, 1)
    ]

    operators = [
        Processor(
            [ExponentialDistribution(lam),
            ExponentialDistribution(lam2)]
        )
    ]
        
    for gen in generators: 
        gen.receivers = operators.copy()

    model = Modeller(generators, operators)
    result = model.event_mode(clients_proccessed)
    '''print("Загрузка системы (расчетная): ", lambda_gen / lambda_proc, 
    "\nВремя работы: ", result['time'], 
    "\nСреднее время ожидания: ", result['avg_wait_time'], 
    "\nКоличество обработанных заявок: ", clients_proccessed)'''
    return result