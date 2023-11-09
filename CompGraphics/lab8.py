#Алгоритм отсечения отрезков произвольным выпуклым отсекателем
#ИУ7-44Б Жабин Дмитрий

from tkinter import *
from tkinter import messagebox as mb
from math import *

WIDTH, HEIGHT = 800, 800
FALSE = "-"

COLOR_BG = ((255, 255, 255), 'white')
COLOR_FILL_INIT = ((0.0, 0.0, 0.0), "#000000")
COLOR_LINE = ((136, 0, 250), "#8800fa")

FONT = "Arial 13"

button_color = 'Khaki'
window_color = 'Orange'
label_color = 'Khaki'


def print_info(string_info):
    mb.showinfo(title = "Информация", message = string_info)

def print_error(string_error):
    mb.showerror(title = "Ошибка", message = string_error)

def create_button(str, func, coords):
    button = Button(text = str, width = 30, command = func,  bg = button_color, font = "Arial 13")
    button.place(x = coords[0], y = coords[1], anchor = "center")

def create_label(root, str, coords, w = 250):
    label = Label(root, text = str, bg = label_color, font = "Arial 13")
    label.place(x = coords[0], y = coords[1], anchor = "center", width = w)

def setup_ui(root, size, title):
    root.title(title)
    root.geometry(size)
    root.configure(bg = window_color)
    root.resizable(width = False, height = False)

def create_entry(root, coords):
    entry = Entry(root, width = "50", font = "Arial 13")
    entry.place(x = coords[0], y = coords[1], anchor = "center", width = 150, height = 25)
    return entry

def set_binds(canvas_class, line_list, contour):

    canvas_class.canvas.bind("<ButtonPress>", lambda event: canvas_class.keyPress(event))
    canvas_class.canvas.bind("<ButtonRelease>", lambda event, arg1=line_list: canvas_class.keyRelease(event, line_list))
    canvas_class.canvas.bind("<Motion>", lambda event: canvas_class.Motion(event))
    canvas_class.canvas.bind("<Shift-ButtonPress>", lambda event: canvas_class.keyPress_rectangle(event))
    canvas_class.canvas.bind("<Shift-ButtonRelease>", lambda event, arg1=contour: canvas_class.keyRelease_rectangle(event, arg1))
    canvas_class.canvas.bind("<Shift-Motion>", lambda event: canvas_class.Motion_rectangle(event))


def clear(canvas_class, points_list, contour):
    canvas_class.clear_all()

    for i in range(len(points_list) - 1, -1, -1):
        del points_list[i]

    for i in range(len(contour) - 1, -1, -1):
        del contour[i]

    contour.append([])


def get_coords(coords):
    try:
        if len(coords.split()) != 2:
            print_error("Введите два целых числа через пробел!")
            return FALSE, FALSE
        a, b = map(int, coords.split())
    except:
        print_error("Координаты должны быть целого типа")
        return FALSE, FALSE
    return (a, b)


class paint_class():
    canvas = None
    flag = False
    start = False
    stop = False

    def __init__(self, root):
        self.canvas = Canvas(root, width=WIDTH, height=HEIGHT, bg="white")
        self.canvas.place(x=0, y=0)
        self.init_canvas()

    def clear_all(self):
        self.canvas.delete(ALL)
        self.init_canvas()

    def init_canvas(self):
        self.canvas.create_line(0, 0, 0, 0, tags="line_temp")
        self.canvas.create_line(0, 0, 0, 0, tags="line_rectangle")

    def draw_line(self, coords, color="blue"):
        self.canvas.create_line(coords[0], coords[1], coords[2], coords[3], fill=color)

    def keyPress(self, event):
        if not self.flag:
            self.start = [event.x, event.y]
        self.flag = True

    def keyRelease(self, event, line_list):
        self.canvas.coords("line_temp", 0, 0, 0, 0)
        if len(line_list) >= 10:
            self.start = False
            self.flag = False
            print_error("Нельзя рисовать более 10 отрезков!")
            return

        self.canvas.create_line(self.start[0], self.start[1], event.x, event.y, fill = "red")
        line_list.append([self.start[0], self.start[1], event.x, event.y])

        self.flag = False
        self.start = False

    def Motion(self, event):
        if self.start:
            self.canvas.coords("line_temp", self.start[0], self.start[1], event.x, event.y)

    def Motion_rectangle(self, event):
        if self.stop:
            self.canvas.coords("line_rectangle", self.stop[0], self.stop[1], event.x, event.y)

    def keyPress_rectangle(self, event):
        self.start = self.stop
        self.stop = [event.x, event.y]

    def keyRelease_rectangle(self, event, lst):
        if self.start:
            self.canvas.create_line(self.start[0], self.start[1], self.stop[0], self.stop[1], fill = "blue")
            lst[-1].append([self.start[0], self.start[1], self.stop[0], self.stop[1]])

    def keySpace_rectangle(self, event, lst):
        if len(lst[-1]) > 1:
            self.canvas.create_line(lst[-1][-1][2], lst[-1][-1][3], lst[-1][0][0], lst[-1][0][1], fill = "blue")
            lst[-1].append([lst[-1][-1][2], lst[-1][-1][3], lst[-1][0][0], lst[-1][0][1]])
            lst.append([])
            self.canvas.coords("line_rectangle", 0, 0, 0, 0)
            self.flag = False
            self.start = False
            self.stop = False


