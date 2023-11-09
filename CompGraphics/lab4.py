from tkinter import *
import tkinter.messagebox as box
from math import pi, cos, sin, sqrt
import matplotlib.pyplot as plt
from time import perf_counter

button_color = 'Khaki'
window_color = 'Orange'
label_color = 'Khaki'

wd = Tk()
wd.geometry('1200x850')
wd['bg'] = window_color
wd.title("Построение окружностей/эллипсов")

can = Canvas(wd, width = 800, height = 800, bg = 'white')
can.pack(side = LEFT)

eps = 0.0001
width_line = 1

def round_num(x):
    return int(x + 0.5)

def draw_4_points(x0, y0, x, y, color):
    can.create_rectangle(x0 + x, y0 - y, x0 + x, y0 - y, fill = color, width = width_line, outline = color)
    can.create_rectangle(x0 + x, y0 + y, x0 + x, y0 + y, fill = color, width = width_line, outline = color)
    can.create_rectangle(x0 - x, y0 - y, x0 - x, y0 - y, fill = color, width = width_line, outline = color)
    can.create_rectangle(x0 - x, y0 + y, x0 - x, y0 + y, fill = color, width = width_line, outline = color)


def circle_lib(x0, y0, R, color):
    can.create_oval(x0 - R, y0 - R, x0 + R, y0 + R, outline = color, width = 2)

def circle_canon(x0, y0, R, color, flag = 1):
    
    x = 0
    y = R
    R2 = R * R

    while x <= y:
        y = round_num(sqrt(R2 - x * x))
        if flag:
            draw_4_points(x0, y0, x, y, color)
            draw_4_points(x0, y0, y, x, color)
        
        x += 1
        

def circle_param(x0, y0, R, color, flag = 1):
    
    if R == 0:
        if flag:
            can.create_rectangle(x0, y0, x0, y0, fill = color, width = width_line, outline = color)
        return

    angle = pi / 4
    step = 1 / R

    while angle >= 0:
        x = round_num(R * cos(angle))
        y = round_num(R * sin(angle))
        if flag:
            draw_4_points(x0, y0, x, y, color)
            draw_4_points(x0, y0, y, x, color)
        
        angle -= step
        
        

def circle_brez(x0, y0, R, color, flag = 1):
    y = R
    x = 0
    d = 2 - 2 * R

    while x <= y:
        if flag:
            draw_4_points(x0, y0, x, y, color)
            draw_4_points(x0, y0, y, x, color)

        if d < 0:
            d1 = 2 * d + 2 * y - 1

            if d1 < 0:
                x += 1
                d += 2 * x + 1
            else:
                x += 1
                y -= 1
                d += 2 * x - 2 * y + 2

        elif d == 0:
            x += 1
            y -= 1
            d += 2 * x - 2 * y + 2
            
        else:
            d1 = 2 * d - 2 * x - 1

            if d1 > 0:
                y -= 1
                d += -2 * y + 1
            else:
                x += 1
                y -= 1
                d += 2 * x - 2 * y + 2
                

def circle_mid_point(x0, y0, R, color, flag = 1):
    x = 0
    y = R

    f = 5 / 4 - R
    df = 0
    delta = -2 * y

    while x <= y:
        if flag:
            draw_4_points(x0, y0, x, y, color)
            draw_4_points(x0, y0, y, x, color)

        x += 1
        if f >= 0:
            y -= 1
            delta += 2
            f += delta
        df += 2
        f += df + 1
        

def ellipse_lib(x0, y0, A, B, color):
    can.create_oval(x0 - A, y0 - B, x0 + A, y0 + B, outline = color, width = 2)

def ellipse_canon(x0, y0, A, B, color, flag = 1):
    x = 0
    B2 = B * B
    A2 = A * A

    if B2 == 0 and A2 == 0:
        if flag:
            can.create_rectangle(x0, y0, x0, y0, fill = color, width = width_line, outline = color)
        return

    end_x = A2 / sqrt(A2 + B2)

    if A2 != 0:
        while x <= end_x:
            y = round_num(sqrt((1 - (x**2 / A2)) * B2))
            if flag:
            	draw_4_points(x0, y0, x, y, color)
            x += 1

    y = 0
    end_y = B2 / sqrt(A2 + B2)

    if B2 != 0:
        while y <= end_y:
            x = round_num(sqrt((1 - (y**2 / B2)) * A2))
            if flag:
            	draw_4_points(x0, y0, x, y, color)
            y += 1
    if flag:
        draw_4_points(x0, y0, end_x, end_y, color)
    

