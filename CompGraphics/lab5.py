#Алгоритм заполнения с перегородкой
#ИУ7-44Б Жабин Дмитрий

from tkinter import *
from tkinter import messagebox as mb
from time import time, sleep

WIDTH, HEIGHT = 800, 800
FALSE = "-"

color_bg = ((255, 255, 255), 'white')
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
    entry = Entry(root, width= "50", font = "Arial 13")
    entry.place(x = coords[0], y = coords[1], anchor="center", width = 150, height = 25)
    return entry


def create_list_box(root, coords, title):
    list_box = Listbox(root, width = 24, height=17)
    list_box.insert(END, title)
    list_box.place(x = coords[0], y = coords[1])
    scroll = Scrollbar(command = list_box.yview)
    scroll.place(x = coords[0] + 150, y = coords[1], height = 275)
    return list_box


def find_intersections(canvas_class, start, stop, lst):
    if stop[1] == start[1]:
        return

    if start[1] > stop[1]:
        start, stop = stop, start

    dy = 1
    dx = (stop[0] - start[0]) / (stop[1] - start[1])
    x = start[0]
    y = start[1]

    while y < stop[1]:
        lst.append([int(x), int(y)])
        y += dy
        x += dx


def fill_wrapper(canvas_class, lst):
    time_start = time()

    if fill(canvas_class, lst) == 1:
        return 1

    time_stop = time()
    print_info("Время заполнения " + str(round(time_stop - time_start, 4)))


def fill(canvas_class, lst):
    # Проводим перегородку через первую вершину многоугольника (расположенную выше)
    if canvas_class.choose_color_line() == 1:
        return 1
    septum = lst[0][0]
    for i in range(len(lst)):
        if lst[i][0] < septum:
            for j in range(lst[i][0] + 1, septum + 1):
                canvas_class.reverse_pixel(j, lst[i][1])
        elif lst[i][0] > septum:
            for j in range(lst[i][0], septum, -1):
                canvas_class.reverse_pixel(j, lst[i][1])

def delayed_fill(canvas_class, lst):
    if canvas_class.choose_color_line() == 1:
        return 1
    septum = lst[0][0]
    for i in range(len(lst)):
        if lst[i][0] < septum:
            for j in range(lst[i][0] + 1, septum + 1):
                canvas_class.reverse_pixel(j, lst[i][1])
        elif lst[i][0] > septum:
            for j in range(lst[i][0], septum, -1):
                canvas_class.reverse_pixel(j, lst[i][1])
        sleep(0.01)
        canvas_class.canvas.update()
                

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

    color_line = ((0.0, 0.0, 0.0), '#000000')
    canvas = None
    img = None

    def __init__(self, root):
        self.canvas = Canvas(root, width=WIDTH, height=HEIGHT)
        self.canvas.place(x=0, y=0)
        self.img = PhotoImage(width=WIDTH, height=HEIGHT)
        self.canvas.create_image((WIDTH/2, HEIGHT/2), image=self.img, state="normal")
        self.img.put(color_bg[1], to=(0, 0, WIDTH, HEIGHT))

    def clear_all(self):
        self.canvas.delete(ALL)
        self.img = PhotoImage(width=WIDTH, height=HEIGHT)
        self.canvas.create_image((WIDTH/2, HEIGHT/2), image=self.img, state="normal")
        self.img.put(color_bg[1], to=(0, 0, WIDTH, HEIGHT))

    def reverse_pixel(self, x, y):
        color_pixel = self.img.get(round(x), round(y))
        if color_pixel == color_bg[0]:
            self.img.put(self.color_line[1], (round(x), round(y)))
        elif color_pixel == self.color_line[0]:
            self.img.put(color_bg[1], (round(x), round(y)))

    def print_line(self, start, stop):
        self.canvas.create_line(start[0], start[1], stop[0], stop[1], fill=self.color_line[1], tags="user_line")

    def print_pixel(self, x, y):
        self.canvas.create_line(round(x), round(y), round(x), round(y) + 1, width=1, fill=self.color_line[1])

    def choose_color_line(self):
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
        self.color_line = color
        self.canvas.itemconfig("user_line", fill = self.color_line[1])