def close_contour(lst, contour, canvas_class):
    if len(lst) >= 2:
        canvas_class.canvas.create_line(lst[-1][0], lst[-1][1], lst[0][0], lst[0][1], fill = "blue")
        for i in range(len(lst) - 1):
            contour[-1].append([lst[i][0], lst[i][1], lst[i+1][0], lst[i+1][1]])
        contour[-1].append([lst[-1][0], lst[-1][1], lst[0][0], lst[0][1]])
        contour.append([])

    for i in range(len(lst) - 1, -1, -1):
        del lst[i]

def add_contour(lst, start, canvas_class):
    start = get_coords(start.get())
    if start[0] == FALSE:
        return
    lst.append([start[0], start[1]])
    if len(lst) >= 2:
        canvas_class.canvas.create_line(lst[-1][0], lst[-1][1], lst[-2][0], lst[-2][1], fill = "blue")


def add_line(lst, start, stop, canvas):
    if len(lst) >= 10:
        print_error("Нельзя рисовать более 10 отрезков!")
        return

    start = get_coords(start.get())
    if start[0] == FALSE:
        return

    stop = get_coords(stop.get())
    if stop[0] == FALSE:
        return

    if [start[0], start[1], stop[0], stop[1]] in lst:
        print_error("Данная линяя уже имеется!")
        return

    canvas.canvas.create_line(start[0], start[1], stop[0], stop[1], fill = "red")
    lst.append([start[0], start[1], stop[0], stop[1]])


def SignVectorProduct(line1, line2): # x1*y2 - x2*y1
    V1 = [line1[2] - line1[0], line1[3] - line1[1]]
    V2 = [line2[2] - line2[0], line2[3] - line2[1]]
    temp = V1[0] * V2[1] - V2[0] * V1[1]
    if fabs(temp) <= 1e-3:
        return
    return 1 if temp > 0 else -1


def IsConvex(polygon):
    if len(polygon):
        sign = SignVectorProduct(polygon[-1], polygon[0])
    for i in range(len(polygon) - 1):
        next_sign = SignVectorProduct(polygon[i], polygon[i + 1])
        if not next_sign:
            continue
        if sign != next_sign:
            return False
    return sign

def scalar(v1, v2):
    return v1[0] * v2[0] + v1[1] * v2[1]

def Find_W(line1, p):
    return [line1[0] - p[0], line1[1] - p[1]]

def GetVector(line):
    return [line[2] - line[0], line[3] - line[1]]

def FindNormal(edge, inc_edge):
    n = [edge[3] - edge[1], edge[0] - edge[2]]
    if scalar(n, GetVector(inc_edge)) < 0:
        n = [-n[0], -n[1]]
    return n

def ConvertParametric(line, t):
    return [line[0] + (line[2] - line[0]) * t, line[1] + (line[3] - line[1]) * t]

def get_k(x1,y1,x2,y2):
    if x1 == x2:
        return 10000
    return (y2-y1)/(x2-x1)

def x_intersect(x1,y1,x2,y2,x3,y3,x4,y4):
    return ((x3*y4-y3*x4)/(x4-x3) - (x1*y2-y1*x2)/(x2-x1)) / ((y4-y3)/(x4-x3)-(y2-y1)/(x2-x1))

def get_y(x1,y1,x2,y2,x):
    return (y2-y1)*(x-x1)/(x2-x1) + y1

eps = 1e-3

def edges_intersect(polygon):
    for i in range(len(polygon) - 1):
        edge1 = polygon[i]
        x1 = edge1[0]
        y1 = 800 - edge1[1]
        x2 = edge1[2]
        y2 = 800 - edge1[3]
        for j in range(i + 2, len(polygon)):
            if i == 0 and j == len(polygon) - 1:
                continue
            edge2 = polygon[j]
            x3 = edge2[0]
            y3 = 800 - edge2[1]
            x4 = edge2[2]
            y4 = 800 - edge2[3]
            k1 = get_k(x1,y1,x2,y2)
            k2 = get_k(x3,y3,x4,y4)
            if abs(k1 - k2) < eps:
                continue
            if k1 == 10000:
                x = x1
                y = get_y(x3,y3,x4,y4,x)
                t1 = (y-y1)/(y2-y1)
                t2 = (x-x3)/(x4-x3)
            elif k2 == 10000:
                x = x3
                y = get_y(x1,y1,x2,y2,x)
                t2 = (y-y3)/(y4-y3)
                t1 = (x-x1)/(x2-x1)
            else:
                x = x_intersect(x1,y1,x2,y2,x3,y3,x4,y4)
                y = get_y(x1,y1,x2,y2,x)
                t1 = (x-x1)/(x2-x1)
                t2 = (x-x3)/(x4-x3)
                
            if t1 >= 0 and t1 <= 1 and t2 >= 0 and t2 <= 1:
                return True
    return False              
        