def ellipse_param(x0, y0, A, B, color, flag = 1):
    if A == 0 and B == 0:
        if flag:
            can.create_rectangle(x0, y0, x0, y0, fill = color, width = width_line, outline = color)
        return

    step = min(1 / A, 1 / B)
    angle = pi / 2

    while angle >= 0:
        x = round_num(A * cos(angle))
        y = round_num(B * sin(angle))
        if flag:
            draw_4_points(x0, y0, x, y, color)
        angle -= step
        

def ellipse_brez(x0, y0, A, B, color, flag = 1):
    x = 0
    y = B
    end_y = 0
    A2 = A*A
    B2 = B*B

    d = B2 - 2 * B * A2 + A2

    b2 = 2 * B2
    a2 = 2 * A2
    a2_b2 = A2 + B2

    while y >= end_y:
        if flag:
            draw_4_points(x0, y0, x, y, color)

        if d < 0:
            d1 = 2 * d + a2 * y - A2

            if d1 < 0:
                x += 1
                d += b2 * x + B2
            else:
                x += 1
                y -= 1
                d += b2 * x - a2 * y + a2_b2

        elif d == 0:
            x += 1
            y -= 1
            d += b2 * x - a2 * y + a2_b2

        else:
            d1 = 2 * d - b2 * x - B2

            if d1 > 0:
                y -= 1
                d += -a2 * y + A2
            else:
                x += 1
                y -= 1
                d += b2 * x - a2 * y + a2_b2
                

def ellipse_mid_point(x0, y0, A, B, color, flag = 1):
    x = 0
    y = B

    B2 = B*B
    A2 = A*A
    b2 = 2 * B2
    a2 = 2 * A2
    end_x = A2 / sqrt(A2 + B2)


    f = B2 - A2 * B + A2 / 4
    df = 0
    delta = -a2 * y
    
    while x <= end_x:
        if flag:
            draw_4_points(x0, y0, x, y, color)

        x += 1
        if f >= 0:
            y -= 1
            delta += a2
            f += delta
        df += b2
        f += df + B2

    delta = b2 * x
    f += 3 / 4 * (A2 - B2) - A2 * y - B2 * x
    df = -a2 * y

    while y >= 0:
        if flag:
            draw_4_points(x0, y0, x, y, color)
        y -= 1

        if f <= 0:
            x += 1
            delta += b2
            f += delta
        df += a2
        f += df + A2


l1 = Label(wd, text = "Алгоритм", bg = label_color, font = 'Arial 13', width = '16')
l1.place(anchor = CENTER, relx = 0.78, rely = 0.04)
alg_list = Listbox(wd, width = '30', height = '5', exportselection = 0)
alg_list.insert(END, 'Каноническое уравнение')
alg_list.insert(END, 'Параметрическое уравнение')
alg_list.insert(END, 'Брезенхема')
alg_list.insert(END, 'Средней точки')
alg_list.insert(END, 'Библиотечная функция')
alg_list.place(anchor = CENTER, relx = 0.78, rely = 0.13)

l2 = Label(wd, text = "Цвет", bg = label_color, font = 'Arial 13', width = '7')
l2.place(anchor = CENTER, relx = 0.93, rely = 0.04)
color_list = Listbox(wd, width = '15', height = '5', exportselection = 0)
color_list.insert(END, 'Красный')
color_list.insert(END, 'Синий')
color_list.insert(END, 'Зеленый')
color_list.insert(END, 'Черный')
color_list.insert(END, 'Белый')
color_list.place(anchor = CENTER, relx = 0.93, rely = 0.13)

l3 = Label(wd, text = "Координаты центра", bg = label_color, font = 'Arial 13')
l3.place(anchor = CENTER, relx = 0.78, rely = 0.24)
e1 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e1.place(anchor = CENTER, relx = 0.9, rely = 0.24)
l4 = Label(wd, text = "Радиус", bg = label_color, font = 'Arial 13', width = '17')
l4.place(anchor = CENTER, relx = 0.78, rely = 0.29)
l5 = Label(wd, text = "Полуоси", bg = label_color, font = 'Arial 13', width = '17')
l5.place(anchor = CENTER, relx = 0.78, rely = 0.34)
e2 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e2.place(anchor = CENTER, relx = 0.9, rely = 0.29)
e3 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e3.place(anchor = CENTER, relx = 0.9, rely = 0.34)
    
