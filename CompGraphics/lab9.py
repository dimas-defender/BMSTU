#Алгоритм отсечения произвольного многоугольника выпуклым отсекателем
#ИУ7-44Б Жабин Дмитрий

from tkinter import *
from tkinter import messagebox as mb
from math import fabs
from copy import deepcopy

CHOICE = ["Отсекатель", "Отсекаемый многоугольник"]

WIDTH, HEIGHT = 800, 800
FALSE = "-"

COLOR_BG = ((255, 255, 255), 'white')
COLOR_FILL_INIT = ((0.0, 0.0, 0.0), "#000000")
COLOR_LINE = ((136, 0, 250), "#8800fa")

button_color = 'Khaki'
window_color = 'Orange'
label_color = 'Khaki'

# LINE

X = 0
Y = 1

X1 = 0
Y1 = 1
X2 = 2
Y2 = 3

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


def selection(count, list_text, coords):
    var = IntVar()
    var.set(0)
    list_method = list()

    for i in range(count):
        method = Radiobutton(text=list_text[i], variable=var, value=i, bg=button_color, width=25, font="Arial 13")
        method.place(x=coords[0], y=coords[1] + 25 * i, anchor = "center")
        list_method.append(method)

    return var


def create_entry(root, coords):
    entry = Entry(root, width = "50", font = "Arial 13")
    entry.place(x = coords[0], y = coords[1], anchor = "center", width = 140, height = 25)
    return entry

def set_binds(canvas_class, line_list, contour):

    canvas_class.canvas.bind("<ButtonPress>", lambda event: canvas_class.keyPress_rectangle(event))
    canvas_class.canvas.bind("<ButtonRelease>", lambda event, arg1=line_list: canvas_class.keyRelease_rectangle(event, line_list, "red"))
    canvas_class.canvas.bind("<Motion>", lambda event: canvas_class.Motion_rectangle(event))
    canvas_class.canvas.bind("<Shift-ButtonPress>", lambda event: canvas_class.keyPress_rectangle(event))
    canvas_class.canvas.bind("<Shift-ButtonRelease>", lambda event, arg1=contour: canvas_class.keyRelease_rectangle(event, arg1, "blue"))
    canvas_class.canvas.bind("<Shift-Motion>", lambda event: canvas_class.Motion_rectangle(event))


def clear(canvas_class, cutter, contour):
    canvas_class.clear_all()

    for i in range(len(cutter) - 1, -1, -1):
        del cutter[i]

    for i in range(len(contour) - 1, -1, -1):
        del contour[i]

    contour.append([])
    cutter.append([])


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

    def draw_line(self, coords, color="blue", w=1):
        self.canvas.create_line(coords[0], coords[1], coords[2], coords[3], fill=color, width=w)

    def keyPress(self, event):
        if not self.flag:
            self.start = [event.x, event.y]
        self.flag = True

    def keyRelease(self, event, line_list):
        self.canvas.coords("line_temp", 0, 0, 0, 0)

        self.canvas.create_line(self.start[0], self.start[1], event.x, event.y, fill="red")
        line_list[-1].append([self.start[0], self.start[1], event.x, event.y])

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

    def keyRelease_rectangle(self, event, lst, color):
        if self.start:
            self.canvas.create_line(self.start[0], self.start[1], self.stop[0], self.stop[1], fill=color)
            lst[-1].append([self.start[0], self.start[1], self.stop[0], self.stop[1]])

    def keySpace_rectangle(self, event, lst, color):
        if (len(lst[-1]) > 1):
            self.canvas.create_line(lst[-1][-1][2], lst[-1][-1][3], lst[-1][0][0], lst[-1][0][1], fill=color)
            lst[-1].append([lst[-1][-1][2], lst[-1][-1][3], lst[-1][0][0], lst[-1][0][1]])
            lst.append([])
            self.canvas.coords("line_rectangle", 0, 0, 0, 0)
            self.flag = False
            self.start = False
            self.stop = False


def close_contour(cutter, contour, lst, canvas_class, selection):
    if len(lst) >= 2:
        choice = selection.get()
        color = "blue"
        if choice == 0:
            color = "red"
        canvas_class.canvas.create_line(lst[-1][0], lst[-1][1], lst[0][0], lst[0][1], fill=color)
        if choice == 1:
            for i in range(len(lst) - 1):
                contour[-1].append([lst[i][0], lst[i][1], lst[i+1][0], lst[i+1][1]])
            contour[-1].append([lst[-1][0], lst[-1][1], lst[0][0], lst[0][1]])
            contour.append([])
        elif choice == 0:
            for i in range(len(lst) - 1):
                cutter[-1].append([lst[i][0], lst[i][1], lst[i+1][0], lst[i+1][1]])
            cutter[-1].append([lst[-1][0], lst[-1][1], lst[0][0], lst[0][1]])
            cutter.append([])

    for i in range(len(lst) - 1, -1, -1):
        del lst[i]


def add_contour(lst, start, canvas_class, selection):
    choice = selection.get()
    color = "blue"
    if choice == 0:
        color = "red"
    start = get_coords(start.get())
    if start[0] == FALSE:
        return
    lst.append([start[0], start[1]])
    if len(lst) >= 2:
        canvas_class.canvas.create_line(lst[-1][0], lst[-1][1], lst[-2][0], lst[-2][1], fill=color)


def GetNodes(polygon):
    nodes = list()
    for i in range(len(polygon)):
        nodes.append([polygon[i][0], polygon[i][1]])
    return nodes


