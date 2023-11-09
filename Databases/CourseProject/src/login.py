from login_ui import Ui_MainWindow as LoginMainWindow
from errors import ErrWindow, connect_err, login_err_str
from admin import AdminWindow
from manager import ManagerWindow
from customer import CustomerWindow
from PyQt5 import QtWidgets, QtCore, QtGui
import psycopg2 as bd
from psycopg2 import Error
import sys
from hashlib import md5

class LoginWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super(LoginWindow, self).__init__()
        self.ui = LoginMainWindow()
        self.ui.setupUi(self)
        self.setFixedSize(self.width(), self.height())
        self.setWindowFlags(self.windowFlags() & ~QtCore.Qt.WindowMaximizeButtonHint)
        self.setWindowIcon(QtGui.QIcon("img/logo.png"))
        self.ui.entry_btn.clicked.connect(self.first_connect_to_db)
        self.ui.login_edit.returnPressed.connect(self.first_connect_to_db)
        self.ui.pswd_edit.returnPressed.connect(self.first_connect_to_db)
        self.child = None

    def first_connect_to_db(self):
        try:
            connection = bd.connect(user="postgres",
                                    password="3563",
                                    host="localhost",
                                    port="5432",
                                    database="techshop")
        except Error:
            return ErrWindow(connect_err).exec_()
        
        lgn = self.ui.login_edit.text()
        pswd = self.ui.pswd_edit.text()
        if len(pswd) > 0:
            pswd = md5(pswd.encode()).hexdigest()
        with connection as con:
            with con.cursor() as curs:
                curs.execute("select *"
                             "from get_connect_str(%s, %s);", (lgn, pswd))
                connect_str = curs.fetchone()
        connection.close()
        self.login_to_db(connect_str)

    def login_to_db(self, connect_str):
        if connect_str[1] == 'Admin':
            try:
                self.child = AdminWindow(self, connect_str)
                self.child.show()
                self.hide()
                self.ui.login_edit.clear()
                self.ui.pswd_edit.clear()
                self.ui.login_edit.setFocus()
            except Error:
                return ErrWindow(connect_err).exec_()
        elif connect_str[1] == 'Manager':
            try:
                self.child = ManagerWindow(self, connect_str)
                self.child.show()
                self.hide()
                self.ui.login_edit.clear()
                self.ui.pswd_edit.clear()
                self.ui.login_edit.setFocus()
            except Error:
                return ErrWindow(connect_err).exec_()
        elif connect_str[1] == 'Customer':
            try:
                self.child = CustomerWindow(self, connect_str)
                self.child.show()
                self.hide()
                self.ui.login_edit.setFocus()
            except Error:
                return ErrWindow(connect_err).exec_()
        else:
            ErrWindow(login_err_str).exec_()
            self.ui.login_edit.clear()
            self.ui.pswd_edit.clear()
            self.ui.login_edit.setFocus()


app = QtWidgets.QApplication([])
application = LoginWindow()
application.show()
sys.exit(app.exec())
