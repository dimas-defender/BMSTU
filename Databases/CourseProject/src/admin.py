from admin_ui import Ui_MainWindow as AdminMainWindow
from errors import ErrWindow, connect_err, add_err_str,\
    firstname_err, lastname_err, email_err, login_err, password_err,\
    post_err, wrongid_err, id_err, noid_err, notoedit_err
from PyQt5 import QtWidgets, QtCore, QtGui
import psycopg2 as bd
from psycopg2 import Error
from hashlib import md5

class AdminWindow(QtWidgets.QMainWindow):
    def __init__(self, parent_window: QtWidgets.QMainWindow, connect_str):
        super(AdminWindow, self).__init__()
        self.ui = AdminMainWindow()
        self.ui.setupUi(self)
        self.setFixedSize(self.width(), self.height())
        self.setWindowFlags(self.windowFlags() & ~QtCore.Qt.WindowMaximizeButtonHint)
        self.setWindowIcon(QtGui.QIcon("img/logo.png"))
        self.ui.goout.setIcon(QtGui.QIcon("img/logout.png"))

        self.ui.add_btn.clicked.connect(self.user_add)
        self.ui.show_btn.clicked.connect(self.show_users)
        self.ui.del_btn.clicked.connect(self.user_del)
        self.ui.ch_btn.clicked.connect(self.user_ch)
        self.ui.table.itemClicked.connect(self.item_clicked)
        self.ui.goout.clicked.connect(self.logout)
        self.ui.table.resizeColumnsToContents()

        self.parent = parent_window
        self.connection = None
        self.connection = bd.connect(user=connect_str[2],
                                     password=connect_str[3],
                                     host="localhost",
                                     port="5432",
                                     database="techshop")
        self.show_users()

    def __del__(self):
        if self.connection:
            self.connection.close()
            
    def exec_query(self, query, act = 'no_data'):
        with self.connection as con:
            with con.cursor() as curs:
                try:
                    curs.execute(query)
                    if act == 'fetchone':
                        data = curs.fetchone()
                    elif act == 'fetchall':
                        data = curs.fetchall()
                except Error:
                    return 'error'
        if act in ('fetchone', 'fetchall'):
            return data

    def show_table(self, f_creat_query, table: QtWidgets.QTableWidget, resize_cols: bool = True):
        query = f_creat_query()
        data = self.exec_query(query, 'fetchall')
        if data == 'error':
            return ErrWindow(connect_err).exec_()
        self.update_table(table, data, resize_cols)

    def show_users(self):
        self.show_table(self.create_query_show_users, self.ui.table)

    def item_clicked(self):
        row = self.ui.table.currentRow()
        user_id = int(self.ui.table.item(row, 0).text())
        self.ui.id.setText(str(user_id))

    def logout(self):
        self.__del__()
        self.parent.show()
        self.close()

    def user_add(self):
        firstname = self.ui.firstname.text().title()
        if not firstname:
            return ErrWindow(firstname_err).exec_()
        lastname = self.ui.lastname.text().title()
        if not lastname:
            return ErrWindow(lastname_err).exec_()
        email = self.ui.email.text()
        if not email:
            return ErrWindow(email_err).exec_()
        login = self.ui.login.text()
        if not login:
            return ErrWindow(login_err).exec_()
        password = self.ui.password.text()
        if not password:
            return ErrWindow(password_err).exec_()
        if self.ui.admin.isChecked():
            post = "Admin"
        elif self.ui.manager.isChecked():
            post = "Manager"
        else:
            return ErrWindow(post_err).exec_()

        password = md5(password.encode()).hexdigest()
        query = f"insert into users values (default, '{login}', '{password}'," \
                f"'{firstname}', '{lastname}', '{post}', '{email}');"
        with self.connection.cursor() as curs:
            try:
                curs.execute(query)
            except Error as e:
                if int(e.pgcode) == 23505:
                    return ErrWindow(add_err_str).exec_()
                return ErrWindow(connect_err).exec_()

        self.ui.firstname.clear()
        self.ui.lastname.clear()
        self.ui.email.clear()
        self.ui.login.clear()
        self.ui.password.clear()
        self.ui.admin.setAutoExclusive(False)
        self.ui.admin.setChecked(False)
        self.ui.admin.setAutoExclusive(True)
        self.ui.manager.setAutoExclusive(False)
        self.ui.manager.setChecked(False)
        self.ui.manager.setAutoExclusive(True)
        self.show_users()
        QtWidgets.QMessageBox.information(self, "Добавление пользователя", "Новый пользователь успешно добавлен!")

    def create_query_show_users(self):
        query = "select * from users where "
        firstname = self.ui.shortname_show.text()
        if firstname:
            query += f"position(lower('{firstname}') in lower(firstname)) > 0 and "
        lastname = self.ui.fullname_show.text()
        if lastname:
            query += f"position(lower('{lastname}') in lower(lastname)) > 0 and "
        email = self.ui.email_show.text()
        if email:
            query += f"position(lower('{email}') in lower(email)) > 0 and "
        login = self.ui.login_show.text()
        if login:
            query += f"position(lower('{login}') in lower(login)) > 0 and "
        password = self.ui.password_show.text()
        if password:
            query += f"position(lower('{password}') in lower(password)) > 0 and "
        query += "true and ("
        if self.ui.manager_show.isChecked():
            query += f"post = 'Manager' or "
        if self.ui.admin_show.isChecked():
            query += f"post = 'Admin' or "
        if self.ui.null_post.isChecked():
            query += f"post is null or "
        if query[-4:] == " or ":
            query = query[:-4]+")"
        else:
            query = query[:-6]
        query += " order by id"
        return query

    def user_del(self):
        del_id = self.ui.id.text()
        if not del_id:
            return ErrWindow(id_err).exec_()
        try:
            del_id = int(del_id)
        except ValueError:
            return ErrWindow(wrongid_err).exec_()

        query = "select id from users"
        data = self.exec_query(query, 'fetchall')
        if data == 'error':
            return ErrWindow(connect_err).exec_()

        ids = [x[0] for x in data]
        if del_id not in ids:
            return ErrWindow(noid_err).exec_()

        query = f"delete from users where id={del_id}"
        data = self.exec_query(query)
        if data == 'error':
            return ErrWindow(connect_err).exec_()
        
        self.ui.id.clear()
        self.show_users()
        QtWidgets.QMessageBox.information(self, "Удаление пользователя", "Пользователь успешно удален!")

    def user_ch(self):
        if len(self.ui.table.selectedItems()) < 1:
            return ErrWindow(notoedit_err).exec_()
        row = self.ui.table.currentRow()
        user_id = int(self.ui.table.item(row, 0).text())
        query = "update users set "
        firstname = self.ui.shortname_ch.text().title()
        if firstname:
            query += f"firstname='{firstname}', "
        lastname = self.ui.fullname_ch.text().title()
        if lastname:
            query += f"lastname='{lastname}', "
        email = self.ui.email_ch.text()
        if email:
            query += f"email='{email}', "
        login = self.ui.login_ch.text()
        if login:
            query += f"login='{login}', "
        password = self.ui.password_ch.text()
        if password:
            password = md5(password.encode()).hexdigest()
            query += f"password='{password}', "
        if self.ui.admin_ch.isChecked():
            query += "post='Admin', "
        elif self.ui.manager_ch.isChecked():
            query += "post='Manager', "
        else:
            query += f"post='{self.ui.table.item(row, 5).text()}', "
        query = query[:-2] + f"where id={user_id}"
        with self.connection.cursor() as curs:
            try:
                curs.execute(query)
            except Error as e:
                if int(e.pgcode) == 23505:
                    return ErrWindow(add_err_str).exec_()
                return ErrWindow(connect_err).exec_()

        self.ui.shortname_ch.clear()
        self.ui.fullname_ch.clear()
        self.ui.email_ch.clear()
        self.ui.login_ch.clear()
        self.ui.password_ch.clear()
        self.ui.admin_ch.setAutoExclusive(False)
        self.ui.admin_ch.setChecked(False)
        self.ui.admin_ch.setAutoExclusive(True)
        self.ui.manager_ch.setAutoExclusive(False)
        self.ui.manager_ch.setChecked(False)
        self.ui.manager_ch.setAutoExclusive(True)
        self.show_users()
        self.ui.table.clearSelection()
        QtWidgets.QMessageBox.information(self, "Редактирование пользователя", "Данные пользователя успешно изменены!")

    @staticmethod
    def update_table(table: QtWidgets.QTableWidget, data, resize_cols: bool = True) -> None:
        table.setRowCount(len(data))
        i = 0
        for record in data:
            j = 0
            for field in record:
                item = QtWidgets.QTableWidgetItem(str(field))
                item.setFlags(QtCore.Qt.ItemIsSelectable | QtCore.Qt.ItemIsEnabled)
                item.setTextAlignment(QtCore.Qt.AlignCenter)
                table.setItem(i, j, item)
                j += 1
            i += 1
        if resize_cols:
            table.resizeColumnsToContents()
        table.clearSelection()