def lock(points_list, canvas_class, list_box, lst):
    if len(points_list[-1]) > 1:
        start = points_list[-1][-1]
        stop = points_list[-1][0]

        find_intersections(canvas_class, start, stop, lst)
        canvas_class.print_line(start, stop)

        points_list.append(list())
        list_box.insert(END, "_" * 8)


def add_point(points_list, point, canvas_class, list_box, lst):
    if point in points_list[-1]:
        print_error("Эта точка уже имеется в данном многоугольнике!")
        return
    
    points_list[-1].append(point)
    list_box.insert(END, point)

    if len(points_list[-1]) > 1:
        start = points_list[-1][-2]
        stop = points_list[-1][-1]

        find_intersections(canvas_class, stop, start, lst)
        canvas_class.print_line(start, stop)

    elif len(points_list[-1]) == 1:
        canvas_class.print_pixel(points_list[-1][0][0], points_list[-1][0][1])


def add_point_click(event, canvas_class, list_box, points_list, lst):
    add_point(points_list, [event.x, event.y], canvas_class, list_box, lst)


def add_point_entry(entry_coords, list_box, canvas_class, points_list, lst):
    point = list(get_coords(entry_coords.get()))
    if point[0] == FALSE:
        return
    add_point(points_list, point, canvas_class, list_box, lst)


def clear(canvas_class, list_box, points_list, lst):

    canvas_class.clear_all()
    list_box.delete(1, list_box.size())

    for i in range(len(points_list) - 1, -1, -1):
        del points_list[i]

    for i in range(len(lst) - 1, -1, -1):
        del lst[i]

    points_list.append([])


root = Tk()
lst = list() #массив точек - пересечений со сканирующей строкой
points_list = [[]] #массив вершин

setup_ui(root, "1200x800", "Растровое заполнение областей")

canvas_class = paint_class(root)
create_label(root, "Вершины", [925, 295], 145)
list_box = create_list_box(root, [850, 325], "(X,Y)")

create_label(root, "Цвет", [1130, 40], 90)
color_list = Listbox(root, width = '15', height = '4')
color_list.insert(END, 'Красный')
color_list.insert(END, 'Синий')
color_list.insert(END, 'Зеленый')
color_list.insert(END, 'Черный')
#color_list.insert(END, 'Белый')
color_list.place(anchor = CENTER, relx = 0.94, rely = 0.13)

create_label(root, "Координаты точки (0 - 800):", [940, 40])
entry_coords = create_entry(root, [940, 80])

create_button("Добавить точку", lambda arg1=entry_coords, arg2=list_box, arg3=canvas_class,
    arg4=points_list, arg5=lst: add_point_entry(arg1, arg2, arg3, arg4, arg5), [1000, 180])

create_button("Замкнуть", lambda arg1=points_list,
        arg2=canvas_class, arg3=list_box, arg4=lst: lock(arg1, arg2, arg3, arg4), [1000, 220])

create_button("Заполнить", lambda arg1=canvas_class, arg2=lst: fill_wrapper(arg1, arg2), [1000, 650])

create_button("Заполнить с задержкой", lambda arg1=canvas_class, arg2=lst: delayed_fill(arg1, arg2), [1000, 700])

create_button("Очистить экран", lambda arg1=canvas_class, arg2=list_box, arg3=points_list, arg4=lst:
    clear(arg1, arg2, arg3, arg4), [1000, 750])

canvas_class.canvas.bind('<Button-1>', lambda event,
    arg1=canvas_class, arg2=list_box, arg3=points_list, arg4=lst: add_point_click(event, arg1, arg2, arg3, arg4))

root.mainloop()
