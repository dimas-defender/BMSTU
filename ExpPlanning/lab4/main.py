from tkinter import *
from tkinter import messagebox
import input as i
from utils import *
from pfeFrame import *


root = Tk()
root.title("ОЦКП")
experiment = PfeFrame(root)

experiment.configure(background = ITEMS_COLOR)
root.configure(background = BG_COLOR)

varList = {
    "lambda": StringVar(),
    "lambda2": StringVar(),
    "sigma": StringVar(),
    "sigma2": StringVar(),
    "mu": StringVar(),
    "mu2": StringVar(),
    "N": StringVar(),
    "lambda_min": StringVar(), 
    "lambda_max": StringVar(),
    "lambda2_min": StringVar(), 
    "lambda2_max": StringVar(),
    "sigma_min": StringVar(),
    "sigma_max": StringVar(),
    "sigma2_min": StringVar(),
    "sigma2_max": StringVar(),
    "mu_min": StringVar(),
    "mu_max": StringVar(),
    "mu2_min": StringVar(),
    "mu2_max": StringVar(),
}

def work_one(Event): 
    lam = float(varList["lambda"].get())
    lam2 = float(varList["lambda2"].get())
    sig = float(varList["sigma"].get())
    sig2 = float(varList["sigma2"].get())
    mu = float(varList["mu"].get())
    mu2 = float(varList["mu2"].get())
    experiment.count_one(lam, sig, mu, lam2, sig2, mu2)

def work_pfe(Event):
    try:
        lambda_min = float(varList["lambda_min"].get())
        lambda_max = float(varList["lambda_max"].get())
        sigma_min = float(varList["sigma_min"].get())
        sigma_max = float(varList["sigma_max"].get())
        mu_min = float(varList["mu_min"].get())
        mu_max = float(varList["mu_max"].get())
        lambda2_min = float(varList["lambda2_min"].get())
        lambda2_max = float(varList["lambda2_max"].get())
        sigma2_min = float(varList["sigma2_min"].get())
        sigma2_max = float(varList["sigma2_max"].get())        
        mu2_min = float(varList["mu2_min"].get())
        mu2_max = float(varList["mu2_max"].get())
        count = float(varList["N"].get())

        experiment.run(
            lambda_min, lambda_max,
            sigma_min, sigma_max,
            mu_min, mu_max, count,
            lambda2_min, lambda2_max,
            sigma2_min, sigma2_max,
            mu2_min, mu2_max
        )        
        add_button.config(state='normal')

    except ValueError:
        messagebox.showinfo(title="Ошибка", message="Некорректные параметры!")


def pfe_inputs(root):
    frame_inputs = Frame(root, bg=ITEMS_COLOR)
    items_1 = [
        i.Item(text="Минимум:", var=varList["lambda_min"], value=0.4), 
        i.Item(text="Максимум:", var=varList["lambda_max"], value=0.45), 
    ]
    items_2 = [
        i.Item(text="Минимум:", var=varList["sigma_min"], value=0.001),
        i.Item(text="Максимум:", var=varList["sigma_max"], value=0.003), 
    ]
    items_3 = [
        i.Item(text="Минимум:", var=varList["lambda2_min"], value=0.4), 
        i.Item(text="Максимум:", var=varList["lambda2_max"], value=0.45), 
    ]
    items_4 = [
        i.Item(text="Минимум:", var=varList["sigma2_min"], value=0.001), 
        i.Item(text="Максимум:", var=varList["sigma2_max"], value=0.003),
    ]
    items_5 = [
        i.Item(text="Минимум:", var=varList["mu_min"], value=0.98), 
        i.Item(text="Максимум:", var=varList["mu_max"], value=1.02), 
    ]
    items_6 = [
        i.Item(text="Минимум:", var=varList["mu2_min"], value=0.98), 
        i.Item(text="Максимум:", var=varList["mu2_max"], value=1.02),
    ]
    
    i_list_1 = i.InputList(master=frame_inputs, items=items_1, title="Интенсивность поступления заявок 1")
    i_list_2 = i.InputList(master=frame_inputs, items=items_2, title="Разброс интенсивности поступления заявок 1")
    i_list_3 = i.InputList(master=frame_inputs, items=items_3, title="Интенсивность поступления заявок 2")
    i_list_4 = i.InputList(master=frame_inputs, items=items_4, title="Разброс интенсивности поступления заявок 2")
    i_list_5 = i.InputList(master=frame_inputs, items=items_5, title="Интенсивность обработки заявок 1")
    i_list_6 = i.InputList(master=frame_inputs, items=items_6, title="Интенсивность обработки заявок 2")

    i_list_1.pack(side=LEFT, padx=10, pady=10)
    i_list_2.pack(side=LEFT, padx=10, pady=10)
    i_list_3.pack(side=LEFT, padx=10, pady=10)
    i_list_4.pack(side=LEFT, padx=10, pady=10)
    i_list_5.pack(side=LEFT, padx=10, pady=10)
    i_list_6.pack(side=LEFT, padx=10, pady=10)

    frame_inputs.grid(column=1)

    items_7 = [
        i.Item(text="Количество заявок:", var=varList["N"], value=5000),
    ]

    i_list_7 = i.InputList(master=root, items=items_7)
    i_list_7.grid(column=1, padx=10, pady=10)

    btn = Button(root, text="Запуск", width=20, height=2)
    btn.bind("<Button-1>", work_pfe)
    btn.grid(column=1, padx=10, pady=10)


def draw_new_point(root):
    items = [
        i.Item(text="Интенсивность поступления заявок 1:", var=varList["lambda"], value=0.42),
        i.Item(text="Разброс интенсивности поступления заявок 1:", var=varList["sigma"], value=0.0017),
        i.Item(text="Интенсивность поступления заявок 2:", var=varList["lambda2"], value=0.42),
        i.Item(text="Разброс интенсивности поступления заявок 2:", var=varList["sigma2"], value=0.0017),
        i.Item(text="Интенсивность обслуживания заявок 1:", var=varList["mu"], value=0.99),
        i.Item(text="Интенсивность обслуживания заявок 2:", var=varList["mu2"], value=0.99),
    ]
    i_list = i.InputList(master=root, items=items, title="Добавление точки факторного пространства")
    i_list.grid(column=1)

    btn = Button(root, text="Добавить", state=DISABLED, width=15)
    btn.bind("<Button-1>", work_one)
    btn.grid(column=1, padx=10, pady=10)
    return btn

if __name__ == '__main__':
    f_pfe = Frame(root, highlightbackground="black", highlightthickness=1, background=ITEMS_COLOR)
    f_one = Frame(root, highlightbackground="black", highlightthickness=1, background=ITEMS_COLOR)
    pfe_inputs(f_pfe)
    add_button = draw_new_point(f_one)
    f_pfe.grid(row=0, column=0, padx=100, pady=10)
    f_one.grid(row=1, column=0, padx=100, pady=10)
    experiment.grid(row=2, column=0, columnspan=2, padx=10, pady=10)
    root.mainloop()