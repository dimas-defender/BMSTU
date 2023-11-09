from tkinter import *
from tkinter import messagebox as mb
from math import *
import numpy as np


WIDTH, HEIGHT = 800, 800

FALSE = "-"

COLOR_BG = "black"  # "white"
COLOR_PIXEL = "white"  # "cyan"

FONT = "Verdana 12"

DEFAULT_BORDERS_Z = "-5 5"
DEFAULT_BORDERS_X = "-5 5"
DEFAULT_STEP_Z = "0.1"
DEFAULT_STEP_X = "0.01"


DEFAULT_ANGLE_X = 20
DEFAULT_ANGLE_Y = -10
DEFAULT_ANGLE_Z = 0

CHOICE = [
    "cos(x) * sin(z)",
    "sqrt(fabs(x * z))",
    "exp(cos(x) * sin(z))",
    "sin(x * z)"
]

EPS = 1e-6
SCALE = 40

def print_info(string_info):
    mb.showinfo(title="Информация", message=string_info)


def print_error(string_error):
    mb.showerror(title="Ошибка", message=string_error)


def create_button(str, function, coordinates):
    button = Button(text=str, width=40,
                    command=function,  bg="thistle3")
    button.place(x=coordinates[0], y=coordinates[1], anchor="center")


def create_label(root, str, coordinates):
    label = Label(root, text=str, bg="lavender", width=25,
                  font="Verdana 12")
    label.place(x=coordinates[0], y=coordinates[1], anchor="center", width=200)


def settings_interface(root, size, title):
    root.title(title)
    root.geometry(size)
    root.configure(bg="lavender")
    root.resizable(width=False, height=False)


def option_menu(master, option, coordinates):
    variable = StringVar(master)
    variable.set(option[0])
    w = OptionMenu(master, variable, *option)
    w.place(x=coordinates[0], y=coordinates[1], anchor="center", width=200)
    return variable


def selection(count, list_text, coordinates):
    # Выбор.
    var = IntVar()
    var.set(0)
    list_method = list()

    for i in range(count):
        method = Radiobutton(text=list_text[i], variable=var,
                             value=i, bg="lavender", width=25, font="Verdana 12")
        method.place(x=coordinates[0],
                     y=coordinates[1] + 25 * i, anchor="center")
        list_method.append(method)

    return var


def create_entry(root, coordinates, text="123"):
    entry = Entry(root, width="50")
    entry.place(x=coordinates[0], y=coordinates[1], anchor="center", width=150)
    entry.insert(0, text)
    return entry


def create_list_box(root, coordinates):
    list_box = Listbox(root, width=48, height=17, bg="lavender")
    list_box.insert(END, "(x y)")
    list_box.place(x=coordinates[0], y=coordinates[1])
    scroll = Scrollbar(command=list_box.yview)
    scroll.place(x=coordinates[0] + 390, y=coordinates[1], height=310)
    return list_box


def settings_bind(canvas_class, line_list, contour):
    # Button-1 # ЛКМ
    # Button-3 # ПКМ
    # space # пробел
    canvas_class.canvas.bind(
        "<ButtonPress>", lambda event: canvas_class.keyPress_rectangle(event))

    canvas_class.canvas.bind(
        "<ButtonRelease>", lambda event, arg1=line_list: canvas_class.keyRelease_rectangle(event, line_list, "red"))

    canvas_class.canvas.bind(
        "<Motion>", lambda event: canvas_class.Motion_rectangle(event))

    canvas_class.canvas.bind(
        "<Shift-ButtonPress>", lambda event: canvas_class.keyPress_rectangle(event))

    canvas_class.canvas.bind(
        "<Shift-ButtonRelease>", lambda event, arg1=contour: canvas_class.keyRelease_rectangle(event, arg1, "blue"))

    canvas_class.canvas.bind(
        "<Shift-Motion>", lambda event: canvas_class.Motion_rectangle(event))


def clear(canvas_class, cutter, contour):
    canvas_class.clear_all()

    for i in range(len(cutter) - 1, -1, -1):
        del cutter[i]

    for i in range(len(contour) - 1, -1, -1):
        del contour[i]

    contour.append([])
    cutter.append([])


def float_answer(answer):
    try:
        if answer == "":
            print_error("У вас пустой ввод")
            return FALSE
        if len(answer.split()) != 1:
            print_error("У вас больше одного числа.")
            return FALSE

        a = float(answer)
    except:
        print_error("Некорректный ввод")
        return FALSE

    return a


