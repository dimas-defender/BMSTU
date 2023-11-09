#Алгоритм отсечения отрезков регулярным отсекателем
#ИУ7-44Б Жабин Дмитрий

from tkinter import *
from tkinter import messagebox as mb

WIDTH, HEIGHT = 800, 800
FALSE = "-"

COLOR_BG = ((255, 255, 255), 'white')
COLOR_FILL_INIT = ((0.0, 0.0, 0.0), "#000000")
COLOR_LINE = ((136, 0, 250), "#8800fa")

# RECTANGLE

LEFT = 0
RIGHT = 1
DOWN = 2
UP = 3

# LINE

X = 0
Y = 1

X1 = 0
Y1 = 1
X2 = 2
Y2 = 3

VISIBLE_LINE = 1
PARTLY_VISIBLE_LINE = 0
INVISIBLE_LINE = -1

NORMAL_LINE = 1
HORIZONTAL_LINE = 0
VERTICAL_LINE = -1

button_color = 'Khaki'
window_color = 'Orange'
label_color = 'Khaki'


def print_info(string_info):
    mb.showinfo(title="Информация", message=string_info)

def print_error(string_error):
    mb.showerror(title="Ошибка", message=string_error)

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
    entry = Entry(root, width="50", font = "Arial 13")
    entry.place(x=coords[0], y=coords[1], anchor="center", width=150, height = 25)
    return entry

def settings_bind(canvas_class, line_list, contour):
    
    canvas_class.canvas.bind("<ButtonPress>", lambda event: canvas_class.keyPress(event))
    canvas_class.canvas.bind("<ButtonRelease>", lambda event, arg1=line_list: canvas_class.keyRelease(event, line_list))
    canvas_class.canvas.bind("<Motion>", lambda event: canvas_class.Motion(event))
    canvas_class.canvas.bind("<Shift-ButtonPress>", lambda event: canvas_class.keyPress_rectangle(event))
    canvas_class.canvas.bind("<Shift-ButtonRelease>", lambda event, arg1=contour: canvas_class.keyRelease_rectangle(event, arg1))
    canvas_class.canvas.bind("<Shift-Motion>", lambda event: canvas_class.Motion_rectangle(event))


def clear(canvas_class, points_list):
    canvas_class.clear_all()

    for i in range(len(points_list) - 1, -1, -1):
        del points_list[i]


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

    def __init__(self, root):
        self.canvas = Canvas(root, width=WIDTH, height=HEIGHT, bg="white")
        self.canvas.place(x=0, y=0)
        self.initial_parameters_canvas()

    def clear_all(self):
        self.canvas.delete(ALL)
        self.initial_parameters_canvas()

    def initial_parameters_canvas(self):
        self.canvas.create_line(0, 0, 0, 0, tags="line_temp")
        self.canvas.create_rectangle(0, 0, 0, 0, tags="rectangle_temp", outline="black")
        self.canvas.create_rectangle(0, 0, 0, 0, tags="contour", outline="black")

    def draw_line(self, coords, color="blue"):
        self.canvas.create_line(coords[0], coords[1], coords[2], coords[3], fill=color)

    def new_rectangle(self, start, stop):
        self.canvas.coords("contour", start[0], start[1], stop[0], stop[1])
        self.canvas.coords("rectangle_temp", 0, 0, 0, 0)

    def keyPress(self, event):
        if not self.flag:
            self.start = [event.x, event.y]
        self.flag = True


    def keyRelease(self, event, line_list):
        self.canvas.coords("line_temp", 0, 0, 0, 0)
        if len(line_list) >= 10:
            self.start = False
            self.flag = False
            print_error("Нельзя рисовать более 10 линий!")
            return

        self.canvas.create_line(self.start[0], self.start[1], event.x, event.y, fill = "red")
        line_list.append([self.start[0], self.start[1], event.x, event.y])

        self.flag = False
        self.start = False

    def Motion(self, event):
        if self.start:
            self.canvas.coords("line_temp", self.start[0], self.start[1], event.x, event.y)

    def keyPress_rectangle(self, event):
        self.start = [event.x, event.y]
        self.flag = True

    def keyRelease_rectangle(self, event, lst):
        self.canvas.coords("line_temp", 0, 0, 0, 0)
        self.canvas.coords("contour", self.start[0], self.start[1], event.x, event.y)

        for i in range(len(lst) - 1, -1, -1):
            del lst[i]

        if self.start[0] < event.x:
            lst.append(self.start[0])
            lst.append(event.x)
        else:
            lst.append(event.x)
            lst.append(self.start[0])

        if self.start[1] > event.y:
            lst.append(self.start[1])
            lst.append(event.y)
        else:
            lst.append(event.y)
            lst.append(self.start[1])

        self.flag = False
        self.start = False

    def Motion_rectangle(self, event):

        if self.start:
            self.canvas.coords("rectangle_temp", self.start[0], self.start[1], event.x, event.y)


