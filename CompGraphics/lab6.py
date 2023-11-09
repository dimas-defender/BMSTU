#Алгоритм затравочного заполнения
#ИУ7-44Б Жабин Дмитрий

from tkinter import *
from tkinter import messagebox as mb
from time import time, sleep
from math import fabs

WIDTH, HEIGHT = 800, 800
FALSE = "-"

COLOR_BG = ((255, 255, 255), 'white')
COLOR_FILL_INIT = ((0.0, 0.0, 0.0), "#000000")
COLOR_LINE = ((0, 0, 0), "#000000")

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


def create_list_box(root, coords, title):
    list_box = Listbox(root, width = 24, height=17)
    list_box.insert(END, title)
    list_box.place(x = coords[0], y = coords[1])
    scroll = Scrollbar(command = list_box.yview)
    scroll.place(x = coords[0] + 150, y = coords[1], height = 275)
    return list_box


def clear(canvas_class, list_box, points_list):
    canvas_class.clear_all()

    list_box.delete(1, list_box.size())

    for i in range(len(points_list) - 1, -1, -1):
        del points_list[i]

    points_list.append([])


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


class stack_class():

    stack = list()

    def __init__(self, elem):
        self.push(elem)

    def pop(self):
        return self.stack.pop()

    def push(self, elem):
        self.stack.append(elem)

    def is_empty(self):
        return not len(self.stack)


def find_pixel(stack, canvas_class, x_right, x, y):
    # Ищем новый затравочный пиксель.
    while x <= x_right:
        flag = False
        while canvas_class.compare_color_line(x, y) and canvas_class.compare_color_fill(x, y) and x <= x_right:
            if flag == False:
                flag = True
            x += 1

        # Если нашли новый пиксель, то помещаем его в стек.
        if flag:
            if x == x_right and canvas_class.compare_color_line(x, y) and \
                    canvas_class.compare_color_fill(x, y):
                stack.push([x, y])
            else:
                stack.push([x - 1, y])
            flag = False

        # Продолжаем проверку (если интервал был прерван)
        x_temp = x
        while not canvas_class.compare_color_line(x, y) or \
                not canvas_class.compare_color_fill(x, y) and x < x_right:
            x += 1
            if x >= WIDTH:
                  break

        if x == x_temp:
            x += 1


def fill_right(canvas_class, x, y):
    while canvas_class.img.get(round(x), round(y)) != canvas_class.color_line[0]:
        canvas_class.draw_pixel((x, y))
        x += 1
    return x - 1


def fill_left(canvas_class, x, y):
    while canvas_class.img.get(round(x), round(y)) != canvas_class.color_line[0]:
        canvas_class.draw_pixel((x, y))
        x -= 1
    return x + 1


def fill(canvas_class, start_point, flag = 0):
    
    stack = stack_class(start_point)
    while not stack.is_empty():
        
        x, y = stack.pop()
        canvas_class.draw_pixel((x, y))
        x_right = fill_right(canvas_class, x + 1, y) #красим все пиксели справа от затр.точки до границы
        x_left = fill_left(canvas_class, x - 1, y) #красим все пиксели слева от затр.точки до границы
        if flag:
            sleep(0.01)
            canvas_class.canvas.update()
        # На строке выше в диапазоне [x_left; x_right] ищем новые затравочные пиксели и помещаем их в стек
        find_pixel(stack, canvas_class, x_right, x_left, y + 1)
        # На строке ниже в диапазоне [x_left; x_right] ищем новые затравочные пиксели и помещаем их в стек
        find_pixel(stack, canvas_class, x_right, x_left, y - 1)


def fill_wrapper(canvas_class, start_point, flag = False):
    start_point = get_coords(start_point.get())
    if start_point[0] == FALSE:
        return

    canvas_class.choose_color_fill()
    if not flag:
        time_start = time()
        fill(canvas_class, start_point)
        print_info(f"Время заполнения: {round(time() - time_start, 4)} ")
    else:
        fill(canvas_class, start_point, 1)


def sign(a):
    if a == 0:
        return 0
    return a / abs(a)


def bresenham_int(canvas_class, start, stop):
    dx = stop[0] - start[0]
    dy = stop[1] - start[1]
    x, y = start[0], start[1]
    sx, sy = sign(dx), sign(dy)
    dx = fabs(dx)
    dy = fabs(dy)

    swap = 0

    if dy > dx:
        swap = 1
        dx, dy = dy, dx

    e = 2 * dy - dx

    for _ in range(int(dx + 1)):
        canvas_class.print_pixel(int(x), int(y))
        if e >= 0:
            if swap == 0:
                y += sy
            else:
                x += sx
            e -= (2 * dx)

        if e < 0:
            if swap == 0:
                x += sx
            else:
                y += sy
            e += (2 * dy)


def lock(points_list, canvas_class, list_box):
    if len(points_list[-1]) > 1:
        start = points_list[-1][-1]
        stop = points_list[-1][0]

        bresenham_int(canvas_class, start, stop)

        points_list.append(list())
        list_box.insert(END, "_" * 8)


def add_point(points_list, point, canvas_class, list_box):
    if point in points_list[-1]:
        print_error("Такая точка уже имеется в данном многоугольнике!")
        return

    points_list[-1].append(point)
    list_box.insert(END, point)

    if len(points_list[-1]) > 1:
        start = points_list[-1][-2]
        stop = points_list[-1][-1]
        bresenham_int(canvas_class, start, stop)
        
    elif len(points_list[-1]) == 1:
        canvas_class.print_pixel(points_list[-1][0][0], points_list[-1][0][1])