def get_two_answer(answer):
    try:
        if answer == "":
            print_error("У вас пустое поле!")
            return FALSE, FALSE
        if len(answer.split()) != 2:
            print_error("Некорректный ввод.")
            return FALSE, FALSE
        a, b = map(int, answer.split())
    except:
        print_error("Координаты должны быть целого типа")
        return FALSE, FALSE
    return (a, b)


def f1(x, z):
    # return x + z
    return cos(x) * sin(z)


def f2(x, z):
    return sqrt(fabs(x * z))


def f3(x, z):
    return exp(cos(x) * sin(z))


def f4(x, z):
    return sin(x * z)



def scale(x, y):
    x *= SCALE
    y *= SCALE
    x += WIDTH // 2
    y += HEIGHT // 2
    return round(x), round(y)
    
def convers(arg):
    return arg * pi / 180


def rotateX(x, y, z, angle):
    angle = convers(angle)
    y = cos(angle) * y - sin(angle) * z
    return x, y


def rotateY(x, y, z, angle):
    angle = convers(angle)
    x = cos(angle) * x - sin(angle) * z
    return x, y


def rotateZ(x, y, z, angle):
    angle = convers(angle)
    buf = x
    x = cos(angle) * x - sin(angle) * y
    y = cos(angle) * y + sin(angle) * buf
    return x, y


def transform(x, y, z, angles):
    x, y = rotateX(x, y, z, angles[0])
    x, y = rotateY(x, y, z, angles[1])
    x, y = rotateZ(x, y, z, angles[2])
    return scale(x, y)



class paint_class():
    canvas = None

    def __init__(self, root):
        self.canvas = Canvas(root, width=WIDTH, height=HEIGHT, bg=COLOR_BG)
        self.canvas.place(x=0, y=0)

    def clear_all(self):
        self.canvas.delete(ALL)

    def create_pixel(self, x, y, color=COLOR_PIXEL):
        # x *= SCALE
        # y *= SCALE
        # x += WIDTH // 2
        # y = HEIGHT // 2 - y

        self.canvas.create_line(round(x), round(y),
            round(x), round(y) + 1, fill=color)

    def draw_line(self, begin, end, color=COLOR_PIXEL):
        # if begin[0] ==  begin[1] and end[0] == end[1]:
        #     self.create_pixel(begin[0], begin[1])
        #     return
        self.canvas.create_line(begin[0], begin[1], end[0], end[1], fill=color)

    def in_canvas(self, event):
        self.canvas.delete("coordinates")
        self.canvas.create_text(event.x + 45, event.y - 15,  text=f"({event.x}, {event.y})",
                                font=FONT, tags="coordinates",  fill="blue")


def coordinate_transform(begin, end):
    for i in range(2):
        begin[i] *= SCALE
        end[i] *= SCALE

    begin[0] += WIDTH // 2
    end[0] += WIDTH // 2
    begin[1] = HEIGHT // 2 - begin[1]
    end[1] = HEIGHT // 2 - end[1]
    # return round(begin[0]), round(begin[1]), round(end[0]), round(end[1])
    return int(round(begin[0])), int(round(begin[1])), int(round(end[0])), int(round(end[1]))
    # return begin[0], begin[1], end[0], end[1]


top = [0] * WIDTH
bottom = [HEIGHT] * WIDTH


# Подпрограмма вычисляет пересечение с горизонтом.
def Intersection(x1, y1, x2, y2, arr, canvas_class):
    dx = x2 - x1
    dyc = y2 - y1
    dyp = arr[x2] - arr[x1]
    if dx == 0:
        xi = x2
        yi = arr[x2]
        return xi, yi
    if y1 == arr[x1] and y2 == arr[x2]:
        return x1, y1
    m = dyc / dx
    xi = x1 - round(dx * (y1 - arr[x1]) / (dyc - dyp))
    yi = round((xi - x1) * m + y1)
    return xi, yi

# Подпрограмма, определяющая видимость точки.
# flag:
# 0 - невидима.
# 1 - выше верхнего.
# -1 - ниже нижнего.
def Visible(x, y):  # Visible point
    global top, bottom
    # Если точка, ниже нижнего горизонта (или на нем)
    # То она видима. 
    if y <= bottom[x]:
        return -1
    # Если точка выше верхнего горизонта (или на нем)
    # То она видима.
    if y >= top[x]:  
        return 1
    # Иначе она невидима.
    return 0


