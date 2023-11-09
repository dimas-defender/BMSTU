#ИУ7-44Б Жабин Дмитрий
#На плоскости заданы множество точек А и множество прямых В. Найти две такие
#различные точки из А, что проходящая через них прямая параллельна наибольшему
#количеству прямых из В.

from tkinter import *
import tkinter.messagebox as box

button_color = 'Khaki'
window_color = 'Orange'
label_color = 'Khaki'

wd = Tk()
wd.geometry('500x700')
wd['bg'] = window_color
wd.title("Графический экран")

can = Canvas(width = 500, height = 500, bg = 'yellow')
can.pack()

X = []
Y = []
K = []
B = []
dots = []
lines = []
res = []
n = 0

e1 = Entry(wd, justify = 'center', font = "Arial 13", width = '20')
e1.place(anchor = CENTER, relx = 0.25, rely = 0.75)
e2 = Entry(wd, justify = 'center', font = "Arial 13", width = '20')
e2.place(anchor = CENTER, relx = 0.7, rely = 0.75)
e3 = Entry(wd, justify = 'center', font = "Arial 13", width = '20')
e3.place(anchor = CENTER, relx = 0.7, rely = 0.8)

def add_dot():
    try:
        a = str(e1.get()).split()
        x = int(a[0])
        y = int(a[1])
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести 2 целых числа')
        e1.delete(0, 'end')
        return
    dot = can.create_oval(x - 2, y - 2, x + 3, y + 3, fill = 'green')
    global n
    n += 1
    X.append(x)
    Y.append(y)
    dots.append(dot)
    dots_list.insert(END, '{:5d}  {:5d}  {:5d}'.format(n, x, y))
    e1.delete(0, 'end')

def y(x1, y1, x2, y2, x):
    return (y2 - y1) / (x2 - x1) * x + y1 - x1 * (y2 - y1) / (x2 - x1)

def add_line():
    try:
        a = str(e2.get()).split()
        x1 = int(a[0])
        y1 = int(a[1])
        b = str(e3.get()).split()
        x2 = int(b[0])
        y2 = int(b[1])
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести 2 пары целых чисел')
        e2.delete(0, 'end')
        e3.delete(0, 'end')
        return
    if x1 == x2:
        line = can.create_line(x1, 0, x1, 500, fill = 'green')
        K.append(10000)
        B.append(0)
    else:
        yL = y(x1, y1, x2, y2, 0)
        yR = y(x1, y1, x2, y2, 500)
        line = can.create_line(0, yL, 500, yR, fill = 'green')
        K.append((y2 - y1) / (x2 - x1))
        B.append(y1 - x1 * (y2 - y1) / (x2 - x1))
    lines.append(line)
    e2.delete(0, 'end')
    e3.delete(0, 'end')
    
def make_dot(event):
    dot = can.create_oval(event.x - 2, event.y - 2, event.x + 3, event.y + 3, fill = 'green')
    X.append(event.x)
    Y.append(event.y)
    global n
    n += 1
    dots_list.insert(END, '{:5d}  {:5d}  {:5d}'.format(n, event.x, event.y))
    dots.append(dot)

def delete_dot():
    try:
        i = dots_list.curselection()[0] - 1
    except:
        box.showerror('Ошибка', 'Необходимо выбрать точку для удаления')
        return
    can.delete(dots[i])
    X.pop(i)
    Y.pop(i)
    dots.pop(i)
    dots_list.delete(i + 1)

def delete_line(i):
    can.delete(lines[i])
    K.pop(i)
    B.pop(i)

def clear():
    for x in dots:
        can.delete(x)
    for y in lines:
        can.delete(y)
    for z in res:
        can.delete(z)
    dots_list.delete(1, len(X))
    X.clear()
    Y.clear()
    K.clear()
    B.clear()
    res.clear()
    dots.clear()
    lines.clear()

def show():
    N = len(X)
    max_cnt = 0
    for i in range(N):
        for j in range(i + 1, N):
            cnt = 0
            if X[j] == X[i]:
                k = 10000
            else:
                k = (Y[j] - Y[i]) / (X[j] - X[i])
            for c in K:
                if c == k:
                    cnt += 1
            if cnt > max_cnt:
                if k == 10000:
                    flag = 1
                else:
                    flag = 0
                max_cnt = cnt
                i_max = i
                j_max = j
    if max_cnt == 0:
        box.showinfo('Нет решения', 'Через данные точки не проходит ни одной прямой, параллельной хотя бы одной из данных прямых')
    else:
        if flag == 1:
            line = can.create_line(X[i_max], 0, X[i_max], 500, fill = 'red')
        else:   
            yL = y(X[i_max], Y[i_max], X[j_max], Y[j_max], 0)
            yR = y(X[i_max], Y[i_max], X[j_max], Y[j_max], 500)
            line = can.create_line(0, yL, 500, yR, fill = 'red')
        res.append(line)

list_wd = Tk()
list_wd.geometry('180x200')
list_wd.title('Точки')
dots_list = Listbox(list_wd)
dots_list.insert(END, '    №    x       y ')
dots_list.pack(side = TOP)
b5 = Button(list_wd, text = 'Удалить точку', width = 14, command = delete_dot, font = "Arial 13")
b5.place(anchor = CENTER, relx = 0.5, rely = 0.9)           
                   
can.bind('<Button-1>', make_dot)
b1 = Button(wd, text = 'Добавить точку', width = 15, command = add_dot, font = "Arial 13", bg = button_color)
b1.place(anchor = CENTER, relx = 0.25, rely = 0.81)
b2 = Button(wd, text = 'Добавить прямую', width = 17, command = add_line, font = "Arial 13", bg = button_color)
b2.place(anchor = CENTER, relx = 0.7, rely = 0.86)
b3 = Button(wd, text = 'Отобразить решение', width = 20, command = show, font = "Arial 13", bg = button_color)
b3.place(anchor = CENTER, relx = 0.7, rely = 0.92)
b4 = Button(wd, text = 'Очистить экран', width = 15, command = clear, font = "Arial 13", bg = button_color)
b4.place(anchor = CENTER, relx = 0.25, rely = 0.87)


def info():
    box.showinfo('Условие задачи', 'На плоскости заданы множество точек А и множество прямых В. Найти две такие \
различные точки из А, что проходящая через них прямая параллельна наибольшему количеству прямых из В.')

def author():
    box.showinfo('Автор', 'Жабин Дмитрий, ИУ7-44Б')
    
mainmenu = Menu(wd) 
wd.config(menu = mainmenu)
infomenu = Menu(mainmenu, tearoff = 0)
infomenu.add_command(label = "О программе", command = info)
infomenu.add_command(label = "Об авторе", command = author)
mainmenu.add_cascade(label = "Справка", menu = infomenu)

message = Label(wd, text="Введите координаты точки (от 0 до 500) или поставьте точку вручную", bg = label_color)
message.pack(side=BOTTOM)
 
wd.mainloop()
