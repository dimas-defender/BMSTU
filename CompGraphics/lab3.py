from tkinter import *
import tkinter.messagebox as box
from math import pi, cos, sin
import matplotlib.pyplot as plt
from time import perf_counter

button_color = 'Khaki'
window_color = 'Orange'
label_color = 'Khaki'

wd = Tk()
wd.geometry('1200x850')
wd['bg'] = window_color
wd.title("Построение отрезков")


can = Canvas(wd, width = 800, height = 800, bg = 'white')
can.pack(side = LEFT)


eps = 0.0001
width_line = 1

def rgb2hex(color):
        r, g, b = color
        return f'#{r:02x}{g:02x}{b:02x}'

def lib_alg(x1, y1, x2, y2, color):
    can.create_line(x1, y1, x2, y2, fill = color)


def DDA(x1, y1, x2, y2, color):

    length = max(abs(x1 - x2), abs(y1 - y2))

    if length < eps:
        can.create_rectangle(x1, y1, x1, y1, fill=color, width=width_line, outline=color)
        return

    dx = (x2 - x1) / length
    dy = (y2 - y1) / length

    x = x1
    y = y1

    length += 1
    while length > 0:
        length -= 1
        can.create_rectangle(round(x), round(y), round(x), round(y), fill=color, width=width_line, outline=color)
        x += dx
        y += dy


def brez_float(x1, y1, x2, y2, color):

    if abs(x1 - x2) < eps and abs(y1 - y2) < eps:
        can.create_rectangle(x1, y1, x1, y1, fill=color, width=width_line, outline=color)
        return
        
    dx = x2 - x1
    dy = y2 - y1
        
    sign_x = 1 if dx>0 else -1 if dx<0 else 0
    sign_y = 1 if dy>0 else -1 if dy<0 else 0
        
    dx = abs(dx)
    dy = abs(dy)

    if dy >= dx:
        dx, dy = dy, dx
        fl = 1
    else:
        fl = 0

    m = dy / dx
    f = m - 0.5
    x, y = x1, y1

    for i in range(int(dx) + 1):
        can.create_rectangle(x, y, x, y, fill=color, width=width_line, outline=color)
        if f >= 0:
            if fl == 1:
                x += sign_x
            else:
                y += sign_y
            f -= 1
        if f <= 0:
            if fl == 1:
                y += sign_y
            else:
                x += sign_x
        f += m


def brez_int(x1, y1, x2, y2, color):

    if abs(x1 - x2) < eps and abs(y1 - y2) < eps:
        can.create_rectangle(x1, y1, x1, y1, fill=color, width=width_line, outline=color)
        return
        
    dx = x2 - x1
    dy = y2 - y1
        
    sign_x = 1 if dx>0 else -1 if dx<0 else 0
    sign_y = 1 if dy>0 else -1 if dy<0 else 0
        
    dx = abs(dx)
    dy = abs(dy)

    if dy >= dx:
        dx, dy = dy, dx
        fl = 1
    else:
        fl = 0

    m = 2 * dy
    f = m - dx
    x, y = x1, y1

    for i in range(int(dx)):
        can.create_rectangle(x, y, x, y, fill=color, width=width_line, outline=color)
        if f >= 0:
            if fl == 1:
                x += sign_x
            else:
                y += sign_y
            f -= 2*dx
        if f <= 0:
            if fl == 1:
                y += sign_y
            else:
                x += sign_x
        f += m

