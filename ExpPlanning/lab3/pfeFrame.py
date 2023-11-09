import tkinter as tk
from table import Table
from utils import *
from math import fabs

class PfeFrame(tk.Frame):
    def __init__(self, master): 
        super().__init__(master)

        canvas = tk.Canvas(self, width=1450, height=300)
        self.MainTable = Table(master=canvas, rows=66, columns=28)
        self.MainTable.grid(column=0, row=0, padx=5, pady=5)
        scrollbar = tk.Scrollbar(self, orient="vertical", command=canvas.yview)

        self.MainTable.bind("<Configure>", lambda e: canvas.configure(scrollregion=canvas.bbox("all")))

        canvas.create_window((0, 0), window=self.MainTable, anchor="nw")
        canvas.configure(yscrollcommand=scrollbar.set)
        canvas.grid(column=0, row=0, sticky="nswe")
        scrollbar.grid(column=1, row=0, sticky="ns")

        self.MainTable.set_row(0, ['№', "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x1x2", "x1x3", "x1x4", "x1x5", 
            "x1x6", "x2x3", "x2x4", "x2x5", "x2x6", "x3x4", "x3x5", "x3x6", "x4x5", "x4x6", "x5x6",  
            "Y", "Yл", "Yчн", "|Y - Yл|", "|Y - Yчн|"])

        self.formula_frame = tk.Frame(master=self, highlightbackground="black", highlightthickness=1)

        self.lin_formula = tk.StringVar()
        self.nl_formula = tk.StringVar()
        self.nat_lin_formula = tk.StringVar()
        self.nat_nl_formula = tk.StringVar()

        lin_label = tk.Label(self.formula_frame, text="Линейная модель: ")
        lin_label.grid(row=0, column=0, sticky="ne")
        lin_formula_label = tk.Label(self.formula_frame, textvariable=self.lin_formula, justify='left')
        lin_formula_label.grid(row=0, column=1, sticky="nw")

        nl_label = tk.Label(self.formula_frame, text="Частично нелинейная модель: ")
        nl_label.grid(row=1, column=0, sticky="ne")
        nl_formula_label = tk.Label(self.formula_frame, textvariable=self.nl_formula, justify='left')
        nl_formula_label.grid(row=1, column=1, sticky="nw")

        nat_lin_label = tk.Label(self.formula_frame, text="Линейная модель (натур.): ")
        nat_lin_label.grid(row=2, column=0, sticky="ne")
        nat_lin_formula_label = tk.Label(self.formula_frame, textvariable=self.nat_lin_formula, justify='left')
        nat_lin_formula_label.grid(row=2, column=1, sticky="nw")

        nat_nl_label = tk.Label(self.formula_frame, text="Частично нелинейная модель (натур.): ")
        nat_nl_label.grid(row=3, column=0, sticky="ne")
        nat_nl_formula_label = tk.Label(self.formula_frame, textvariable=self.nat_nl_formula, justify='left')
        nat_nl_formula_label.grid(row=3, column=1, sticky="nw")

        self.formula_frame.grid(column=0, row=1, pady=20)


    def set_x_values(self): 
        for i in range(len(self.x_table)):
            self.MainTable.set_column(i + 1, self.x_table[i])

    def modelling(self): 
        y = []
        i_lam = (self.lambda_max - self.lambda_min) / 2
        lam0 = (self.lambda_max + self.lambda_min) / 2
        i_sig = (self.sigma_max - self.sigma_min) / 2
        sig0 = (self.sigma_max + self.sigma_min) / 2
        i_mu = (self.mu_max - self.mu_min) / 2
        mu0 = (self.mu_max + self.mu_min) / 2
        
        i_lam2 = (self.lambda2_max - self.lambda2_min) / 2
        lam02 = (self.lambda2_max + self.lambda2_min) / 2
        i_sig2 = (self.sigma2_max - self.sigma2_min) / 2
        sig02 = (self.sigma2_max + self.sigma2_min) / 2
        i_mu2 = (self.mu2_max - self.mu2_min) / 2
        mu02 = (self.mu2_max + self.mu2_min) / 2

        for i in range(len(self.x_table[0])):
            result = modelling(
                clients_number=self.count + 1000,
                clients_proccessed=self.count,
                lambda_gen=self.x_table[1][i] * i_lam + lam0,
                sigma_gen=self.x_table[2][i] * i_sig + sig0,
                lambda_proc=self.x_table[3][i]* i_mu + mu0, 
                lambda_gen2=self.x_table[4][i] * i_lam2 + lam02,
                sigma_gen2=self.x_table[5][i] * i_sig2 + sig02,
                lambda_proc2=self.x_table[6][i]* i_mu2 + mu02,
            )
            y.append(result['avg_wait_time'])

        return y


    def count_one(self, lam, sig, mu, lam2=None, sig2=None, mu2=None):
        if lam < self.lambda_min or lam > self.lambda_max or sig < self.sigma_min or sig > self.sigma_max\
            or mu < self.mu_min or mu > self.mu_max: 
            tk.messagebox.showinfo(title="Ошибка", message="Точка не входит в интервал варьирования!")
            return

        result = modelling(
                clients_number=self.count + 1000,
                clients_proccessed=self.count,
                lambda_gen=lam,
                sigma_gen=sig,
                lambda_proc=mu,
                lambda_gen2=lam2 or lam,
                sigma_gen2=sig2 or sig,
                lambda_proc2=mu2 or mu,
            )

        i_lam = (self.lambda_max - self.lambda_min) / 2
        lam0 = (self.lambda_max + self.lambda_min) / 2
        i_sig = (self.sigma_max - self.sigma_min) / 2
        sig0 = (self.sigma_max + self.sigma_min) / 2
        i_mu = (self.mu_max - self.mu_min) / 2
        mu0 = (self.mu_max + self.mu_min) / 2
        
        i_lam2 = (self.lambda2_max - self.lambda2_min) / 2
        lam02 = (self.lambda2_max + self.lambda2_min) / 2
        i_sig2 = (self.sigma2_max - self.sigma2_min) / 2
        sig02 = (self.sigma2_max + self.sigma2_min) / 2
        i_mu2 = (self.mu2_max - self.mu2_min) / 2
        mu02 = (self.mu2_max + self.mu2_min) / 2

        x0 = 1
        x1 = (lam - lam0) / i_lam
        x2 = (sig - sig0) / i_sig
        x3 = (mu - mu0) / i_mu
        x4 = (lam2  - lam02) / i_lam2
        x5 = (sig2 - sig02) / i_sig2
        x6 = (mu2 - mu02) / i_mu2
        x12 = x1 * x2
        x13 = x1 * x3
        x14 = x1 * x4
        x15 = x1 * x5
        x16 = x1 * x6
        x23 = x2 * x3
        x24 = x2 * x4
        x25 = x2 * x5
        x26 = x2 * x6
        x34 = x3 * x4 
        x35 = x3 * x5
        x36 = x3 * x6
        x45 = x4 * x5
        x46 = x4 * x6
        x56 = x5 * x6

        line = [x0] + [x1] + [x2] + [x3] + [x4] + [x5] + [x6] + [x12] + [x13] + [x14] + [x15] + [x16] +\
               [x23] + [x24] + [x25] + [x26] + [x34] + [x35] + [x36] + [x45] + [x46] + [x56]
        
        y = result['avg_wait_time']

        s = 0
        for j in range(7):
            s += line[j] * self.b[j]
        y_lin = s

        s = 0
        for j in range(len(line)): 
            s += line[j] * self.b[j]
        y_nl = s

        y_lin_per = abs(y - y_lin)
        y_nl_per = abs(y - y_nl)

        line += [y] + [y_lin] + [y_nl] + [y_lin_per] + [y_nl_per]
        self.MainTable.set_row(65, line, 1)


    def run(self, lambda_min, lambda_max, sigma_min, sigma_max, mu_min, mu_max,
            count, lambda2_min, lambda2_max, sigma2_min, sigma2_max, mu2_min, mu2_max):
        self.lambda_max = lambda_max
        self.lambda_min = lambda_min
        self.sigma_max = sigma_max
        self.sigma_min = sigma_min
        self.mu_max = mu_max
        self.mu_min = mu_min
        self.lambda2_max = lambda2_max
        self.lambda2_min = lambda2_min
        self.sigma2_max = sigma2_max
        self.sigma2_min = sigma2_min
        self.mu2_max = mu2_max
        self.mu2_min = mu2_min
        self.count = count
        lin_count = 6
        N0 = 2 ** lin_count

        x0 = [1 for i in range(N0)]
        x1 = [-1 if i % 2 < 1 else 1 for i in range(N0)]
        x2 = [-1 if i % 4 < 2 else 1 for i in range(N0)]
        x3 = [-1 if i % 8 < 4 else 1 for i in range(N0)]
        x4 = [-1 if i % 16 < 8 else 1 for i in range(N0)]
        x5 = [-1 if i % 32 < 16 else 1 for i in range(N0)]
        x6 = [-1 if i % 64 < 32 else 1 for i in range(N0)]
        x12 = [x1[i]*x2[i] for i in range(len(x1))]
        x13 = [x1[i]*x3[i] for i in range(len(x1))]
        x14 = [x1[i]*x4[i] for i in range(len(x1))]
        x15 = [x1[i]*x5[i] for i in range(len(x1))]
        x16 = [x1[i]*x6[i] for i in range(len(x1))]
        x23 = [x2[i]*x3[i] for i in range(len(x2))]
        x24 = [x2[i]*x4[i] for i in range(len(x2))]
        x25 = [x2[i]*x5[i] for i in range(len(x1))]
        x26 = [x2[i]*x6[i] for i in range(len(x1))]
        x34 = [x3[i]*x4[i] for i in range(len(x2))]
        x35 = [x3[i]*x5[i] for i in range(len(x1))]
        x36 = [x3[i]*x6[i] for i in range(len(x1))]
        x45 = [x4[i]*x5[i] for i in range(len(x1))]
        x46 = [x4[i]*x6[i] for i in range(len(x1))]
        x56 = [x5[i]*x6[i] for i in range(len(x1))]

        for i in range(N0 + 1):
            self.MainTable.set(i + 1, 0, i + 1)

        self.x_table = [x0] + [x1] + [x2] + [x3] + [x4] + [x5] + [x6] + [x12] + [x13] + [x14] + [x15] + [x16] +\
               [x23] + [x24] + [x25] + [x26] + [x34] + [x35] + [x36] + [x45] + [x46] + [x56]

        self.set_x_values()

        y = self.modelling()

        b = []
        for i in range(len(self.x_table)):
            b.append(self.count_b(self.x_table[i], y))

        self.MainTable.set_column(23, y)
        self.b = b

        y_lin = self.count_lin(self.x_table, b, lin_count + 1)
        y_nl = self.count_lin(self.x_table, b, len(b))
        
        y_lin_per = [abs(y[i] - y_lin[i]) for i in range(len(y))]
        y_nl_per = [abs(y[i] - y_nl[i]) for i in range(len(y))]

        self.MainTable.set_column(24, y_lin)
        self.MainTable.set_column(25, y_nl)
        self.MainTable.set_column(26, y_lin_per)
        self.MainTable.set_column(27, y_nl_per)
        #self.MainTable.set_row(65, ['','','','','','','','','','','','',''], 1)

        lin_str = "y = " + str('{:.3g}'.format(b[0]))
        for i in range (1, lin_count + 1): 
            if (b[i] > 0):
                lin_str += " + " + str('{:.3g}'.format(b[i])) + " * x" + str(i)
            else: 
                lin_str += " - " + str('{:.3g}'.format(fabs(b[i]))) + " * x" + str(i)

        x_indexes = ["0", "1", "2", "3", "4", "5", "6", "1x2", "1x3", "1x4", \
                     "1x5", "1x6", "2x3", "2x4", "2x5", "2x6", "3x4", "3x5", "3x6", "4x5", "4x6", "5x6"]
        
        nl_str = "y = " + str('{:.3g}'.format(b[0]))
        for i in range (1, len(b)):
            if i == round(len(b) / 2):
                nl_str += '\n'
            if (b[i] > 0):
                nl_str += " + " + str('{:.3g}'.format(b[i])) + " * x" + x_indexes[i]
            else:
                nl_str += " - " + str('{:.3g}'.format(fabs(b[i]))) + " * x" + x_indexes[i]

        self.lin_formula.set(lin_str)
        self.nl_formula.set(nl_str)

        self.calculate_natural_lin_b()
        b = self.natural_lin_b

        nat_lin_str = "y = " + str('{:.3g}'.format(b[0]))
        for i in range (1, lin_count + 1): 
            if (b[i] > 0):
                nat_lin_str += " + " + str('{:.3g}'.format(b[i])) + " * x" + str(i)
            else: 
                nat_lin_str += " - " + str('{:.3g}'.format(fabs(b[i]))) + " * x" + str(i)

        self.calculate_natural_nl_b()
        b = self.natural_nl_b

        nat_nl_str = "y = " + str('{:.3g}'.format(b[0]))
        for i in range (1, len(b)):
            if i == round(len(b) / 2):
                nat_nl_str += '\n'
            if (b[i] > 0):
                nat_nl_str += " + " + str('{:.3g}'.format(b[i])) + " * x" + x_indexes[i]
            else:
                nat_nl_str += " - " + str('{:.3g}'.format(fabs(b[i]))) + " * x" + x_indexes[i]

        self.nat_lin_formula.set(nat_lin_str)
        self.nat_nl_formula.set(nat_nl_str)


    def count_b(self, x, y):
        sum = 0
        for i in range(len(x)):
            sum += x[i] * y[i]
        return sum / len(x)
    

    def count_lin(self, x_table, b, l):
        y_lin = []
        for i in range(len(x_table[0])):
            y = 0
            for j in range(l):
                y += x_table[j][i] * b[j]
            y_lin.append(y)
        return y_lin
    

    def calculate_natural_lin_b(self):
        b = self.b[:]
        x1max = self.lambda_max
        x1min = self.lambda_min
        x2max = self.sigma_max
        x2min = self.sigma_min
        x3max = self.mu_max
        x3min = self.mu_min
        x4max = self.lambda2_max
        x4min = self.lambda2_min
        x5max = self.sigma2_max
        x5min = self.sigma2_min
        x6max = self.mu2_max
        x6min = self.mu2_min
        
        diffx1 = x1max - x1min
        sumx1 = x1max + x1min
        diffx2 = x2max - x2min
        sumx2 = x2max + x2min
        diffx3 = x3max - x3min
        sumx3 = x3max + x3min
        diffx4 = x4max - x4min
        sumx4 = x4max + x4min
        diffx5 = x5max - x5min
        sumx5 = x5max + x5min
        diffx6 = x6max - x6min
        sumx6 = x6max + x6min

        nat_lin_b = [0.] * 7

        nat_lin_b[0] = b[0]
        nat_lin_b[0] -= b[1] * sumx1 / diffx1
        nat_lin_b[0] -= b[2] * sumx2 / diffx2
        nat_lin_b[0] -= b[3] * sumx3 / diffx3
        nat_lin_b[0] -= b[4] * sumx4 / diffx4
        nat_lin_b[0] -= b[5] * sumx5 / diffx5
        nat_lin_b[0] -= b[6] * sumx6 / diffx6

        nat_lin_b[1] = 2 * b[1] / diffx1
        nat_lin_b[2] = 2 * b[2] / diffx2
        nat_lin_b[3] = 2 * b[3] / diffx3
        nat_lin_b[4] = 2 * b[4] / diffx4
        nat_lin_b[5] = 2 * b[5] / diffx5
        nat_lin_b[6] = 2 * b[6] / diffx6

        self.natural_lin_b = nat_lin_b[:]


    def calculate_natural_nl_b(self):
        a = self.b[:]
        x1max = self.lambda_max
        x1min = self.lambda_min
        x2max = self.sigma_max
        x2min = self.sigma_min
        x3max = self.mu_max
        x3min = self.mu_min
        x4max = self.lambda2_max
        x4min = self.lambda2_min
        x5max = self.sigma2_max
        x5min = self.sigma2_min
        x6max = self.mu2_max
        x6min = self.mu2_min
        
        diffx1 = x1max - x1min
        sumx1 = x1max + x1min
        diffx2 = x2max - x2min
        sumx2 = x2max + x2min
        diffx3 = x3max - x3min
        sumx3 = x3max + x3min
        diffx4 = x4max - x4min
        sumx4 = x4max + x4min
        diffx5 = x5max - x5min
        sumx5 = x5max + x5min
        diffx6 = x6max - x6min
        sumx6 = x6max + x6min

        i = {'1': 1, '2': 2, '3': 3, '4': 4, '5': 5, '6': 6,
            '12': 7, '13': 8, '14': 9, '15': 10, '16': 11,
            '23': 12, '24': 13, '25': 14, '26': 15, '34': 16,
            '35': 17, '36': 18, '45': 19, '46': 20, '56': 21}

        s = {'1': sumx1, '2': sumx2, '3': sumx3, '4': sumx4, '5': sumx5, '6': sumx6,
            '12': sumx1 * sumx2, '13': sumx1 * sumx3, '14': sumx1 * sumx4, '15': sumx1 * sumx5, '16': sumx1 * sumx6,
            '23': sumx3 * sumx2, '24': sumx4 * sumx2, '25': sumx5 * sumx2, '26': sumx6 * sumx2, '34': sumx3 * sumx4,
            '35': sumx3 * sumx5, '36': sumx3 * sumx6, '45': sumx4 * sumx5, '46': sumx4 * sumx6, '56': sumx5 * sumx6}

        d = {'1': diffx1, '2': diffx2, '3': diffx3, '4': diffx4, '5': diffx5, '6': diffx6,
            '12': diffx1 * diffx2, '13': diffx1 * diffx3, '14': diffx1 * diffx4, '15': diffx1 * diffx5, '16': diffx1 * diffx6,
            '23': diffx3 * diffx2, '24': diffx4 * diffx2, '25': diffx5 * diffx2, '26': diffx6 * diffx2, '34': diffx3 * diffx4,
            '35': diffx3 * diffx5, '36': diffx3 * diffx6, '45': diffx4 * diffx5, '46': diffx4 * diffx6, '56': diffx5 * diffx6}

        nat_nl_b = [0.] * 22

        nat_nl_b[0] = a[0] - \
            a[i['1']] * sumx1 / diffx1 - a[i['2']] * sumx2 / diffx2 - a[i['3']] * sumx3 / diffx3 - \
            a[i['4']] * sumx4 / diffx4 - a[i['5']] * sumx5 / diffx5 - a[i['6']] * sumx6 / diffx6 + \
            a[i['12']] * s['12'] / d['12'] + \
            a[i['13']] * s['13'] / d['13'] + a[i['14']] * s['14'] / d['14'] + \
            a[i['15']] * s['15'] / d['15'] + a[i['16']] * s['16'] / d['16'] + \
            a[i['23']] * s['23'] / d['23'] + a[i['24']] * s['24'] / d['24'] + \
            a[i['25']] * s['25'] / d['25'] + a[i['26']] * s['26'] / d['26'] + \
            a[i['34']] * s['34'] / d['34'] + a[i['35']] * s['35'] / d['35'] + \
            a[i['36']] * s['36'] / d['36'] + a[i['45']] * s['45'] / d['45'] + \
            a[i['46']] * s['46'] / d['46'] + a[i['56']] * s['56'] / d['56']

        nat_nl_b[i['1']] = 2 * a[i['1']] / d['1'] - 2 * a[i['12']] * s['2'] / d['12'] - \
            2 * a[i['13']] * s['3'] / d['13'] - 2 * a[i['14']] * s['4'] / d['14'] - \
            2 * a[i['15']] * s['5'] / d['15'] - 2 * a[i['16']] * s['6'] / d['16']

        nat_nl_b[i['2']] = 2 * a[i['2']] / d['2'] - 2 * a[i['12']] * s['1'] / d['12'] - \
            2 * a[i['23']] * s['3'] / d['23'] - 2 * a[i['24']] * s['4'] / d['24'] - \
            2 * a[i['25']] * s['5'] / d['25'] - 2 * a[i['26']] * s['6'] / d['26']

        nat_nl_b[i['3']] = 2 * a[i['3']] / d['3'] - 2 * a[i['13']] * s['1'] / d['13'] - \
            2 * a[i['23']] * s['2'] / d['23'] - 2 * a[i['34']] * s['4'] / d['34'] - \
            2 * a[i['35']] * s['5'] / d['35'] - 2 * a[i['36']] * s['6'] / d['36']

        nat_nl_b[i['4']] = 2 * a[i['4']] / d['4'] - 2 * a[i['14']] * s['1'] / d['14'] - \
            2 * a[i['24']] * s['2'] / d['24'] - 2 * a[i['34']] * s['3'] / d['34'] - \
            2 * a[i['45']] * s['5'] / d['45'] - 2 * a[i['46']] * s['6'] / d['46']

        nat_nl_b[i['5']] = 2 * a[i['5']] / d['5'] - 2 * a[i['15']] * s['1'] / d['15'] - \
            2 * a[i['25']] * s['2'] / d['25'] - 2 * a[i['35']] * s['3'] / d['35'] - \
            2 * a[i['45']] * s['4'] / d['45'] - 2 * a[i['56']] * s['6'] / d['56']

        nat_nl_b[i['6']] = 2 * a[i['6']] / d['6'] - 2 * a[i['16']] * s['1'] / d['16'] - \
            2 * a[i['26']] * s['2'] / d['26'] - 2 * a[i['36']] * s['3'] / d['36'] - \
            2 * a[i['46']] * s['4'] / d['46'] - 2 * a[i['56']] * s['5'] / d['56']

        nat_nl_b[i['12']] = 4 * a[i['12']] / d['12']

        nat_nl_b[i['13']] = 4 * a[i['13']] / d['13']

        nat_nl_b[i['14']] = 4 * a[i['14']] / d['14']

        nat_nl_b[i['15']] = 4 * a[i['15']] / d['15']

        nat_nl_b[i['16']] = 4 * a[i['16']] / d['16']

        nat_nl_b[i['23']] = 4 * a[i['23']] / d['23']

        nat_nl_b[i['24']] = 4 * a[i['24']] / d['24']

        nat_nl_b[i['25']] = 4 * a[i['25']] / d['25']

        nat_nl_b[i['26']] = 4 * a[i['26']] / d['26']

        nat_nl_b[i['34']] = 4 * a[i['34']] / d['34']

        nat_nl_b[i['35']] = 4 * a[i['35']] / d['35']

        nat_nl_b[i['36']] = 4 * a[i['36']] / d['36']

        nat_nl_b[i['45']] = 4 * a[i['45']] / d['45']

        nat_nl_b[i['46']] = 4 * a[i['46']] / d['46']

        nat_nl_b[i['56']] = 4 * a[i['56']] / d['56']

        self.natural_nl_b = nat_nl_b[:]