# Подпрограмма заполнения массивов горизонтов между x1 и x2 
# На основе линейной интерполяции.
def Horizon(x1, y1, x2, y2, canvas_class):
    global top, bottom
    # Проверка вертикальности наклона.
    if (x2 - x1 == 0):
        top[x2] = max(top[x2], y2)
        bottom[x2] = min(bottom[x2], y2)
        return
    # Иначе вычисляем наклон.
    m = (y2 - y1) / (x2 - x1)
    # Движемся по x с шагом 1, чтобы заполнить 
    # Массивы от x1 до x2.
    for x in range(x1, x2 + 1):
        y = round(m * (x - x1) + y1)
        top[x] = max(top[x], y)
        bottom[x] = min(bottom[x], y)


# Функция обработки и обновления точек бокового рёбра
def Side(x, y, xe, ye,canvas_class):
    if (xe != -1):
        # Если кривая не первая
        canvas_class.draw_line([xe, ye], [x, y])
        Horizon(xe, ye, x, y, canvas_class)
    xe = x
    ye = y
    return xe, ye


def FloatHorizon(borders_x, x_step, borders_z, z_step, canvas_class, f, angles):
    global top, bottom
    # Инициализируем начальными значениями массивы горизонтов.
    top = [0] * WIDTH  # Верхний.
    bottom = [HEIGHT] * WIDTH  # Нижний.

    x_start = borders_x[0]
    x_end = borders_x[1]

    z_start = borders_z[0]
    z_end = borders_z[1]

    x_left, y_left = -1, -1
    x_right, y_right = -1, -1

    z = z_end
    while z >= z_start - z_step / 2:

        x_prev = x_start
        y_prev = f(x_start, z)
        x_prev, y_prev = transform(x_prev,y_prev, z, angles)

        flag_prev = Visible(x_prev, y_prev)
        #
        x_left, y_left = Side(x_prev, y_prev, x_left, y_left, canvas_class)
        x = x_start
        while x <= x_end + x_step / 2:
            y_curr = f(x, z)
            x_curr, y_curr = transform(x, y_curr, z, angles)
            # Проверка видимости текущей точки. 
            flag_curr = Visible(x_curr, y_curr)
            # Равенство флагов означает, что обе точки находятся
            # Либо выше верхнего горизонта, либо ниже нижнего,
            # Либо обе невидимы.
            if flag_curr == flag_prev:
                # Если текущая вершина выше верхнего горизонта
                # Или ниже нижнего (Предыдущая такая же)
                if flag_curr != 0:
                    # Значит отображаем отрезок от предыдущей до текущей.
                    canvas_class.draw_line([x_prev, y_prev], [x_curr, y_curr])
                    Horizon(x_prev, y_prev, x_curr, y_curr, canvas_class)
                # flag_curr == 0 означает, что и flag_prev == 0,
                # А значит часть от flag_curr до flag_prev невидима. Ничего не делаем.
            else:
                # Если видимость изменилась, то
                # Вычисляем пересечение.
                if flag_curr == 0:
                    if flag_prev == 1:
                        # Сегмент "входит" в верхний горизонт.
                        # Ищем пересечение с верхним горизонтом.
                        xi, yi = Intersection(x_prev, y_prev, x_curr, y_curr, top, canvas_class)
                    else: # flag_prev == -1 (flag_prev нулю (0) не может быть равен, т.к. мы обработали это выше).
                        # Сегмент "входит" в нижний горизонт.
                        # Ищем пересечение с нижним горизонтом.
                        xi, yi = Intersection(x_prev, y_prev, x_curr, y_curr, bottom, canvas_class)
                    # Отображаем сегмент, от предыдущий точки, до пересечения.
                    canvas_class.draw_line([x_prev, y_prev], [xi, yi])
                    Horizon(x_prev, y_prev, xi, yi, canvas_class)
                else:
                    if flag_curr == 1:
                        if flag_prev == 0:
                            # Сегмент "выходит" из верхнего горизонта.
                            # Ищем пересечение с верхним горизонтом. 
                            xi, yi = Intersection(x_prev, y_prev, x_curr, y_curr, top, canvas_class)
                            # Отображаем сегмент от пересечения до текущей точки.
                            canvas_class.draw_line([xi, yi], [x_curr, y_curr])
                            Horizon(xi, yi, x_curr, y_curr, canvas_class) 
                        else: # flag_prev == -1
                            # Сегмент начинается с точки, ниже нижнего горизонта
                            # И заканчивается в точке выше верхнего горизонта.
                            # Нужно искать 2 пересечения.
                            # Первое пересечение с нижним горизонтом.
                            xi, yi = Intersection(x_prev, y_prev, x_curr, y_curr, bottom, canvas_class)
                            # Отображаем сегмент от предыдущей то пересечения.
                            canvas_class.draw_line([x_prev, y_prev], [xi, yi])
                            Horizon(x_prev, y_prev, xi, yi, canvas_class)
                            # Второе пересечение с верхним горизонтом.
                            xi, yi = Intersection(x_prev, y_prev, x_curr, y_curr, top, canvas_class)
                            # Отображаем сегмент от пересечения до текущей.
                            canvas_class.draw_line([xi, yi], [x_curr, y_curr])
                            Horizon(xi, yi, x_curr, y_curr, canvas_class)
                    else: # flag_curr == -1
                        if flag_prev == 0:
                            # Сегмент "выходит" из нижнего горизонта.
                            # Ищем пересечение с нижним горизонтом.
                            xi, yi = Intersection(x_prev, y_prev, x_curr, y_curr, bottom, canvas_class)
                            canvas_class.draw_line([xi, yi], [x_curr, y_curr])
                            Horizon(xi, yi, x_curr, y_curr, canvas_class)  
                        else:
                            # Сегмент начинается с точки, выше верхнего горизонта
                            # И заканчивается в точке ниже нижнего горизонта.
                            # Нужно искать 2 пересечения.
                            # Первое пересечение с верхним горизонтом.
                            xi, yi = Intersection(x_prev, y_prev, x_curr, y_curr, top, canvas_class)
                            # Отображаем сегмент от предыдущей до пересечения.
                            canvas_class.draw_line([x_prev, y_prev], [xi, yi])
                            Horizon(x_prev, y_prev, xi, yi, canvas_class)
                            # Ищем второе пересечение с нижним горизонтом.
                            xi, yi = Intersection(x_prev, y_prev, x_curr, y_curr, bottom, canvas_class)
                            # Отображаем сегмент от пересечения до текущей.
                            canvas_class.draw_line([xi, yi], [x_curr, y_curr])
                            Horizon(xi, yi, x_curr, y_curr, canvas_class)
            x_prev, y_prev = x_curr, y_curr
            flag_prev = flag_curr
            x += x_step
        x_right, y_right = Side(x_prev, y_prev, x_right, y_right,canvas_class)
        z -= z_step