def brez_s(x1, y1, x2, y2, color, I = 4, flag = 1):
    d0 = (255-color[0])/I
    d1 = (255-color[1])/I
    d2 = (255-color[2])/I
    if (abs(x1-x2) < eps) and (abs(y1-y2) < eps):
        if flag:
            can.create_rectangle(x1, y1, x1, y1, fill=color, width=width_line, outline=color)
    else:
        dx = x2-x1
        dy = y2-y1
        sx = 1 if dx>0 else -1 if dx<0 else 0
        sy = 1 if dy>0 else -1 if dy<0 else 0
        dx = abs(dx)
        dy = abs(dy)
        if dy > dx:
            dx, dy = dy, dx
            fl = 1
        else:
            fl = 0
        m = dy/dx
        f = I/2 #доля площади пикселя
        x, y = x1, y1
        W = (1 - m) * I #корректировка доли площади
        m *= I
        if flag:
            can.create_rectangle(x, y, x, y, fill=rgb2hex((color[0]+int(d0*f), color[1]+int(d1*f),color[2]+int(d2*f))),\
                                width=width_line, outline=rgb2hex((color[0]+int(d0*f), color[1]+int(d1*f),color[2]+int(d2*f))))
        for i in range(1, int(dx)):
            if f < W: #мера перехода по ординате
                if fl == 0:
                    x += sx
                if fl == 1:
                    y += sy
                f += m
            else:
                x += sx
                y += sy
                f -= W #f = f + m - 1
            if flag:
                can.create_rectangle(x, y, x, y, fill=rgb2hex((color[0]+int(d0*f), color[1]+int(d1*f),color[2]+int(d2*f))),\
                                    width=width_line, outline=rgb2hex((color[0]+int(d0*f), color[1]+int(d1*f),color[2]+int(d2*f))))



def wu(x1, y1, x2, y2, color):

    x1 = round(x1)
    x2 = round(x2)
    y1 = round(y1)
    y2 = round(y2)
    
    def _fpart(x):
        return x - int(x)
    
    def _rfpart(x):
        return 1 - _fpart(x)
    
    def pixel_c(p, cc, intens):
 
        x, y = p
        col = [round(c * (1 - intens) + 255 * intens) for c in cc]
        can.create_rectangle(x, y, x, y, fill=rgb2hex(col), width=width_line, outline=rgb2hex(col))

    if abs(x1 - x2) < eps and abs(y1 - y2) < eps:
        img.put(rgb2hex(color),(x1, y1))
        return

    if abs(y1 - y2) < eps:
        step = 1 if x2 > x1 else -1
        x = x1
        while abs(x - x2) > eps:
            can.create_rectangle(x, y1, x, y1, fill=rgb2hex(color), width=width_line, outline=rgb2hex(color))
            x += step
        return
    
    dx, dy = x2 - x1, y2 - y1
    steep = abs(dx) < abs(dy)
 
    if steep:
        x1, y1, x2, y2, dx, dy = y1, x1, y2, x2, dy, dx
    if x2 < x1:
        x1, x2, y1, y2 = x2, x1, y2, y1
 
    grad = dy/dx
    if dx == 0.0:
        grad = 1
        
 
    xp1, xp2 = x1, x2
    intY = y1
    x = xp1
    if steep:
        while x != xp2:
            pixel_c([int(intY), x], color, _rfpart(intY))
            pixel_c([int(intY-1), x], color, _fpart(intY))
            intY += grad
            x += 1
    else:
        while x != xp2:
            pixel_c([x, int(intY)], color, _rfpart(intY))
            pixel_c([x, int(intY-1)], color, _rfpart(intY))
            intY += grad
            x += 1

'''
def brez_stup(x1, y1, x2, y2, color, I):

    if x1 == x2 and y1 == y2:
        img.put(color,(round(x), round(y)))
        return
        
    dx = x2 - x1
    dy = y2 - y1
        
    sign_x = 1 if dx>0 else -1 if dx<0 else 0
    sign_y = 1 if dy>0 else -1 if dy<0 else 0
        
    dx = abs(dx)
    dy = abs(dy)

    m = dy / dx

    if m > 1:
        dx, dy = dy, dx
        m = 1 / m
        fl = 1
    else:
        fl = 0

    f = I / 2
    x, y = x1, y1
    m *= I
    W = I - m
    
    r, g, b = color
    print(r)
    dr = (255 - r) / I
    dg = (255 - g) / I
    db = (255 - b) / I

    R = round(r+dr*f)
    G = round(g+dg*f)
    B = round(b+db*f)
    
    img.put("#%02x%02x%02x" % (R,G,B), (x, y))

    for i in range(dx):
        if f < W:
            if fl == 0:
                x += sign_x
            if fl == 1:
                y += sign_y
            f += m
        if f > W:
            x += sign_x
            y += sign_y
            f -= W
        R = round(r+dr*f)
        G = round(g+dg*f)
        B = round(b+db*f)
        img.put("#%02x%02x%02x" % (R,G,B), (x, y))
'''

