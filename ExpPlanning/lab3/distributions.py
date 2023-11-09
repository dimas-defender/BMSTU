from numpy.random import uniform, exponential
  
class EvenDistribution:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def generate(self):
        return uniform(self.a, self.b)
    
class ExponentialDistribution:
    def __init__(self, lambd):
        self.lambd = lambd

    def generate(self):
        return exponential(self.lambd)