def create_circle():
    try:
        a = str(e1.get()).split()
        x0 = int(a[0])
        y0 = int(a[1])
        b = str(e2.get())
        R = int(b)
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести пару целых чисел и натуральное число')
        #e1.delete(0, 'end')
        #e2.delete(0, 'end')
        return

    try:
        i = alg_list.curselection()[0]
        j = color_list.curselection()[0]
    except:
        box.showerror('Ошибка', 'Необходимо выбрать алгоритм и цвет')
        return

    if j == 0:
        color = 'red'
    if j == 1:
        color = 'blue'
    if j == 2:
        color = 'green'
    if j == 3:
        color = 'black'
    if j == 4:
        color = 'white'
        
    if i == 0:
        circle_canon(x0, y0, R, color)
    if i == 1:
        circle_param(x0, y0, R, color)
    if i == 2:
        circle_brez(x0, y0, R, color)
    if i == 3:
        circle_mid_point(x0, y0, R, color)
    if i == 4:
        circle_lib(x0, y0, R, color)


def create_ellipse():
    try:
        a = str(e1.get()).split()
        x0 = int(a[0])
        y0 = int(a[1])
        b = str(e3.get()).split()
        a = int(b[0])
        b = int(b[1])
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести пару целых и пару натуральных чисел')
        #e1.delete(0, 'end')
        #e3.delete(0, 'end')
        return

    try:
        i = alg_list.curselection()[0]
        j = color_list.curselection()[0]
    except:
        box.showerror('Ошибка', 'Необходимо выбрать алгоритм и цвет')
        return

    if j == 0:
        color = 'red'
    if j == 1:
        color = 'blue'
    if j == 2:
        color = 'green'
    if j == 3:
        color = 'black'
    if j == 4:
        color = 'white'
        
    if i == 0:
        ellipse_canon(x0, y0, a, b, color)
    if i == 1:
        ellipse_param(x0, y0, a, b, color)
    if i == 2:
        ellipse_brez(x0, y0, a, b, color)
    if i == 3:
        ellipse_mid_point(x0, y0, a, b, color)
    if i == 4:
        ellipse_lib(x0, y0, a, b, color)


def create_circle_spec():

    try:
        a = str(e4.get())
        R = int(a)
        b = str(e6.get())
        step = int(b)
        c = str(e7.get())
        N = int(c)
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести 3 целых числа')
        #e4.delete(0, 'end')
        #e6.delete(0, 'end')
        #e7.delete(0, 'end')
        return

    try:
        i = alg_list.curselection()[0]
        j = color_list.curselection()[0]
    except:
        box.showerror('Ошибка', 'Необходимо выбрать алгоритм и цвет')
        return
    
    x0 = y0 = 400
    
    if j == 0:
        color = 'red'
    if j == 1:
        color = 'blue'
    if j == 2:
        color = 'green'
    if j == 3:
        color = 'black'
    if j == 4:
        color = 'white'
    
    if i == 0:
        for i in range(N):
            circle_canon(x0, y0, R, color)
            R += step
            
    elif i == 1:
        for i in range(N):
            circle_param(x0, y0, R, color)
            R += step
        
    elif i == 2:
        for i in range(N):
            circle_brez(x0, y0, R, color)
            R += step
        
    elif i == 3:
        for i in range(N):
            circle_mid_point(x0, y0, R, color)
            R += step
    
    elif i == 4:
        for i in range(N):
            circle_lib(x0, y0, R, color)
            R += step

