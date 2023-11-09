from err_ui import Ui_Dialog as ErrDialog
from PyQt5 import QtWidgets, QtCore, QtGui

class ErrWindow(QtWidgets.QDialog):
    def __init__(self, text):
        super(ErrWindow, self).__init__()
        self.ui = ErrDialog()
        self.ui.setupUi(self)
        self.setWindowFlags(self.windowFlags() & ~QtCore.Qt.WindowContextHelpButtonHint)
        self.setFixedSize(self.width(), self.height())
        self.setWindowIcon(QtGui.QIcon("img/error.png"))
        self.ui.err_msg.setText(text)
        self.ui.ok_btn.clicked.connect(self.kill)

    def kill(self):
        self.close()


connect_err = "Невозможно установить соединение! Попробуйте еще раз позднее!"
add_err_str = "Такие логин и пароль уже есть!"
id_err = "Введите идентификатор!"
wrongid_err = "Введен недопустимый идентификатор!"
noid_err = "Пользователь с указанным идентификатором не существует!"
notoedit_err = "Выберите строку для редактирования!"
login_err_str = "Неверный логин и\\или пароль!\nПопробуйте еще раз!"

firstname_err = "Укажите имя пользователя!"
lastname_err = "Укажите фамилию пользователя!"
email_err = "Укажите электронную почту пользователя!"
login_err = "Укажите логин пользователя!"
password_err = "Укажите пароль пользователя!"
post_err = "Укажите должность пользователя!"

name_err = "Укажите название товара!"
samename_err = "Товар с таким названием уже представлен в магазине!"
type_err = "Укажите категорию товара!"
country_err = "Укажите страну производства товара!"
manufacturer_err = "Укажите производителя товара!"
price_err = "Цена товара не может быть нулевой!"
supplier_err = "Укажите поставщика товара!"
address_err = "Укажите адрес поставщика товара!"
phone_err = "Укажите номер телефона поставщика товара!"

zero_qnt_err = "Количество единиц товара должно быть ненулевым!"
qnt_err = "Данного товара нет на складе в таком количестве!"
cname_err = "Укажите имя!"
cphone_err = "Укажите контактный телефон!"
caddress_err = "Укажите адрес доставки!"
cart_err = "Ваша корзина пуста!"
nogoods_err = "Товар с указанным идентификатором не существует!"
del_goods_err = "Данного товара нет в корзине!"