def CyrusBeck(edges, line): #алгоритм Кируса-Бека
    t_b, t_e = 0, 1  # изначально считаем, что отрезок полностью видимый.
    D = GetVector(line) # определяем направление отрезка
    for i in range(len(edges)):
        W = Find_W(line, edges[i])  # Wi = P1 - fi
        if i == len(edges) - 1:
            N = FindNormal(edges[i], edges[0])
        else:
            N = FindNormal(edges[i], edges[i + 1]) # находим вектор внутренней нормали
        Dscalar = scalar(D, N)
        Wscalar = scalar(W, N)
        if Dscalar == 0: # если отрезок параллелен i-ой стороне отсекателя
            if Wscalar < 0: # и находятся снаружи отсекателя
                return # то он невидимый
        else:
            t = -Wscalar / Dscalar
            # Если Dск > 0 - то точку пересечения
            # нужно отнести к группе, определяющей начало видимой части.
            if Dscalar > 0:
                if t > 1:
                    return # т. пересечения вне отрезка -> невидим
                else:
                    t_b = max(t_b, t) # иначе из точек, определяющих начало, выбираем макс
            # Если Dск < 0 - то точку пересечения нужно отнести к
            # группе, определяющей конец видимой части.
            elif Dscalar < 0:
                if t < 0:
                    return # т. пересечения вне отрезка -> невидим
                else:
                    t_e = min(t_e, t) # иначе из точек, определяющих конец, выбираем мин
    # Проверка видимости отрезка
    if t_e < t_b:
        return
    # Возвращаем начало и конец видимого отрезка.
    return [ConvertParametric(line, t_b), ConvertParametric(line, t_e)]


def FindSolution(line_list, edges):
    result_list = list()

    for i in range(len(line_list)):
        temp = CyrusBeck(edges, line_list[i])
        if temp:
            result_list.append(temp)
            
    return result_list


def SolutionWrapper(canvas_class, line_list, polygon):
    if len(polygon) < 2:
        print_error('Необходимо задать отсекатель')
        return
    if len(line_list) < 1:
        print_error('Для отсечения необходимо ввести хотя бы один отрезок')
        return
    polygon = polygon[-2]
    if not IsConvex(polygon) or edges_intersect(polygon):
        print_error("Введен невыпуклый многоугольник!")
        return

    res_list = FindSolution(line_list, polygon)
    for i in range(len(res_list)):
        canvas_class.draw_line([res_list[i][0][0], res_list[i][0][1], res_list[i][1][0], res_list[i][1][1]], "green")


root = Tk()
line_list = list()
contour = [[]]
temp_contour = []
setup_ui(root, "1200x800", "Отсечение отрезков выпуклым отсекателем")

canvas_class = paint_class(root)

create_label(root, "Координаты концов отрезка", [1000, 50])
create_label(root, "Начало:", [910, 100], 100)
entry_line_start = create_entry(root, [1050, 100])
create_label(root, "Конец:", [910, 150], 100)
entry_line_stop = create_entry(root, [1050, 150])
create_button("Добавить отрезок", lambda arg1=line_list, arg2=entry_line_start,
    arg3=entry_line_stop, arg4=canvas_class: add_line(arg1, arg2, arg3, arg4), [1000, 200])

create_label(root, "Чтобы ввести отрезок,\nнужно зажать ЛКМ и провести мышью", [1000, 280], 380)
create_label(root, "Чтобы ввести отсекатель, нужно\nзажать Shift и кликами ЛКМ добавлять вершины,\n для замыкания нажать Пробел(с зажатым Shift)", [1000, 350], 380)
create_label(root, "Ввод вершин контура:", [1000, 450])
create_label(root, "Координаты", [920, 500], 120)
entry_contour_start = create_entry(root, [1070, 500])
create_button("Добавить вершину", lambda arg1=temp_contour, arg2=entry_contour_start,
    arg4=canvas_class: add_contour(arg1, arg2, arg4), [1000, 550])
create_button("Замкнуть", lambda arg1=temp_contour, arg2=contour,
    arg3=canvas_class: close_contour(arg1, arg2, arg3), [1000, 600])

create_button("Отсечь", lambda arg1=canvas_class, arg2=line_list, arg3=contour:
    SolutionWrapper(arg1, arg2, arg3), [1000, 700])

create_button("Очистить экран", lambda arg1=canvas_class, arg2=line_list, arg3=contour:
    clear(arg1, arg2, arg3), [1000, 745])

set_binds(canvas_class, line_list, contour)
root.bind("<Shift-space>", lambda event, arg1=contour: canvas_class.keySpace_rectangle(event, arg1))

root.mainloop()
