from tkinter import *
import input as i
from utils import *
from pfeFrame import *

root = Tk()
root.title("ПФЭ")
experiment = PfeFrame(root)
experiment.configure(background = ITEMS_COLOR)
root.configure(background = BG_COLOR)

varList = {
    "lambda": StringVar(),
    "sigma": StringVar(),
    "mu": StringVar(),
    "N": StringVar(), 
    "lambda_min": StringVar(), 
    "lambda_max": StringVar(),
    "sigma_min": StringVar(), 
    "sigma_max": StringVar(),
    "mu_min": StringVar(),
    "mu_max": StringVar(),
}

def work_pfe(Event):
    try:
        lambda_min = float(varList["lambda_min"].get())
        lambda_max = float(varList["lambda_max"].get())
        sigma_min = float(varList["sigma_min"].get())
        sigma_max = float(varList["sigma_max"].get())
        mu_min = float(varList["mu_min"].get())
        mu_max = float(varList["mu_max"].get())
        count = float(varList["N"].get())

        experiment.run(lambda_min, lambda_max, sigma_min, sigma_max, mu_min, mu_max, count)
        add_button.config(state='normal')
    except ValueError:
        messagebox.showinfo(title="Ошибка", message="Некорректные параметры!")


def work_one(Event): 
    lam = float(varList["lambda"].get())
    sig = float(varList["sigma"].get())
    mu = float(varList["mu"].get())
    experiment.count_one(lam, sig, mu)


def pfe_inputs(root):
    t = Label(root, text="Полный факторный эксперимент")
    t.grid(column=1, padx=10, pady=10)

    frame_inputs = Frame(root, bg=ITEMS_COLOR)
    items_1 = [
        i.Item(text="Минимум:", var=varList["lambda_min"], value=0.03), 
        i.Item(text="Максимум:", var=varList["lambda_max"], value=0.05), 
    ]
    items_2 = [
        i.Item(text="Минимум:", var=varList["sigma_min"], value=0),
        i.Item(text="Максимум:", var=varList["sigma_max"], value=0.01),
    ]
    items_3 = [
        i.Item(text="Минимум:", var=varList["mu_min"], value=0.099),
        i.Item(text="Максимум:", var=varList["mu_max"], value=0.101), 
    ]

    i_list_1 = i.InputList(master=frame_inputs, items=items_1, title="Интенсивность поступления заявок")
    i_list_2 = i.InputList(master=frame_inputs, items=items_2, title="Разброс интенсивности поступления заявок")
    i_list_3 = i.InputList(master=frame_inputs, items=items_3, title="Интенсивность обработки заявок")

    i_list_1.pack(side=LEFT, padx=10, pady=10)
    i_list_2.pack(side=LEFT, padx=10, pady=10)
    i_list_3.pack(side=LEFT, padx=10, pady=10)

    frame_inputs.grid(column=1)

    items_4 = [
        i.Item(text="Количество заявок:", var=varList["N"], value=5000),
    ]

    i_list_4 = i.InputList(master=root, items=items_4)
    i_list_4.grid(column=1, padx=10, pady=10)

    btn = Button(root, text="Запуск")
    btn.bind("<Button-1>", work_pfe)
    btn.grid(column=1, padx=10, pady=10)


def draw_new_point(root):
    items = [
        i.Item(text="Интенсивность поступления заявок:", var=varList["lambda"], value=0.04),
        i.Item(text="Разброс интенсивности поступления заявок:", var=varList["sigma"], value=0.005),
        i.Item(text="Интенсивность обработки заявок:", var=varList["mu"], value=0.1),
    ]
    i_list = i.InputList(master=root, items=items, title="Добавление точки факторного пространства")
    i_list.grid(column=1)

    btn = Button(root, text="Добавить", state=DISABLED)
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