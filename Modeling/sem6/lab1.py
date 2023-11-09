from prettytable import PrettyTable

X_MAX = 1
STEP = 1e-4
    
def f(x, y):
	return x * x + y * y

def approx1(x):
	return pow(x, 3) / 3

def approx2(x):
	return pow(x, 7) / 63 + approx1(x)

def approx3(x):
	return pow(x, 15) / 59535 + 2 * pow(x, 11) / 2079 + approx2(x)

def approx4(x):
	return pow(x, 31) / 109876903905 + \
		4 * pow(x, 27) / 3341878155 + \
		662 * pow(x, 23) / 10438212015 + \
		82 * pow(x, 19) / 37328445 + approx3(x)

def Picar(x_max, h, func):
	arr = []
	x, y = 0, 0
	while x < x_max:
		arr.append(y)
		x += h
		y = func(x)
	return arr

def Euler(x_max, h):
	arr = []
	x, y = 0, 0
	while x < x_max:
		arr.append(y)
		y += h * f(x, y)
		x += h
	return arr

def Runge(x_max, h):
	arr = []
	x, y = 0, 0
	while x < x_max:
		arr.append(y)
		y += h * f(x + h / 2, y + h / 2 * f(x, y))
		x += h
	return arr

def args(x_max, h):
	arr = []
	x = 0
	while x < x_max:
		arr.append(round(x, 4))
		x += h
	return arr

def main():
    res = PrettyTable()
    res.add_column("X", args(X_MAX, STEP))
    res.add_column("Picard 1", Picar(X_MAX, STEP, approx1))
    res.add_column("Picard 2", Picar(X_MAX, STEP, approx2))
    res.add_column("Picard 3", Picar(X_MAX, STEP, approx3))
    res.add_column("Picard 4", Picar(X_MAX, STEP, approx4))
    res.add_column("Euler", Euler(X_MAX, STEP))
    res.add_column("Runge", Runge(X_MAX, STEP))
    print(res)

main()