def add_contour(lst, start, stop, canvas_class):
    start = get_coords(start.get())
    if start[0] == FALSE:
        return

    stop = get_coords(stop.get())
    if stop[0] == FALSE:
        return

    if start == stop:
        print_error("Противоположные вершины отсекателя совпадают!")
        return

    canvas_class.new_rectangle(start, stop)

    for i in range(len(lst) - 1, -1, -1):
        del lst[i]

    if start[0] < stop[0]:
        lst.append(start[0])
        lst.append(stop[0])
    else:
        lst.append(stop[0])
        lst.append(start[0])

    if start[1] > stop[1]:
        lst.append(start[1])
        lst.append(stop[1])
    else:
        lst.append(stop[1])
        lst.append(start[1])


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

    # if start == stop:
    #     print_error("Начало и конец отрезка совпадают!")
    #     return

    if [start[0], start[1], stop[0], stop[1]] in lst:
        print_error("Данный отрезок уже введен!")
        return

    canvas.canvas.create_line(start[0], start[1], stop[0], stop[1], fill="red")
    lst.append([start[0], start[1], stop[0], stop[1]])

def multiplication_code(code1, code2):
    result = 0
    for i in range(len(code1)):
        result += code1[i] * code2[i]
    return result


def sum_code(code):
    result = 0
    for c in code:
        result += c
    return result


def create_code(point, rectangle):
    result_list = [0, 0, 0, 0]

    result_list[0] = 1 if point[X] < rectangle[LEFT] else 0
    result_list[1] = 1 if point[X] > rectangle[RIGHT] else 0
    result_list[2] = 1 if point[Y] > rectangle[DOWN] else 0
    result_list[3] = 1 if point[Y] < rectangle[UP] else 0

    return result_list


def is_visible(code_1, code_2, rectangle):

    if not sum_code(code_1) and not sum_code(code_2):
        return VISIBLE_LINE

    if multiplication_code(code_1, code_2) != 0:
        return INVISIBLE_LINE

    return PARTLY_VISIBLE_LINE


def cohen_sutherland(line, rectangle):
    # Возвращает концы видимого отрезка (INVISIBLE_LINE если невидим)
    flag, m = NORMAL_LINE, 1
    if line[X1] - line[X2] == 0:
        flag = VERTICAL_LINE
    else:
        m = (line[Y2] - line[Y1]) / (line[X2] - line[X1])
        if m == 0:
            flag = HORIZONTAL_LINE

    # Итерируемся (x_левое, x_правое, y_нижнее, y_верхнее)
    for i in range(4):
        code_1 = create_code([line[X1], line[Y1]], rectangle)
        code_2 = create_code([line[X2], line[Y2]], rectangle)
        vis = is_visible(code_1, code_2, rectangle)
        if vis == VISIBLE_LINE:
            return line
        elif vis == INVISIBLE_LINE:
            return INVISIBLE_LINE

        # Проверяем пересечение отрезка и стороны отсекателя.
        
        if code_1[i] == code_2[i]: # два нуля - пересечения нет
            continue

        if not code_1[i]: #чтобы затем отбросить именно невидимую часть
            line[X1], line[Y1], line[X2], line[Y2] = line[X2], line[Y2], line[X1], line[Y1]

        if flag != VERTICAL_LINE:
            if i < 2: #проверяем пересечение с вертикальными сторонами
                line[Y1] = m * (rectangle[i] - line[X1]) + line[Y1]
                line[X1] = rectangle[i]
                continue
            else: #с горизонтальными сторонами
                line[X1] = (1 / m) * (rectangle[i] - line[Y1]) + line[X1]
        line[Y1] = rectangle[i]

    return line


def find_solution(line_list, rectangle):
    result_list = list()

    for i in range(len(line_list)):
        temp_line = line_list[i].copy() #чтобы не изменять line_list, нужно для дальнейшего отсечения новым отсекателем
        res = cohen_sutherland(temp_line, rectangle)
        if res != INVISIBLE_LINE:
            result_list.append(res)

    return result_list


def solution_wrapper(canvas_class, line_list, contour):
    result_list = find_solution(line_list, contour)
    for i in range(len(result_list)):
        canvas_class.draw_line(result_list[i])


root = Tk()
line_list = list()
contour = list()
setup_ui(root, "1200x800", "Отсечение отрезков регулярным отсекателем")

canvas_class = paint_class(root)

create_label(root, "Координаты концов отрезка", [1000, 100])
create_label(root, "Начало:", [910, 150], 100)
entry_line_start = create_entry(root, [1050, 150])
create_label(root, "Конец:", [910, 200], 100)
entry_line_stop = create_entry(root, [1050, 200])
create_button("Добавить отрезок", lambda arg1=line_list, arg2=entry_line_start,
    arg3=entry_line_stop, arg4=canvas_class: add_line(arg1, arg2, arg3, arg4), [1000, 250])

create_label(root, "Отрезки и отсекатели можно добавлять,\n зажав ЛКМ (для отсекателей\nдополнительно зажать Shift)", [1000, 330], 380)
create_label(root, "Координаты вершин отсекателя", [1000, 400], 300)
create_label(root, "Левый верхний угол:", [920, 450], 200)
entry_contour_start = create_entry(root, [1110, 450])
create_label(root, "Правый нижний угол:", [920, 500], 200)
entry_contour_stop = create_entry(root, [1110, 500])
create_button("Добавить отсекатель", lambda arg1=contour, arg2=entry_contour_start,
    arg3=entry_contour_stop, arg4=canvas_class: add_contour(arg1, arg2, arg3, arg4), [1000, 550])

create_button("Отсечь", lambda arg1=canvas_class, arg2=line_list, arg3=contour:
    solution_wrapper(arg1, arg2, arg3), [1000, 650])

create_button("Очистить экран", lambda arg1=canvas_class, arg2=line_list:
    clear(arg1, arg2), [1000, 690])

settings_bind(canvas_class, line_list, contour)

root.mainloop()
