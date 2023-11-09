#ИУ7-44Б Жабин Дмитрий

from tkinter import *
import tkinter.messagebox as box
from math import sin, cos, pi

button_color = 'Khaki'
window_color = 'Orange'
label_color = 'Khaki'

wd = Tk()
wd.geometry('1100x850')
wd['bg'] = window_color
wd.title("Графический экран")

fix = 400

can = Canvas(width = 800, height = 800, bg = 'white')
can.pack(side = LEFT)

lines = []

def create_axes():
    can.create_line(0, 400, 800, 400, width = 2, arrow = LAST)
    can.create_line(400, 800, 400, 0, width = 2, arrow = LAST)
    can.create_text(390, 410, text = '0')
    can.create_text(795, 390, text = 'x')
    can.create_text(390, 5, text = 'y')

def clear():
    can.delete('all')
    create_axes()

def reset():
    clear()
    global lines
    lines.clear()
    lines = [[[0,0],[0,85]],[[0,0],[240,0]],[[0,85],[240,85]],[[240,85],[240,0]],[[0,100],[0,225]],
         [[0,100],[240,100]],[[240,100],[240,225]],[[52,142],[188,142]],
         [[185,115],[225,115]],[[225,115],[225,155]],[[185,115],[225,155]],
         [[85,85],[85,55]],[[85,55],[155,55]],[[155,55],[155,85]],
         [[185,-7],[225,-7]],[[185,-7],[185,0]],[[225,-7],[225,0]],
         [[120,290],[120,298]],[[120,160],[120,152]],[[55,225],[47,225]],[[185,225],[193,225]],
         [[154,280],[160,286]],[[154,170],[160,164]],[[86,280],[80,286]],[[86,170],[80,164]],
         [[64,256],[57,260]],[[176,256],[183,260]],[[64,194],[57,190]],[[176,194],[183,190]],
         [[120,220],[155,220]],[[120,230],[155,230]],[[155,220],[160,225]],[[155,230],[160,225]],
         [[120,220],[115,225]],[[120,230],[115,225]],[[120,220],[125,225]],[[125,225],[125,275]],
         [[115,225],[115,275]],[[115,275],[120,280]],[[125,275],[120,280]],[[118,223],[165,270]],
         [[20,8],[220,8]],[[85,344],[85,357]],[[155,344],[155,357]]]
    
    t = 1.24
    while (t < 1.9):
        x1 = 107.84 * cos(t) + 120
        y1 = 107.84 * sin(t) + 255
        t += 0.01
        x2 = 107.84 * cos(t) + 120
        y2 = 107.84 * sin(t) + 255
        lines.append([[x1,y1],[x2,y2]])

    t = 0
    while (t < pi):
        x1 = 120 * cos(t) + 120
        y1 = 125 * sin(t) + 225
        t += 0.01
        x2 = 120 * cos(t) + 120
        y2 = 125 * sin(t) + 225
        lines.append([[x1,y1],[x2,y2]])

    t = -0.87
    while (t < 4.01):
        x1 = 105 * cos(t) + 120
        y1 = 110 * sin(t) + 225
        t += 0.01
        x2 = 105 * cos(t) + 120
        y2 = 110 * sin(t) + 225
        lines.append([[x1,y1],[x2,y2]])

    for i in range(1, 24):
        lines.append([[i * 10, 0], [i * 10, 85]])

    for j in range(len(lines)):
        lines[j][0][0] += 10
        lines[j][0][1] += 10
        lines[j][1][0] += 10
        lines[j][1][1] += 10
    
def author():
    box.showinfo('Автор', 'Жабин Дмитрий, ИУ7-44Б')
        
def draw():
    for j in range(len(lines)):
        can.create_line(fix + lines[j][0][0], fix - lines[j][0][1], fix + lines[j][1][0], fix - lines[j][1][1])
    
def move():
    try:
        a = str(e1.get()).split()
        dx = int(a[0])
        dy = int(a[1])
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести 2 целых числа через пробел')
        e1.delete(0, 'end')
        return
    clear()
    for i in range(len(lines)):
        lines[i][0][0] += dx
        lines[i][0][1] += dy
        lines[i][1][0] += dx
        lines[i][1][1] += dy
    draw()
    #e1.delete(0, 'end')

