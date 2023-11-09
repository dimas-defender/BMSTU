from tkinter import messagebox, Frame
from table import Table
from utils import *

class PfeFrame(Frame):
    def __init__(self, master): 
        super().__init__(master)

        self.MainTable = Table(master=self, rows=10, columns=14)
        self.MainTable.pack(padx=10, pady=10)
        self.BTable = Table(master=self, rows=2, columns=8)
        self.BTable.pack(padx=10, pady=10)

        self.BNLinTable = Table(master=self, rows=2, columns=4)
        self.BNLinTable.pack(padx=10, pady=10)

        self.BNTable = Table(master=self, rows=2, columns=8)
        self.BNTable.pack(padx=10, pady=10)

        self.MainTable.set_row(0, ['№', "x0", "x1", "x2", "x3", "x1x2", "x1x3", "x2x3", "x1x2x3", "Y", "Yл", "Yчн", "|Y - Yл|", "|Y - Yчн|"])

        self.BTable.set( 0, 0, "b0")
        self.BTable.set( 0, 1, "b1")
        self.BTable.set( 0, 2, "b2")
        self.BTable.set( 0, 3, "b3")
        self.BTable.set( 0, 4, "b12")
        self.BTable.set( 0, 5, "b13")
        self.BTable.set( 0, 6, "b23")
        self.BTable.set( 0, 7, "b123")

        self.BNLinTable.set( 0, 0, "b0'")
        self.BNLinTable.set( 0, 1, "b1'")
        self.BNLinTable.set( 0, 2, "b2'")
        self.BNLinTable.set( 0, 3, "b3'")

        self.BNTable.set( 0, 0, "b0'")
        self.BNTable.set( 0, 1, "b1'")
        self.BNTable.set( 0, 2, "b2'")
        self.BNTable.set( 0, 3, "b3'")
        self.BNTable.set( 0, 4, "b12'")
        self.BNTable.set( 0, 5, "b13'")
        self.BNTable.set( 0, 6, "b23'")
        self.BNTable.set( 0, 7, "b123'")
        

    def set_x_values(self): 
        for i in range(len(self.x_table)):
            self.MainTable.set_column(i + 1, self.x_table[i])

    def modelling(self): 
        y = []

        for i in range(len(self.x_table[0])):
            result = modelling(
                clients_number=self.count+1000,
                clients_proccessed=self.count,
                lambda_gen=self.lambda_min if self.x_table[1][i] == -1 else self.lambda_max,
                sigma_gen=self.sigma_min if self.x_table[2][i] == -1 else self.sigma_max,
                lambda_proc=self.mu_min if self.x_table[3][i] == -1 else self.mu_max, 
            )
            y.append(result['avg_wait_time'])
        
        return y

    def count_one(self, lam, sig, mu):
        if lam < self.lambda_min or lam > self.lambda_max or sig < self.sigma_min or sig > self.sigma_max or\
              mu < self.mu_min or mu > self.mu_max:
            messagebox.showinfo(title="Ошибка", message="Точка не входит в интервал варьирования!")
            return

        result = modelling(
                clients_number=self.count+1000,
                clients_proccessed=self.count,
                lambda_gen=lam,
                sigma_gen=sig,
                lambda_proc=mu
            )

        x0 = 1
        I_j_lambda = (self.lambda_max - self.lambda_min) / 2
        X_i_avg_lambda = (self.lambda_max + self.lambda_min) / 2
        x1 = (lam - X_i_avg_lambda) / I_j_lambda

        I_j_sigma = (self.sigma_max - self.sigma_min) / 2
        X_i_avg_sigma = (self.sigma_max + self.sigma_min) / 2
        x2 = (sig - X_i_avg_sigma) / I_j_sigma
        
        I_j_mu = (self.mu_max - self.mu_min) / 2
        X_i_avg_mu = (self.mu_max + self.mu_min) / 2
        x3 = (mu - X_i_avg_mu) / I_j_mu

        x12 = x1 * x2
        x13 = x1 * x3
        x23 = x2 * x3
        x123 = x1 * x2 * x3

        line = [x0] + [x1] + [x2] + [x3] + [x12] + [x13] + [x23] + [x123]
        y = result['avg_wait_time']

        s = 0
        l = 4
        for j in range(l): 
            s += line[j] * self.b[j]
        y_lin = s

        s = 0
        l = len(line)
        for j in range(l):
            s += line[j] * self.b[j]
        y_nl = s

        y_lin_per = abs(y - y_lin)
        y_nl_per = abs(y - y_nl)

        line += [y] + [y_lin] + [y_nl] + [y_lin_per] + [y_nl_per]
        self.MainTable.set_row(9, line, 1)

    def run(self, lambda_min, lambda_max, sigma_min, sigma_max, mu_min, mu_max, count):
        self.lambda_max = lambda_max
        self.lambda_min = lambda_min
        self.sigma_max = sigma_max
        self.sigma_min = sigma_min
        self.mu_max = mu_max
        self.mu_min = mu_min
        self.count = count
        exp_count = 8

        x0 = [1 for i in range(exp_count)]
        x1 = [-1 if i % 2 < 1 else 1 for i in range(exp_count)]
        x2 = [-1 if i % 4 < 2 else 1 for i in range(exp_count)]
        x3 = [-1 if i % 8 < 4 else 1 for i in range(exp_count)]
        x12 = [x1[i] * x2[i] for i in range(len(x1))]
        x13 = [x1[i] * x3[i] for i in range(len(x1))]
        x23 = [x2[i] * x3[i] for i in range(len(x1))]
        x123 = [x1[i] * x2[i] * x3[i] for i in range(len(x1))]

        self.x_table = [x0] + [x1] + [x2] + [x3] + [x12] + [x13] + [x23] + [x123]
        self.set_x_values()

        y = self.modelling()

        for i in range(exp_count + 1):
            self.MainTable.set(i + 1, 0, i + 1)

        b0 = self.count_b(x0, y)
        b1 = self.count_b(x1, y)
        b2 = self.count_b(x2, y)
        b3 = self.count_b(x3, y)
        b12 = self.count_b(x12, y)
        b13 = self.count_b(x13, y)
        b23 = self.count_b(x23, y)
        b123 = self.count_b(x123, y)

        b = [b0] + [b1] + [b2] + [b3] + [b12] + [b13] + [b23] + [b123]

        self.MainTable.set_column(9, y)
        self.BTable.set_row(1, b)
        self.b = b

        self.calculate_natural_lin_b()
        self.BNLinTable.set_row(1, self.natural_lin_b)

        self.calculate_natural_nl_b()
        self.BNTable.set_row(1, self.natural_nl_b)

        y_lin = self.count_lin(self.x_table, b, 4)
        y_nl = self.count_lin(self.x_table, b, len(b))
        
        y_lin_per = [abs(y[i] - y_lin[i]) for i in range(len(y))]
        y_nl_per = [abs(y[i] - y_nl[i]) for i in range(len(y))]

        self.MainTable.set_column(10, y_lin)
        self.MainTable.set_column(11, y_nl)
        self.MainTable.set_column(12, y_lin_per)
        self.MainTable.set_column(13, y_nl_per)
        self.MainTable.set_row(9, ['','','','','','','','','','','','',''], 1)


    def count_b(self, x, y):
        sum = 0
        for i in range(len(x)):
            sum += x[i] * y[i]
        return sum / len(x)
    

    def count_lin(self, x_table, b, l):
        y_lin = []
        for i in range(len(x_table)):
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
        diffx1 = x1max - x1min
        sumx1 = x1max + x1min
        diffx2 = x2max - x2min
        sumx2 = x2max + x2min
        diffx3 = x3max - x3min
        sumx3 = x3max + x3min

        nat_lin_b = [0.] * 4

        nat_lin_b[0] = b[0]
        nat_lin_b[0] -= b[1] * sumx1 / diffx1
        nat_lin_b[0] -= b[2] * sumx2 / diffx2
        nat_lin_b[0] -= b[3] * sumx3 / diffx3

        nat_lin_b[1] = 2 * b[1] / diffx1
        nat_lin_b[2] = 2 * b[2] / diffx2
        nat_lin_b[3] = 2 * b[3] / diffx3      

        self.natural_lin_b = nat_lin_b[:]

    
    def calculate_natural_nl_b(self):
        a = self.b[:]
        x1max = self.lambda_max
        x1min = self.lambda_min
        x2max = self.sigma_max
        x2min = self.sigma_min
        x3max = self.mu_max
        x3min = self.mu_min
        diffx1 = x1max - x1min
        sumx1 = x1max + x1min
        diffx2 = x2max - x2min
        sumx2 = x2max + x2min
        diffx3 = x3max - x3min
        sumx3 = x3max + x3min
        nat_nl_b = [0.] * 8

        nat_nl_b[0] = a[0] - a[1] * sumx1 / diffx1 - a[2] * sumx2 / diffx2 - a[3] * sumx3 / diffx3 + \
            a[4] * sumx1 * sumx2 / (diffx1 * diffx2) + a[5] * sumx1 * sumx3 / (diffx1 * diffx3) + a[6] * sumx2 * sumx3 / (diffx2 * diffx3) - \
            a[7] * sumx1 * sumx2 * sumx3 / (diffx1 * diffx2 * diffx3)

        nat_nl_b[1] = 2 * a[1] / diffx1 - 2 * a[4] * sumx2 / (diffx1 * diffx2) - 2 * a[5] * sumx3 / (diffx1 * diffx3) + \
            2 * a[7] * sumx2 * sumx3 / (diffx1 * diffx2 * diffx3)

        nat_nl_b[2] = 2 * a[2] / diffx2 - 2 * a[4] * sumx1 / (diffx1 * diffx2) - 2 * a[6] * sumx3 / (diffx2 * diffx3) + \
            2 * a[7] * sumx1 * sumx3 / (diffx1 * diffx2 * diffx3)

        nat_nl_b[3] = 2 * a[3] / diffx3 - 2 * a[5] * sumx1 / (diffx1 * diffx3) - 2 * a[6] * sumx2 / (diffx2 * diffx3) + \
            2 * a[7] * sumx1 * sumx2 / (diffx1 * diffx2 * diffx3)

        nat_nl_b[4] = 4 * a[4] / (diffx1 * diffx2) - 4 * a[7] * sumx3 / (diffx1 * diffx2 * diffx3)

        nat_nl_b[5] = 4 * a[5] / (diffx1 * diffx3) - 4 * a[7] * sumx2 / (diffx1 * diffx2 * diffx3)

        nat_nl_b[6] = 4 * a[6] / (diffx2 * diffx3) - 4 * a[7] * sumx1 / (diffx1 * diffx2 * diffx3)

        nat_nl_b[7] = 8 * a[7] / (diffx1 * diffx2 * diffx3)

        self.natural_nl_b = nat_nl_b[:]