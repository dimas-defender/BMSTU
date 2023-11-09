from numpy.random import exponential, uniform
import matplotlib.pyplot as plt
from math import sqrt
from PyQt5.QtWidgets import QApplication, QMainWindow, QMessageBox
from mainwindow import Ui_MainWindow

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

def add_event(events, event):
    i = 0
    while i < len(events) and events[i][0] < event[0]:
        i += 1
    events.insert(i, event)

def event_model(generator, processor, total_tasks):
    processed_tasks = 0
    cur_qlen = 0
    max_qlen = 0
    initGenTime = generator.generate()
    events = [[initGenTime, 'g']]
    totalProcTime = 0
    totalGenTime = initGenTime
    totalStayTime = 0
    free, process_flag = True, False
    generated_tasks = 0

    while processed_tasks < total_tasks:
        event = events.pop(0)
        #print('EVENT', *event, cur_qlen)

        if event[1] == 'g' and generated_tasks < total_tasks:
            cur_qlen += 1
            generated_tasks += 1
            totalStayTime -= event[0]
            if cur_qlen > max_qlen:
                max_qlen = cur_qlen
            genTime = generator.generate()
            totalGenTime += genTime
            add_event(events, [event[0] + genTime, 'g'])
            if free:
                process_flag = True
        elif event[1] == 'p':
            processed_tasks += 1
            cur_qlen -= 1
            totalStayTime += event[0]
            process_flag = True
        if process_flag:
            if cur_qlen > 0:
                #cur_qlen -= 1
                procTime = processor.generate()
                totalProcTime += procTime
                add_event(events, [event[0] + procTime, 'p'])
                free = False
            else:
                free = True
            process_flag = False

    avgStayTime = totalStayTime / total_tasks
    p_fact = totalProcTime / totalGenTime

    return max_qlen, processed_tasks, p_fact, avgStayTime

def graph():
    N = 50
    i = 0.001
    load = []
    time = []
    time_i = []
    load_i = []
    tasks = 1000

    while i < 1.01:
        k = 10

        timeGen = k / i
        sigmaGen = 0.2 * timeGen
        a = timeGen - sqrt(3) * sigmaGen
        b = timeGen + sqrt(3) * sigmaGen

        generator = EvenDistribution(a, b)
        processor = ExponentialDistribution(k)
        
        for j in range(N):
            result = event_model(generator, processor, tasks)
            p = result[2]
            avgTime = result[3]
            load_i.append(p)
            time_i.append(avgTime)

        time.append(sum(time_i) / N)
        load.append(sum(load_i) / N)
        time_i.clear()
        load_i.clear()

        if i < 0.1:
            i += 0.01
        else:
            i += 0.05

    plt.plot(load, time, '#d3b3ff')
    plt.grid(True)
    plt.title("Генератор: равномерный, ОА: экспоненциальный")
    plt.ylabel('Время пребывания заявки в системе')
    plt.xlabel('Загрузка системы')
    plt.show()

class mywindow(QMainWindow):
    def __init__(self):
        super(mywindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.ui.pushButton_model.clicked.connect(self.onModelBtnClick)
        self.ui.pushButton_graph.clicked.connect(graph)

    def onModelBtnClick(self):
        try:
            intensGen = self.ui.spinbox_intensivity_gen.value()
            sigmaGen = self.ui.spinbox_intensivity_gen_range.value()
            intensProc = self.ui.spinbox_intensivity_oa.value()
            tasks = self.ui.spinbox_tasks.value()

            if intensGen == 0 or intensProc == 0:
                raise Exception

            timeGen = 1 / intensGen
            a = timeGen - sqrt(3) * sigmaGen
            b = timeGen + sqrt(3) * sigmaGen
            lambd = 1 / intensProc

            generator = EvenDistribution(a, b)
            processor = ExponentialDistribution(lambd)
            event_res = event_model(generator, processor, tasks)
            #print('Max QLen ',event_res[0], 'Load',event_res[2], 'AvgTime',event_res[3])

            p_teor = round(intensGen / intensProc, 2)
            p_fact = round(event_res[2], 2)
            self.ui.p_teor.setText(str(p_teor))
            self.ui.p_fact.setText(str(p_fact))
        except:
            msgBox = QMessageBox()
            msgBox.setText('Произошла ошибка!')
            msgBox.show()
            msgBox.exec()

if __name__ == "__main__":
    app = QApplication([])
    w = mywindow()
    w.show()
    app.exec()