l1 = Label(wd, text = "Алгоритм", bg = label_color, font = 'Arial 13', width = '16')
l1.place(anchor = CENTER, relx = 0.75, rely = 0.04)
alg_list = Listbox(wd, width = '38', height = '6', exportselection = 0)
alg_list.insert(END, 'Библиотечная функция')
alg_list.insert(END, 'ЦДА')
alg_list.insert(END, 'Брезенхем с действ.данными')
alg_list.insert(END, 'Брезенхем с целочисл.данными')
alg_list.insert(END, 'Брезенхем с устранением ступенчатости')
alg_list.insert(END, 'Ву')
alg_list.place(anchor = CENTER, relx = 0.78, rely = 0.13)

l2 = Label(wd, text = "Цвет", bg = label_color, font = 'Arial 13', width = '7')
l2.place(anchor = CENTER, relx = 0.93, rely = 0.04)
color_list = Listbox(wd, width = '15', height = '6', exportselection = 0)
color_list.insert(END, 'Красный')
color_list.insert(END, 'Синий')
color_list.insert(END, 'Зеленый')
color_list.insert(END, 'Черный')
color_list.insert(END, 'Белый')
color_list.place(anchor = CENTER, relx = 0.93, rely = 0.13)


l3 = Label(wd, text = "Координаты начала", bg = label_color, font = 'Arial 13')
l3.place(anchor = CENTER, relx = 0.75, rely = 0.24)
e1 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e1.place(anchor = CENTER, relx = 0.87, rely = 0.24)
l3 = Label(wd, text = "Координаты конца", bg = label_color, font = 'Arial 13', width = '17')
l3.place(anchor = CENTER, relx = 0.75, rely = 0.29)
e2 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e2.place(anchor = CENTER, relx = 0.87, rely = 0.29)


def convert_color(color):
    if color == 'red':
        return (255, 0, 0)
    if color == 'blue':
        return (0, 0, 255)
    if color == 'green':
        return (0, 255, 0)
    if color == 'black':
        return (0, 0, 0)
    if color == 'white':
        return (255, 255, 255)
    
def create_line():
    try:
        a = str(e1.get()).split()
        x1 = int(a[0])
        y1 = int(a[1])
        b = str(e2.get()).split()
        x2 = int(b[0])
        y2 = int(b[1])
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести 2 пары целых чисел')
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
        lib_alg(x1, y1, x2, y2, color)
    if i == 1:
        DDA(x1, y1, x2, y2, color)
    if i == 2:
        brez_float(x1, y1, x2, y2, color)
    if i == 3:
        brez_int(x1, y1, x2, y2, color)
    if i == 4:
        brez_s(x1, y1, x2, y2, convert_color(color))
    if i == 5:
        wu(x1, y1, x2, y2, convert_color(color))


def create_spec():

    try:
        a = str(e3.get())
        R = int(a)
        b = str(e4.get())
        step = int(b)
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести 2 целых числа')
        #e3.delete(0, 'end')
        #e4.delete(0, 'end')
        return

    try:
        i = alg_list.curselection()[0]
        j = color_list.curselection()[0]
    except:
        box.showerror('Ошибка', 'Необходимо выбрать алгоритм и цвет')
        return
    
    x1 = y1 = 400
    
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

    step = step * pi / 180
    ang = 0
    
    if i == 0:
        while ang < 2 * pi:
            x2 = x1 + R * cos(ang)
            y2 = y1 + R * sin(ang)
            lib_alg(x1, y1, x2, y2, color)
            ang += step
            
    elif i == 1:
        while ang < 2 * pi:
            x2 = x1 + R * cos(ang)
            y2 = y1 + R * sin(ang)
            DDA(x1, y1, x2, y2, color)
            ang += step
        
    elif i == 2:
        while ang < 2 * pi:
            x2 = x1 + R * cos(ang)
            y2 = y1 + R * sin(ang)
            brez_float(x1, y1, x2, y2, color)
            ang += step
        
    elif i == 3:
        while ang < 2 * pi:
            x2 = x1 + R * cos(ang)
            y2 = y1 + R * sin(ang)
            brez_int(x1, y1, x2, y2, color)
            ang += step
    
    elif i == 4:
        while ang < 2 * pi:
            x2 = x1 + R * cos(ang)
            y2 = y1 + R * sin(ang)
            brez_s(x1, y1, x2, y2, convert_color(color))
            ang += step
        
    elif i == 5:
        while ang < 2 * pi:
            x2 = x1 + R * cos(ang)
            y2 = y1 + R * sin(ang)
            wu(x1, y1, x2, y2, convert_color(color))
            ang += step
        

