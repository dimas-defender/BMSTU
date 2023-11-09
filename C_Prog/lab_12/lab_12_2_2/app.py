import ctypes
import tkinter as tk
import tkinter.messagebox as box

lib = ctypes.CDLL('C:/Users/dimas/Desktop/Cprogs/WIP/iu7-cprog-labs-2020-zhabindmitriy/lab_12_2_2/func.dll')

#void fill_fibon(int *a, int n)
#int get_unique_elems(int *src, int *dst, int src_len, int *dst_len)

_fill_fibon = lib.fill_fibon
_fill_fibon.argtypes = (ctypes.POINTER(ctypes.c_int), ctypes.c_int)
_fill_fibon.restype = None

def fill_fibon(n):
    arr = (ctypes.c_int * n)()
    _fill_fibon(arr, n)
    return list(arr)

_get_unique_elems = lib.get_unique_elems
_get_unique_elems.argtypes = (ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.POINTER(ctypes.c_int))
_get_unique_elems.restype = ctypes.c_int

def get_unique_elems1(nums):
    n = len(nums)
    arr = (ctypes.c_int * n)(*nums)
    n_res = ctypes.c_int(n)
    res = (ctypes.c_int * n_res.value)()
    rc = _get_unique_elems(arr, res, n, n_res)
    if rc:
        return list()
    else:
        return list(res)[0:n_res.value]

def get_unique_elems2(nums):
    n = len(nums)
    arr = (ctypes.c_int * n)(*nums)
    n_res = ctypes.c_int(0)
    rc = _get_unique_elems(arr, None, n, n_res)
    if rc:
        res = (ctypes.c_int * n_res.value)()
        rc = _get_unique_elems(arr, res, n, n_res)
        return list(res)
    else:
        return list()

button_color = 'Khaki'
window_color = 'Orange'
label_color = 'Khaki'

wd=tk.Tk()
wd.title('Числа Фибоначчи')
wd.geometry('500x500')
wd['bg'] = window_color

l1 = tk.Label(wd, text='Заполнить массив числами Фибоначчи', font="Arial 13", bg = label_color)
l1.place(anchor = tk.CENTER, relx = 0.4, rely = 0.2)
l2 = tk.Label(wd, text='Результат', font="Arial 13", bg = label_color)
l2.place(anchor = tk.CENTER, relx = 0.18, rely = 0.3)
l3 = tk.Label(wd, text='Выбрать уникальные элементы из массива', font="Arial 13", bg = label_color)
l3.place(anchor = tk.CENTER, relx = 0.43, rely = 0.5)
l4 = tk.Label(wd, text='Ввод', font="Arial 13", bg = label_color)
l4.place(anchor = tk.CENTER, relx = 0.13, rely = 0.6)
l5 = tk.Label(wd, text='Результат', font="Arial 13", bg = label_color)
l5.place(anchor = tk.CENTER, relx = 0.17, rely = 0.7)

e1 = tk.Entry(wd, justify = 'center', font="Arial 13", width = '6')
e1.place(anchor = tk.CENTER, relx = 0.83, rely = 0.2)
e2 = tk.Entry(wd, justify = 'center', font="Arial 13", width = '32')
e2.place(anchor = tk.CENTER, relx = 0.6, rely = 0.3)
e3 = tk.Entry(wd, justify = 'center', font="Arial 13", width = '32')
e3.place(anchor = tk.CENTER, relx = 0.6, rely = 0.6)
e4 = tk.Entry(wd, justify = 'center', font="Arial 13", width = '32')
e4.place(anchor = tk.CENTER, relx = 0.6, rely = 0.7)
        
def print_unique():
    e4.delete(0, 'end')
    try:
        a = str(e3.get())
    except:
        box.showerror('Ошибка ввода')
        e3.delete(0, 'end')
        return
    a=a.split()
    for i in range(len(a)):
        try:
            a[i] = int(a[i])
        except ValueError:
            box.showerror('Ошибка ввода', 'Необходимо ввести целые числа')
            e3.delete(0, 'end')
            return
    b = get_unique_elems2(a)
    for i in range(len(b)):
        b[i] = str(b[i])
    s = ' '.join(b)
    e4.insert(0, s)

def print_fibon():
    e2.delete(0, 'end')
    try:
        n = str(e1.get())
    except:
        box.showerror('Ошибка ввода')
        e1.delete(0, 'end')
        return
    try:
        n = int(n)
    except ValueError:
        box.showerror('Ошибка ввода', 'Необходимо ввести целое число')
        e1.delete(0, 'end')
        return
    if n <= 0:
        box.showerror('Ошибка ввода', 'Размер массива должен быть положительным')
        e1.delete(0, 'end')
        return
    b = fill_fibon(n)
    for i in range(len(b)):
        b[i] = str(b[i])
    s = ' '.join(b)
    e2.insert(0, s)

b1 = tk.Button(wd, text='Тест', width=5, command=print_fibon, font="Arial 13", bg = button_color)
b1.place(anchor = tk.CENTER, relx = 0.5, rely = 0.1)
b2 = tk.Button(wd, text='Тест', width=5, command=print_unique, font="Arial 13", bg = button_color)
b2.place(anchor = tk.CENTER, relx = 0.5, rely = 0.82)

wd.mainloop()
