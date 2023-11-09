import matplotlib.pyplot as plt
from math import sqrt
import numpy as np
from scipy.stats import norm

def uniform_func(a, b, x):
    return (x - a) / (b - a) if a < x < b else 0 if x <= a else 1

def uniform_dens(a, b, x):
    return 1 / (b - a) if a <= x <= b else 0

def norm_func(x, mu, sigma):
    return norm.cdf(x, mu, sqrt(sigma))

def norm_dens(x, mu, sigma):
    return norm.pdf(x, mu, sqrt(sigma))

def draw(x, y_func, y_dens, name):
    fig, axs = plt.subplots(2, figsize=(6, 7))

    fig.suptitle(name)
    axs[0].plot(x, y_func, color='purple')
    axs[1].plot(x, y_dens, color='purple')

    axs[0].set_xlabel('x')
    axs[0].set_ylabel('F(x)')
    axs[1].set_xlabel('x')
    axs[1].set_ylabel('f(x)')
    axs[0].grid(True)
    axs[1].grid(True)

    plt.show()

def main():
    a = float(input("Input a: "))
    b = float(input("Input b: "))
    delta = b - a
    x = np.arange(a - delta / 2, b + delta / 2, 0.001)
    y_func = [uniform_func(a, b, i) for i in x]
    y_dens = [uniform_dens(a, b, i) for i in x]
    draw(x, y_func, y_dens, 'Равномерное распределение')

    mu = float(input("Input mu: "))
    sigma = float(input("Input sigma: "))
    x = np.arange(-10, 10, 0.001)
    y_func = norm_func(x, mu, sigma)
    y_dens = norm_dens(x, mu, sigma)
    draw(x, y_func, y_dens, 'Нормальное распределение')

if __name__ == '__main__':
    main()