def create_ellipse_spec():

    try:
        d = str(e5.get()).split()
        a = int(d[0])
        b = int(d[1])
        f = str(e6.get())
        step = int(f)
        c = str(e7.get())
        N = int(c)
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести пару целых чисел и 2 целых числа')
        #e5.delete(0, 'end')
        #e6.delete(0, 'end')
        #e7.delete(0, 'end')
        return

    try:
        i = alg_list.curselection()[0]
        j = color_list.curselection()[0]
    except:
        box.showerror('Ошибка', 'Необходимо выбрать алгоритм и цвет')
        return
    
    x0 = y0 = 400
    
    if j == 0:
        color = 'red'
    if j == 1:
        color = 'blue'
    if j == 2:
        color = 'green'
    if j == 3:
        color = 'black'
    if j == 4:
        color = 'white'
    
    if i == 0:
        for i in range(N):
            ellipse_canon(x0, y0, a, b, color)
            a += step
            b += step
            
    elif i == 1:
        for i in range(N):
            ellipse_param(x0, y0, a, b, color)
            a += step
            b += step
        
    elif i == 2:
        for i in range(N):
            ellipse_brez(x0, y0, a, b, color)
            a += step
            b += step
        
    elif i == 3:
        for i in range(N):
            ellipse_mid_point(x0, y0, a, b, color)
            a += step
            b += step
    
    elif i == 4:
        for i in range(N):
            ellipse_lib(x0, y0, a, b, color)
            a += step
            b += step
        

def clear():
    can.delete("all")


def graph_circle():
    T_1 = []
    T_2 = []
    T_3 = []
    T_4 = []
    T_5 = []

    N = 400
    k = 200
    
    R = [x for x in range(N + 1)]

    x0 = y0 = 400
    color = 'red'
    
    for i in range(N + 1):
        t = 0
        for j in range(k):
            t1_start = perf_counter()
            circle_canon(x0, y0, R[i], color, 0)
            t1_stop = perf_counter()
            time = t1_stop - t1_start
            t += time
        T_1.append(t / k)

    for i in range(N + 1):
        t = 0
        for j in range(k):
            t1_start = perf_counter()
            circle_param(x0, y0, R[i], color, 0)
            t1_stop = perf_counter()
            time = t1_stop - t1_start
            t += time
        T_2.append(t / k)

    for i in range(N + 1):
        t = 0
        for j in range(k):
            t1_start = perf_counter()
            circle_brez(x0, y0, R[i], color, 0)
            t1_stop = perf_counter()
            time = t1_stop - t1_start
            t += time
        T_3.append(t / k)

    for i in range(N + 1):
        t = 0
        for j in range(k):
            t1_start = perf_counter()
            circle_mid_point(x0, y0, R[i], color, 0)
            t1_stop = perf_counter()
            time = t1_stop - t1_start
            t += time
        T_4.append(t / k)

    for i in range(N + 1):
        t = 0
        for j in range(k):
            t1_start = perf_counter()
            circle_lib(x0, y0, R[i], color)
            t1_stop = perf_counter()
            time = t1_stop - t1_start
            t += time
        T_5.append(t / k)
        
    clear()

    figure = plt.figure()                          
    figure.canvas.set_window_title('Зависимость времени работы алгоритмов от радиуса окружности')
    plt.plot(R, T_1, label = 'Каноническое')
    plt.plot(R, T_2, label = 'Параметрическое')
    plt.plot(R, T_3, label = 'Брезенхем')
    plt.plot(R, T_4, label = 'Средняя точка')
    plt.plot(R, T_5, label = 'Библиотечная')
    plt.legend(loc = 'upper left')
    plt.grid(True, axis = 'both')
    plt.xlabel('Радиус окружности')
    plt.ylabel('Время работы') 
    plt.show()
        