def scale():
    try:
        a = str(e2.get()).split()
        xm = int(a[0])
        ym = int(a[1])
        b = str(e3.get()).split()
        kx = float(b[0])
        ky = float(b[1])
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести пару целых и пару вещественных чисел через пробел')
        e2.delete(0, 'end')
        e3.delete(0, 'end')
        return
    clear()
    for i in range(len(lines)):
        lines[i][0][0] = kx * lines[i][0][0] - (kx - 1) * xm
        lines[i][0][1] = ky * lines[i][0][1] - (ky - 1) * ym
        lines[i][1][0] = kx * lines[i][1][0] - (kx - 1) * xm
        lines[i][1][1] = ky * lines[i][1][1] - (ky - 1) * ym
    draw()
    #e2.delete(0, 'end')
    #e3.delete(0, 'end')
    
def rotate():
    try:
        a = str(e4.get()).split()
        x0 = int(a[0])
        y0 = int(a[1])
        b = str(e5.get())
        ang = float(b) * pi / 180
    except:
        box.showerror('Ошибка ввода', 'Необходимо ввести пару целых чисел через пробел и вещественное число')
        e4.delete(0, 'end')
        e5.delete(0, 'end')
        return
    clear()
    for i in range(len(lines)):
        x1 = lines[i][0][0]
        y1 = lines[i][0][1]
        x2 = lines[i][1][0]
        y2 = lines[i][1][1]
        lines[i][0][0] = x0 + (x1 - x0) * cos(ang) - (y1 - y0) * sin(ang)
        lines[i][0][1] = y0 + (x1 - x0) * sin(ang) + (y1 - y0) * cos(ang)
        lines[i][1][0] = x0 + (x2 - x0) * cos(ang) - (y2 - y0) * sin(ang)
        lines[i][1][1] = y0 + (x2 - x0) * sin(ang) + (y2 - y0) * cos(ang)
    draw()
    #e4.delete(0, 'end')
    #e5.delete(0, 'end')

reset()

mainmenu = Menu(wd) 
wd.config(menu = mainmenu)
infomenu = Menu(mainmenu, tearoff = 0)
infomenu.add_command(label = "Об авторе", command = author)
mainmenu.add_cascade(label = "Справка", menu = infomenu)

e1 = Entry(wd, justify = 'center', font = "Arial 13", width = '20')
e1.place(anchor = CENTER, relx = 0.85, rely = 0.29)
e2 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e2.place(anchor = CENTER, relx = 0.79, rely = 0.45)
e3 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e3.place(anchor = CENTER, relx = 0.92, rely = 0.45)
e4 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e4.place(anchor = CENTER, relx = 0.81, rely = 0.65)
e5 = Entry(wd, justify = 'center', font = "Arial 13", width = '10')
e5.place(anchor = CENTER, relx = 0.91, rely = 0.65)

b1 = Button(wd, text = 'Нарисовать', width = 12, height = 3, command = draw, font = "Arial 13", bg = button_color)
b1.place(anchor = CENTER, relx = 0.85, rely = 0.1)
b2 = Button(wd, text = 'Переместить', width = 15, command = move, font = "Arial 13", bg = button_color)
b2.place(anchor = CENTER, relx = 0.85, rely = 0.37)
b3 = Button(wd, text = 'Промасштабировать', width = 20, command = scale, font = "Arial 13", bg = button_color)
b3.place(anchor = CENTER, relx = 0.85, rely = 0.55)
b4 = Button(wd, text = 'Повернуть', width = 15, command = rotate, font = "Arial 13", bg = button_color)
b4.place(anchor = CENTER, relx = 0.85, rely = 0.75)
b5 = Button(wd, text = 'Очистить', width = 12, height = 3, command = reset, font = "Arial 13", bg = button_color)
b5.place(anchor = CENTER, relx = 0.85, rely = 0.2)

mes1 = Label(wd, text = "Координаты вектора переноса", bg = label_color)
mes1.place(anchor = CENTER, relx = 0.85, rely = 0.32)
mes2 = Label(wd, text = "Координаты центра\n масштабирования", bg = label_color)
mes2.place(anchor = CENTER, relx = 0.79, rely = 0.5)
mes3 = Label(wd, text = "Коэффициенты\n масштабирования по X и Y", bg = label_color)
mes3.place(anchor = CENTER, relx = 0.92, rely = 0.5)
mes4 = Label(wd, text = "Координаты центра\n поворота", bg = label_color)
mes4.place(anchor = CENTER, relx = 0.81, rely = 0.7)
mes5 = Label(wd, text = "Угол поворота\n (в градусах)", bg = label_color)
mes5.place(anchor = CENTER, relx = 0.91, rely = 0.7)

mes6 = Label(wd, text = "Все пары чисел вводятся\n через пробел!\nКоординаты - целые числа\n в интервале [-400; 400]\n по обеим осям", bg = label_color, font = "Arial 14")
mes6.place(anchor = CENTER, relx = 0.87, rely = 0.9)

wd.mainloop()