def FindDirection(line):
    return [line[1][0] - line[0][0], line[1][1] - line[0][1]]


def Find_W(p1, p2):
    return [p1[0] - p2[0], p1[1] - p2[1]]


def ConvertParametric(line, t):
    return [round(line[0][0] + (line[1][0] - line[0][0]) * t), round(line[0][1] + (line[1][1] - line[0][1]) * t)]


def IsVisible(point, peak1, peak2, peak3):
    n = FindNormal(peak1, peak2, peak3)
    if Scalar(n, GetVector([peak2, point])) < 0:
        return False
    return True


def IsIntersection(ed1, ed2, peak):
    #определяем видимость вершин относительно рассматриваемого ребра отсекателя
    visible1 = IsVisible(ed1[0], ed2[0], ed2[1], peak)
    visible2 = IsVisible(ed1[1], ed2[0], ed2[1], peak)
    if not (visible1 ^ visible2): #если разная видимость вершин, то есть пересечение
        return False
    N = FindNormal(ed2[0], ed2[1], peak)
    D = FindDirection(ed1)
    W = Find_W(ed1[0], ed2[0])
    DScalar = Scalar(D, N)
    WScalar = Scalar(W, N)
    t = -WScalar/DScalar
    return ConvertParametric(ed1, t)


def Scalar(v1, v2):
    return v1[0] * v2[0] + v1[1] * v2[1]


def FindNormal(peak1, peak2, peak3):
    n = [peak2[1] - peak1[1], peak1[0] - peak2[0]]
    if Scalar([peak3[0] - peak2[0], peak3[1] - peak2[1]], n) < 0: #нужна внутр.нормаль
        n = [-n[0], -n[1]]
    return n


def GetVector(line):
    return [line[1][0] - line[0][0], line[1][1] - line[0][1]]


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


def SutherlandHodgman(cutter, polygon):
    cutter.append(cutter[0]) #для обработки последнего ребра
    cutter.append(cutter[1]) #для поиска нормали к последнему ребру
    
    for i in range(len(cutter) - 2):
        new = []
        f = polygon[0]  #запоминаем первую вершину
        if IsVisible(f,  cutter[i], cutter[i + 1], cutter[i + 2]):
            new.append(f)
        s = polygon[0]
        for j in range(1, len(polygon)):
            t = IsIntersection([s, polygon[j]], [cutter[i], cutter[i + 1]], cutter[i + 2])
            if t:  #если есть пересечение, то заносим его в новый массив вершин
                new.append(t)
            s = polygon[j]
            if IsVisible(s, cutter[i], cutter[i + 1], cutter[i + 2]):
                new.append(s) #если тек.вершина видна, то заносим ее в новый массив вершин
        if not len(new): #невидимость многоугольника относительно текущей границы отсекателя
            return False
        t = IsIntersection([s, f], [cutter[i], cutter[i + 1]], cutter[i + 2])
        if t:
            new.append(t)
        polygon = deepcopy(new)
    return polygon


def SolutionWrapper(canvas_class, cutter, polygon):
    if len(cutter) < 2:
        print_error("Необходимо задать отсекатель!")
        return

    if len(polygon) < 2:
        print_error("Необходимо задать отсекаемый многоугольник!")
        return

    cutter = cutter[-2]
    if not IsConvex(cutter) or edges_intersect(cutter):
        print_error("Отсекатель не выпуклый!")
        return

    polygon = GetNodes(polygon[-2])
    cutter = GetNodes(cutter)
    result = SutherlandHodgman(cutter, polygon)

    if not result:
        return

    result.append(result[0])
    for i in range(len(result) - 1):
        canvas_class.draw_line([round(result[i][0]), round(result[i][1]),
             round(result[i+1][0]), round(result[i+1][1])], "green", 2)

root = Tk()

cutter = [[]]
contour = [[]]
temp_contour = []
setup_ui(root, "1200x800", "Отсечение многоугольника выпуклым отсекателем")

canvas_class = paint_class(root)

figure_selection = selection(2, CHOICE, [1000, 100])

create_label(root, "Ввод вершин:", [1000, 55], 200)
create_label(root, "Координаты", [920, 175], 120)
entry = create_entry(root, [1070, 175])
create_button("Добавить вершину", lambda arg1=temp_contour, arg2=entry,
        arg3=canvas_class, arg4=figure_selection: add_contour(arg1, arg2, arg3, arg4), [1000, 225])
create_button("Замкнуть", lambda arg1=cutter,  arg2=contour, arg3=temp_contour,
        arg4=canvas_class, arg5=figure_selection: close_contour(arg1, arg2, arg3, arg4, arg5), [1000, 275])

create_label(root, "Вершины отсекателя добавляются\n кликами ЛКМ, для замыкания нужно\n нажать пробел", [1000, 400], 350)
create_label(root, "Отсекаемый многоугольник вводится\n аналогично, но с зажатым Shift", [1000, 470], 350)

create_button("Отсечь", lambda arg1=canvas_class, arg2=cutter, arg3=contour: SolutionWrapper(arg1, arg2, arg3), [1000, 620])
create_button("Очистить экран", lambda arg1=canvas_class, arg2=cutter, arg3=contour: clear(arg1, arg2, arg3), [1000, 670])

set_binds(canvas_class, cutter, contour)
root.bind("<Shift-space>", lambda event, arg1=contour: canvas_class.keySpace_rectangle(event, arg1, "blue"))
root.bind("<space>", lambda event, arg1=cutter: canvas_class.keySpace_rectangle(event, arg1, "red"))

root.mainloop()