def clear():
    can.delete("all")


def get_steps(R, a):
    return min(round(R*cos(a)), round(R*sin(a)))

def graph():
    R = 500
    A = [pi/2 / 1000 * i for i in range(1001)]
    N = [get_steps(R,a) for a in A]
    figure = plt.figure()                          
    figure.canvas.set_window_title('График зависимости количества ступенек от угла наклона отрезка')            
    plt.plot(A, N, c='b')                      
    plt.grid(True, axis = 'both')
    plt.xlabel('Угол (длина отрезка = 500)')
    plt.ylabel('Кол-во ступенек') 
    plt.show()

def hist():
    x1 = 200
    y1 = 600
    x2 = 700
    y2 = 300
    color = 'red'
    T = []
    
    t1_start = perf_counter()
    lib_alg(x1, y1, x2, y2, color)
    t1_stop = perf_counter()
    time = t1_stop - t1_start
    T.append(time)
    
    t1_start = perf_counter()
    DDA(x1, y1, x2, y2, color)
    t1_stop = perf_counter()
    time = t1_stop - t1_start
    T.append(time)
    
    t1_start = perf_counter()
    brez_float(x1, y1, x2, y2, color)
    t1_stop = perf_counter()
    time = t1_stop - t1_start
    T.append(time)

    t1_start = perf_counter()
    brez_int(x1, y1, x2, y2, color)
    t1_stop = perf_counter()
    time = t1_stop - t1_start
    T.append(time)
    
    t1_start = perf_counter()
    brez_s(x1, y1, x2, y2, convert_color(color))
    t1_stop = perf_counter()
    time = t1_stop - t1_start
    T.append(time)

    t1_start = perf_counter()
    wu(x1, y1, x2, y2, convert_color(color))
    t1_stop = perf_counter()
    time = t1_stop - t1_start
    T.append(time)

    clear()

    X = ['lib','dda','bres_float','bres_int','bres_smooth','wu']
    fig, ax = plt.subplots()
    fig.canvas.set_window_title('Временные характеристики алгоритмов')
    ax.bar(X, T)
    plt.show()
      

b1 = Button(wd, text = 'Построить отрезок', width = 18, command = create_line, font = "Arial 13", bg = button_color)
b1.place(anchor = CENTER, relx = 0.83, rely = 0.35)
b2 = Button(wd, text = 'Построить спектр', width = 18, command = create_spec, font = "Arial 13", bg = button_color)
b2.place(anchor = CENTER, relx = 0.83, rely = 0.55)
b3 = Button(wd, text = 'Очистить экран', width = 18, command = clear, font = "Arial 13", bg = button_color, height = 2)
b3.place(anchor = CENTER, relx = 0.83, rely = 0.65)
b4 = Button(wd, text = 'Исследование ступенчатости отрезков', command = graph, font = "Arial 13", bg = button_color, height = 2)
b4.place(anchor = CENTER, relx = 0.83, rely = 0.8)
b5 = Button(wd, text = 'Временные характеристики алгоритмов', command = hist, font = "Arial 13", bg = button_color, height = 2)
b5.place(anchor = CENTER, relx = 0.83, rely = 0.9)

e3 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e3.place(anchor = CENTER, relx = 0.87, rely = 0.44)
l3 = Label(wd, text = "Длина луча", bg = label_color, font = 'Arial 13', width = '17')
l3.place(anchor = CENTER, relx = 0.75, rely = 0.44)
e4 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e4.place(anchor = CENTER, relx = 0.87, rely = 0.49)
l3 = Label(wd, text = "Шаг угла", bg = label_color, font = 'Arial 13', width = '17')
l3.place(anchor = CENTER, relx = 0.75, rely = 0.49)
