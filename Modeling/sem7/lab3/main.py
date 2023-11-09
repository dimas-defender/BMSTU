from PyQt5 import QtWidgets, uic
from PyQt5.QtWidgets import QTableWidgetItem
from math import log
from itertools import islice
import sys

STEP = 10

class Window(QtWidgets.QMainWindow):
	def __init__(self):
		QtWidgets.QWidget.__init__(self)
		uic.loadUi("main.ui", self)
		self.fill_alg.clicked.connect(lambda: fillAlgNums(self))
		self.fill_table.clicked.connect(lambda: fillTableNums(self))
		self.manual_input.returnPressed.connect(lambda: calcManual(self))
		self.meas_alg_1.setReadOnly(True)
		self.meas_alg_2.setReadOnly(True)
		self.meas_alg_3.setReadOnly(True)
		self.meas_table_1.setReadOnly(True)
		self.meas_table_2.setReadOnly(True)
		self.meas_table_3.setReadOnly(True)
		self.meas_manual.setReadOnly(True)
		self.alg_table.setEditTriggers(QtWidgets.QAbstractItemView.NoEditTriggers)
		self.table_table.setEditTriggers(QtWidgets.QAbstractItemView.NoEditTriggers)
		self.line_num = 0

		for i in range(10):
			self.alg_table.insertRow(i)
		for i in range(10):
			self.table_table.insertRow(i)

def randomCriteria(seq):
	N = len(seq)
	if N == 0:
		return 0

	numsFreq = dict()
	for x in seq:
		if x not in numsFreq.keys():
			numsFreq.update({x: 1})
		else:
			numsFreq[x] += 1

	crit = 0
	for x in numsFreq.keys():
		p = numsFreq[x] / N
		crit -= p * log(p, N)
	return crit

class LinearCongruent:
    m = 2**32
    a = 1664525
    c = 1013904223
    _cur = 1

    def next(self):
        self._cur = (self.a * self._cur + self.c) % self.m
        return self._cur

def fillAlgNums(win):
    table = win.alg_table
    one_digit = [generator.next() % 9 + 1 for i in range(1000)]
    two_digits = [generator.next() % 90 + 10 for i in range(1000)]
    three_digits = [generator.next() % 900 + 100 for i in range(1000)]

    for i in range(10):
        item = QTableWidgetItem(str(one_digit[i * STEP]))
        table.setItem(i, 0, item)
    for i in range(10):
        item = QTableWidgetItem(str(two_digits[i * STEP]))
        table.setItem(i, 1, item)
    for i in range(10):
        item = QTableWidgetItem(str(three_digits[i * STEP]))
        table.setItem(i, 2, item)

    crit_one = randomCriteria(one_digit)
    crit_two = randomCriteria(two_digits)
    crit_three = randomCriteria(three_digits)
    win.meas_alg_1.setText('{:.4%}'.format(crit_one))
    win.meas_alg_2.setText('{:.4%}'.format(crit_two))
    win.meas_alg_3.setText('{:.4%}'.format(crit_three))

def fillTableNums(win):
	table = win.table_table
	numbers = set()
	with open('nums.txt') as file:
		lines = islice(file, win.line_num, None)
		for l in lines:
			numbers.update(set(l.split(" ")[1:-1]))
			win.line_num += 1
			if len(numbers) > 3000:
				break
		numbers.remove("")
		numbers = list(numbers)[:3000]

	one_digit = [int(i) % 9 + 1 for i in numbers[:1000]]
	two_digits = [int(i) % 90 + 10 for i in numbers[1000:2000]]
	three_digits = [int(i) % 900 + 100 for i in numbers[2000:3000]]

	for i in range(10):
		item = QTableWidgetItem(str(one_digit[i * STEP]))
		table.setItem(i, 0, item)
	for i in range(10):
		item = QTableWidgetItem(str(two_digits[i * STEP]))
		table.setItem(i, 1, item)
	for i in range(10):
		item = QTableWidgetItem(str(three_digits[i * STEP]))
		table.setItem(i, 2, item)

	crit_one = randomCriteria(one_digit)
	crit_two = randomCriteria(two_digits)
	crit_three = randomCriteria(three_digits)
	win.meas_table_1.setText(' {:.4%}'.format(crit_one))
	win.meas_table_2.setText(' {:.4%}'.format(crit_two))
	win.meas_table_3.setText(' {:.4%}'.format(crit_three))

def calcManual(win):
    input = win.manual_input.text().split(" ")
    seq = []
    for x in input:
        try:
            int(x)
        except ValueError:
            continue
        else:
            seq.append(int(x))

    crit = randomCriteria(seq)
    win.meas_manual.setText(' {:.4%}'.format(crit))

if __name__ == "__main__":
    generator = LinearCongruent()
    app = QtWidgets.QApplication(sys.argv)
    w = Window()
    w.show()
    sys.exit(app.exec_())