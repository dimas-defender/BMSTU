from customer_ui import Ui_MainWindow as CustomerMainWindow
from errors import ErrWindow, connect_err, qnt_err, cname_err, cphone_err,\
    caddress_err, cart_err, nogoods_err, zero_qnt_err, del_goods_err
from PyQt5 import QtWidgets, QtCore, QtGui
import psycopg2 as bd
from psycopg2 import Error
from datetime import date


class CustomerWindow(QtWidgets.QMainWindow):
    def __init__(self, parent_window: QtWidgets.QMainWindow, connect_str):
        super(CustomerWindow, self).__init__()
        self.ui = CustomerMainWindow()
        self.ui.setupUi(self)
        self.setFixedSize(self.width(), self.height())
        self.setWindowFlags(self.windowFlags() & ~QtCore.Qt.WindowMaximizeButtonHint)
        self.setWindowIcon(QtGui.QIcon("img/logo.png"))
        self.ui.goout.setIcon(QtGui.QIcon("img/logout.png"))

        self.ui.catalog_show_btn.clicked.connect(self.show_catalog)
        self.ui.catalog_add_to_cart_btn.clicked.connect(self.add_to_cart)
        self.ui.make_order_btn.clicked.connect(self.make_order)
        self.ui.clear_cart_btn.clicked.connect(self.clear_cart)
        self.ui.catalog_table.itemClicked.connect(self.item_clicked)
        self.ui.cart_table.itemClicked.connect(self.cart_item_clicked)
        self.ui.remove_goods.clicked.connect(self.remove_from_cart)
        self.ui.goout.clicked.connect(self.logout)

        self.parent = parent_window
        self.child = None
        self.connect_str = connect_str
        self.connection = bd.connect(user=connect_str[2],
                                     password=connect_str[3],
                                     host="localhost",
                                     port="5432",
                                     database="techshop")
        self.cart = []
        self.show_catalog()
        self.show_cart()

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

    def item_clicked(self):
        row = self.ui.catalog_table.currentRow()
        goods_id = int(self.ui.catalog_table.item(row, 0).text())
        self.ui.catalog_goods_id.setValue(goods_id)
        self.ui.catalog_cnt.setValue(1)

    def cart_item_clicked(self):
        row = self.ui.cart_table.currentRow()
        goods_id = int(self.ui.cart_table.item(row, 0).text())
        self.ui.del_goods_id.setValue(goods_id)

    def logout(self):
        self.__del__()
        self.parent.show()
        self.close()

    def show_catalog(self):
        self.show_table(self.create_query_show_catalog, self.ui.catalog_table)

    def show_cart(self):
        total = 0
        for x in self.cart:
            total += x[4]
        self.ui.cart_total.setText(str(total))
        self.update_table(self.ui.cart_table, self.cart)

    def add_to_cart(self):
        goods_id = int(self.ui.catalog_goods_id.text())

        query = "select id from goods"
        data = self.exec_query(query, 'fetchall')
        if data == 'error':
            return ErrWindow(connect_err).exec_()

        goods_ids = [x[0] for x in data]
        if goods_id not in goods_ids:
            return ErrWindow(nogoods_err).exec_()

        query = f"select name, price, quantity from goods where id={goods_id}"
        data = self.exec_query(query, 'fetchone')
        if data == 'error':
            return ErrWindow(connect_err).exec_()

        qnt = int(self.ui.catalog_cnt.text())
        if qnt == 0:
            return ErrWindow(zero_qnt_err).exec_()
        qnt_incart = 0
        ids = [x[0] for x in self.cart]
        flag = False
        if goods_id in ids:
            flag = True
            cart_id = ids.index(goods_id)
            qnt_incart += self.cart[cart_id][3]

        in_stock = int(data[2])
        if qnt_incart + qnt > in_stock:
            return ErrWindow(qnt_err).exec_()

        price = int(data[1])
        sum = qnt * price

        if flag:
            self.cart[cart_id][3] += qnt
            self.cart[cart_id][4] += sum
        else:
            name = data[0]
            goods = [goods_id, name, price, qnt, sum]
            self.cart.append(goods)
        self.show_cart()

    def remove_from_cart(self):
        goods_id = int(self.ui.del_goods_id.text())
        goods_ids = [x[0] for x in self.cart]
        if goods_id not in goods_ids:
            return ErrWindow(del_goods_err).exec_()
        
        ind = goods_ids.index(goods_id)
        self.cart.pop(ind)
        self.show_cart()

    def make_order(self):
        if len(self.cart) == 0:
            return ErrWindow(cart_err).exec_()
        name = self.ui.cart_name.text()
        if not name:
            return ErrWindow(cname_err).exec_()
        phone = self.ui.cart_phone.text()
        if not phone:
            return ErrWindow(cphone_err).exec_()
        address = self.ui.cart_address.text()
        if not address:
            return ErrWindow(caddress_err).exec_()
        
        query = "select phone from client"
        data = self.exec_query(query, 'fetchall')
        if data == 'error':
            return ErrWindow(connect_err).exec_()

        phones = [x[0] for x in data]
        sum = int(self.ui.cart_total.text())
        if phone in phones:
            query = f"update client set name='{name}', total=total+'{sum}' where phone='{phone}'"
            data = self.exec_query(query)
            if data == 'error':
                return ErrWindow(connect_err).exec_()
        else:
            query = f"insert into client values (default, '{name}', '{phone}', '{sum}')"
            data = self.exec_query(query)
            if data == 'error':
                return ErrWindow(connect_err).exec_()

        query = f"select id from client where phone='{phone}'"
        data =self.exec_query(query, 'fetchone')
        if data == 'error':
            return ErrWindow(connect_err).exec_()
        client_id = int(data[0])

        query = f"insert into sale values (default, '{client_id}', '{sum}', '{address}', '{str(date.today())}')"
        data = self.exec_query(query)
        if data == 'error':
            return ErrWindow(connect_err).exec_()

        query = "select max(id) from sale"
        data = self.exec_query(query, 'fetchone')
        if data == 'error':
            return ErrWindow(connect_err).exec_()
        sale_id = int(data[0])

        for x in self.cart:
            query = f"insert into goods_to_sale values (default, '{x[0]}', '{x[3]}', '{sale_id}')"
            data = self.exec_query(query)
            if data == 'error':
                return ErrWindow(connect_err).exec_()

        self.show_catalog()
        self.clear_cart()
        QtWidgets.QMessageBox.information(self, "Корзина", "Ваш заказ успешно оформлен!")

    def clear_cart(self):
        self.cart.clear()
        self.show_cart()

    def create_query_show_catalog(self):
        query = "select g.id, g.name, g.type, g.manufacturer, g.country,\
            g.price, g.warranty, g.quantity from goods g where "
        name = self.ui.catalog_name.text()
        if name:
            query += f"position(lower('{name}') in lower(g.name)) > 0 and "
        type = self.ui.catalog_type.text()
        if type:
            query += f"position(lower('{type}') in lower(g.type)) > 0 and "
        manufacturer = self.ui.catalog_manufacturer.text()
        if manufacturer:
            query += f"position(lower('{manufacturer}') in lower(g.manufacturer)) > 0 and "
        country = self.ui.catalog_country.text()
        if country:
            query += f"position(lower('{country}') in lower(g.country)) > 0 and "
        price = self.ui.catalog_price.text()
        if int(price) > 0:
            query += f"g.price >= {price} and "
        warranty = self.ui.catalog_warranty.text()
        if int(warranty) > 0:
            query += f"g.warranty >= {warranty} and "
        quantity = self.ui.catalog_quantity.text()
        if int(quantity) > 0:
            query += f"g.quantity >= {quantity} and "
        query += "true order by g.id"
        return query

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