def graph_ellipse():
    T_1 = []
    T_2 = []
    T_3 = []
    T_4 = []
    T_5 = []

    N = 400
    k = 200
    
    delta = [x for x in range(N + 1)]

    x0 = y0 = 400
    A = 10
    B = 5
    color = 'red'
    
    for i in range(N + 1):
        t = 0
        for j in range(k):
            t1_start = perf_counter()
            ellipse_canon(x0, y0, A + delta[i], B + delta[i], color, 0)
            t1_stop = perf_counter()
            time = t1_stop - t1_start
            t += time
        T_1.append(t / k)

    for i in range(N + 1):
        t = 0
        for j in range(k):
            t1_start = perf_counter()
            ellipse_param(x0, y0, A + delta[i], B + delta[i], color, 0)
            t1_stop = perf_counter()
            time = t1_stop - t1_start
            t += time
        T_2.append(t / k)

    for i in range(N + 1):
        t = 0
        for j in range(k):
            t1_start = perf_counter()
            ellipse_brez(x0, y0, A + delta[i], B + delta[i], color, 0)
            t1_stop = perf_counter()
            time = t1_stop - t1_start
            t += time
        T_3.append(t / k)

    for i in range(N + 1):
        t = 0
        for j in range(k):
            t1_start = perf_counter()
            ellipse_mid_point(x0, y0, A + delta[i], B + delta[i], color, 0)
            t1_stop = perf_counter()
            time = t1_stop - t1_start
            t += time
        T_4.append(t / k)

    for i in range(N + 1):
        t = 0
        for j in range(k):
            t1_start = perf_counter()
            ellipse_lib(x0, y0, A + delta[i], B + delta[i], color)
            t1_stop = perf_counter()
            time = t1_stop - t1_start
            t += time
        T_5.append(t / k)

    clear()

    figure = plt.figure()                          
    figure.canvas.set_window_title('Зависимость времени работы алгоритмов от изменения полуосей эллипса')
    plt.plot(delta, T_1, label = 'Каноническое')
    plt.plot(delta, T_2, label = 'Параметрическое')
    plt.plot(delta, T_3, label = 'Брезенхем')
    plt.plot(delta, T_4, label = 'Средняя точка')
    plt.plot(delta, T_5, label = 'Библиотечная')
    plt.legend(loc = 'upper left')
    plt.grid(True, axis = 'both')
    plt.xlabel('Изменение полуосей эллипса')
    plt.ylabel('Время работы') 
    plt.show()
    

b1 = Button(wd, text = 'Построить окружность', width = 19, command = create_circle, font = "Arial 13", bg = button_color)
b1.place(anchor = CENTER, relx = 0.76, rely = 0.4)
b2 = Button(wd, text = 'Построить эллипс', width = 18, command = create_ellipse, font = "Arial 13", bg = button_color)
b2.place(anchor = CENTER, relx = 0.92, rely = 0.4)
b3 = Button(wd, text = 'Построить спектр окружностей', width = 27, command = create_circle_spec, font = "Arial 13", bg = button_color)
b3.place(anchor = CENTER, relx = 0.83, rely = 0.68)
b4 = Button(wd, text = 'Построить спектр эллипсов', width = 27, command = create_ellipse_spec, font = "Arial 13", bg = button_color)
b4.place(anchor = CENTER, relx = 0.83, rely = 0.73)
b5 = Button(wd, text = 'Очистить экран', width = 18, command = clear, font = "Arial 13", bg = button_color, height = 2)
b5.place(anchor = CENTER, relx = 0.83, rely = 0.82)
b6 = Button(wd, text = 'Временные хар-ки алгоритмов (окружность)', command = graph_circle, font = "Arial 13", bg = button_color)
b6.place(anchor = CENTER, relx = 0.83, rely = 0.9)
b7 = Button(wd, text = 'Временные хар-ки алгоритмов (эллипс)', command = graph_ellipse, font = "Arial 13", bg = button_color, width = '38')
b7.place(anchor = CENTER, relx = 0.83, rely = 0.95)

l6 = Label(wd, text = "Начальный радиус", bg = label_color, font = 'Arial 13', width = '30')
l6.place(anchor = CENTER, relx = 0.79, rely = 0.47)
l7 = Label(wd, text = "Начальные значения полуосей", bg = label_color, font = 'Arial 13', width = '30')
l7.place(anchor = CENTER, relx = 0.79, rely = 0.52)
l8 = Label(wd, text = "Шаг изменения радиуса/полуосей", bg = label_color, font = 'Arial 13', width = '30')
l8.place(anchor = CENTER, relx = 0.79, rely = 0.57)
l9 = Label(wd, text = "Количество окружностей/эллипсов", bg = label_color, font = 'Arial 13', width = '30')
l9.place(anchor = CENTER, relx = 0.79, rely = 0.62)

e4 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e4.place(anchor = CENTER, relx = 0.95, rely = 0.47)
e5 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e5.place(anchor = CENTER, relx = 0.95, rely = 0.52)
e6 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e6.place(anchor = CENTER, relx = 0.95, rely = 0.57)
e7 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e7.place(anchor = CENTER, relx = 0.95, rely = 0.62)

wd.mainloop()