def add_point_click(event, canvas_class, list_box, points_list):
    add_point(points_list, [event.x, event.y], canvas_class, list_box)


def add_point_entry(point_coords, list_box, canvas_class, points_list):
    point = list(get_coords(point_coords.get()))
    if point[0] == FALSE:
        return
    add_point(points_list, point, canvas_class, list_box)


def convert_color(color):
    if color == 'red':
        return (255, 0, 0)
    if color == 'blue':
        return (0, 0, 255)
    if color == 'green':
        return (0, 255, 0)
    if color == 'black':
        return (0, 0, 0)
    #if color == 'white':
        #return (255, 255, 255)

def rgb2hex(color):
        r, g, b = color
        return f'#{r:02x}{g:02x}{b:02x}'
    

class paint_class():
    color_fill = COLOR_FILL_INIT
    color_line = COLOR_LINE
    canvas = None
    img = None

    def __init__(self, root):
        self.canvas = Canvas(root, width=WIDTH, height=HEIGHT, bg="white")
        self.canvas.place(x=0, y=0)

        self.img = PhotoImage(width=WIDTH, height=HEIGHT)
        self.canvas.create_image((WIDTH/2, HEIGHT/2), image=self.img, state="normal")
        self.img.put(COLOR_BG[1], to=(0, 0, WIDTH, HEIGHT))
        bresenham_int(self, (0,0), (WIDTH - 1,0))
        bresenham_int(self, (0,0), (0,WIDTH - 1))
        bresenham_int(self, (WIDTH - 1,0), (WIDTH - 1,WIDTH - 1))
        bresenham_int(self, (0,WIDTH - 1), (WIDTH - 1,WIDTH - 1))

    def clear_all(self):
        self.canvas.delete(ALL)

        self.img = PhotoImage(width=WIDTH, height=HEIGHT)
        self.canvas.create_image((WIDTH/2, HEIGHT/2), image=self.img, state="normal")
        self.img.put(COLOR_BG[1], to=(0, 0, WIDTH, HEIGHT))
        bresenham_int(self, (0,0), (WIDTH - 1,0))
        bresenham_int(self, (0,0), (0,WIDTH - 1))
        bresenham_int(self, (WIDTH - 1,0), (WIDTH - 1,WIDTH - 1))
        bresenham_int(self, (0,WIDTH - 1), (WIDTH - 1,WIDTH - 1))

    def draw_pixel(self, coords):
        self.img.put(self.color_fill[1], (round(coords[0]), round(coords[1])))

    def print_pixel(self, x, y):
        self.img.put(self.color_line[1], (round(x), round(y)))

    def choose_color_fill(self):
        try:
            j = color_list.curselection()[0]
        except:
            mb.showerror('Ошибка', 'Необходимо выбрать цвет')
            return 1

        if j == 0:
            color = 'red'
        if j == 1:
            color = 'blue'
        if j == 2:
            color = 'green'
        if j == 3:
            color = 'black'
        #if j == 4:
            #color = 'white'
        
        color = convert_color(color)
        color = (color, rgb2hex(color))
        self.color_fill = color
        #self.canvas.itemconfig("user_line", fill = self.color_line[1])

    def in_canvas(self, event):
        self.canvas.delete("coords")
        self.canvas.create_text(event.x + 45, event.y - 15,  text=f"({event.x}, {event.y})", font="Arial 10", tags="coords")

    def compare_color_line(self, x, y):
        return self.img.get(round(x), round(y)) != self.color_line[0]

    def compare_color_fill(self, x, y):
        return self.img.get(round(x), round(y)) != self.color_fill[0]


root = Tk()
points_list = [[]]
setup_ui(root, "1200x800", "Алгоритм затравочного заполнения")

canvas_class = paint_class(root)
create_label(root, "Вершины", [1000, 340], 145)
list_box = create_list_box(root, [920, 365], "(X,Y)")

create_label(root, "Цвет", [1130, 40], 90)
color_list = Listbox(root, width = '15', height = '4')
color_list.insert(END, 'Красный')
color_list.insert(END, 'Синий')
color_list.insert(END, 'Зеленый')
color_list.insert(END, 'Черный')
#color_list.insert(END, 'Белый')
color_list.place(anchor = CENTER, relx = 0.94, rely = 0.13)

create_label(root, "Координаты точки:", [940, 40], 200)
entry_coords = create_entry(root, [940, 75])

create_button("Добавить точку", lambda arg1=entry_coords, arg2=list_box, arg3=canvas_class,
        arg4=points_list: add_point_entry(arg1, arg2, arg3, arg4), [1000, 165])

create_button("Замкнуть", lambda arg1=points_list,
        arg2=canvas_class, arg3=list_box: lock(arg1, arg2, arg3), [1000, 200])

create_label(root, "Координаты \nзатравочной точки:", [1000, 260])
entry_start_point = create_entry(root, [1000, 300])

create_button("Заполнить", lambda arg1=canvas_class, arg2=entry_start_point: fill_wrapper(arg1, arg2), [1000, 675])
create_button("Заполнить с задержкой", lambda arg1=canvas_class, arg2=entry_start_point: fill_wrapper(arg1, arg2, 1), [1000, 715])
create_button("Очистить экран", lambda arg1=canvas_class, arg2=list_box, arg3=points_list: clear(arg1, arg2, arg3), [1000, 755])
canvas_class.canvas.bind("<Button-1>", lambda event, arg1=canvas_class, arg2=list_box, arg3=points_list: add_point_click(event, arg1, arg2, arg3))
canvas_class.canvas.bind("<Motion>", lambda event: canvas_class.in_canvas(event))

root.mainloop()