def SolutionWrapper(choice, borders, step, angles, canvas_class):
    borders_x = get_two_answer(borders[0].get())
    if borders_x[0] == FALSE:
        return

    step_x = float_answer(step[0].get())
    if step_x == FALSE:
        return

    borders_z = get_two_answer(borders[1].get())
    if borders_z[0] == FALSE:
        return

    step_z = float_answer(step[1].get())
    if step_z == FALSE:
        return

    angle_x = float_answer(angles[0].get())
    if angle_x == FALSE:
        return

    angle_y = float_answer(angles[1].get())
    if angle_y == FALSE:
        return

    angle_z = float_answer(angles[2].get())
    if angle_z == FALSE:
        return

    f = [f1, f2, f3, f4]
    index = CHOICE.index(choice.get())

    canvas_class.clear_all()
    FloatHorizon(borders_x, step_x, borders_z, step_z,
                 canvas_class, f[index], [angle_x, angle_y, angle_z])


root = Tk()
settings_interface(root, "1200x800", "Алгоритм Плавающего горизонта")
canvas_class = paint_class(root)

choice_func = option_menu(root, CHOICE, [1000, 30])

create_label(root, "Пределы по x:", [1000, 100])
borders_x = create_entry(root, [1000, 125], DEFAULT_BORDERS_X)

create_label(root, "Шаг по x:", [1000, 150])
step_x = create_entry(root, [1000, 175], DEFAULT_STEP_X)

create_label(root, "Пределы по z:", [1000, 225])
borders_z = create_entry(root, [1000, 250], DEFAULT_BORDERS_Z)

create_label(root, "Шаг по z:", [1000, 275])
step_z = create_entry(root, [1000, 300], DEFAULT_STEP_Z)

create_label(root, "∠ по x:", [1000, 350])
angle_x = create_entry(root, [1000, 375], DEFAULT_ANGLE_X)

create_label(root, "∠ по y:", [1000, 400])
angle_y = create_entry(root, [1000, 425], DEFAULT_ANGLE_Y)

create_label(root, "∠ по z:", [1000, 450])
angle_z = create_entry(root, [1000, 475], DEFAULT_ANGLE_Z)

create_button("Отобразить", lambda arg1=choice_func, arg2=[borders_x, borders_z], arg3=[step_x, step_z], arg4=[angle_x, angle_y, angle_z], arg5=canvas_class:  SolutionWrapper(arg1, arg2, arg3, arg4, arg5), [1000, 700])
canvas_class.canvas.bind("<Motion>", lambda event: canvas_class.in_canvas(event))

root.mainloop()
