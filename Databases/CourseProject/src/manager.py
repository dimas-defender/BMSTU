from manager_ui import Ui_MainWindow as ManagerMainWindow
from errors import ErrWindow, connect_err, name_err, type_err, country_err,\
    manufacturer_err, price_err, supplier_err, address_err, phone_err, samename_err
from PyQt5 import QtWidgets, QtCore, QtGui
import psycopg2 as bd
from psycopg2 import Error


class ManagerWindow(QtWidgets.QMainWindow):
    def __init__(self, parent_window: QtWidgets.QMainWindow, connect_str):
        super(ManagerWindow, self).__init__()
        self.ui = ManagerMainWindow()
        self.ui.setupUi(self)
        self.setFixedSize(self.width(), self.height())
        self.setWindowFlags(self.windowFlags() & ~QtCore.Qt.WindowMaximizeButtonHint)
        self.setWindowIcon(QtGui.QIcon("img/logo.png"))
        self.ui.goout.setIcon(QtGui.QIcon("img/logout.png"))

        self.ui.catalog_show_btn.clicked.connect(self.show_catalog)
        self.ui.catalog_add_btn.clicked.connect(self.add_catalog)
        self.ui.show_btn.clicked.connect(self.show_sales_history)
        self.ui.clients_show_btn.clicked.connect(self.show_clients)
        self.ui.suppliers_show_btn.clicked.connect(self.show_suppliers)
        self.ui.goout.clicked.connect(self.logout)

        self.parent = parent_window
        self.child = None
        self.connect_str = connect_str
        self.connection = bd.connect(user=connect_str[2],
                                     password=connect_str[3],
                                     host="localhost",
                                     port="5432",
                                     database="techshop")
        self.userid = int(connect_str[0])
        self.show_catalog()
        self.show_sales_history()
        self.show_clients()
        self.show_suppliers()

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

    def show_catalog(self):
        self.show_table(self.create_query_show_catalog, self.ui.catalog_table)

    def show_sales_history(self):
        self.show_table(self.create_query_show_sales_history, self.ui.sales_table)

    def show_clients(self):
        self.show_table(self.create_query_show_clients, self.ui.clients_table)

    def show_suppliers(self):
        self.show_table(self.create_query_show_suppliers, self.ui.supplier_table)

    def logout(self):
        self.__del__()
        self.parent.show()
        self.close()

    def create_query_show_catalog(self):
        query = "select g.id, g.name, g.type, g.manufacturer, g.country,\
            g.price, g.warranty, g.quantity, s.name from goods g join supplier s\
             on g.supplier_id = s.id where "
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
        supplier = self.ui.catalog_supplier.text()
        if supplier:
            query += f"position(lower('{supplier}') in lower(s.name)) > 0 and "
        query += "true order by g.id"
        return query

    def create_query_show_sales_history(self):
        query = "select s.id, c.phone, s.price, s.address, s.date, g.name, gts.quantity  \
            from sale s join goods_to_sale gts on gts.sale_id = s.id join goods g on gts.goods_id = g.id join client c on s.client_id = c.id\
            where "
        minsum = self.ui.minsum_spinBox.text()
        if int(minsum) > 0:
            query += f"s.price >= {minsum} and "        
        datefrom = self.ui.datefromEdit.text()
        dateto = self.ui.datetoEdit.text()
        query += f"s.date between '{datefrom}' and '{dateto}' order by s.id"
        return query

    def create_query_show_clients(self):
        query = "select * from client"
        mintotal = self.ui.mintotal_spinBox.text()
        if int(mintotal) > 0:
            query += f" where total >= {mintotal}"
        return query

    def create_query_show_suppliers(self):
        query = "select * from supplier"
        name = self.ui.supplier_name.text()
        if name:
            query += f" where position(lower('{name}') in lower(name)) > 0"
        return query

    def add_catalog(self):
        name = self.ui.catalog_name.text()
        if not name:
            return ErrWindow(name_err).exec_()

        query = "select name from goods"
        data = self.exec_query(query, 'fetchall')
        if data == 'error':
            return ErrWindow(connect_err).exec_()

        goods_names = [x[0] for x in data]
        if name in goods_names:
            return ErrWindow(samename_err).exec_()

        type = self.ui.catalog_type.text()
        if not type:
            return ErrWindow(type_err).exec_()
        manufacturer = self.ui.catalog_manufacturer.text()
        if not manufacturer:
            return ErrWindow(manufacturer_err).exec_()
        country = self.ui.catalog_country.text()
        if not country:
            return ErrWindow(country_err).exec_()
        price = self.ui.catalog_price.text()
        if float(price) == 0:
            return ErrWindow(price_err).exec_()
        warranty = self.ui.catalog_warranty.text()
        quantity = self.ui.catalog_quantity.text()
        supplier = self.ui.catalog_supplier.text()
        if not supplier:
            return ErrWindow(supplier_err).exec_()

        query = f"select id from supplier where name = '{supplier}'"
        data = self.exec_query(query, 'fetchone')
        if data == 'error':
            return ErrWindow(connect_err).exec_()
        if not data:
            address = self.ui.catalog_address.text()
            if not address:
                return ErrWindow(address_err).exec_()
            phone = self.ui.catalog_phone.text()
            if not phone:
                return ErrWindow(phone_err).exec_()
            query = f"insert into supplier values (default, '{supplier}', '{address}', '{phone}')"
            data = self.exec_query(query)
            if data == 'error':
                return ErrWindow(connect_err).exec_()
            self.show_suppliers()
            QtWidgets.QMessageBox.information(self, "Каталог товаров", "Новый поставщик успешно добавлен!")
            query = f"select id from supplier where name = '{supplier}'"
            data = self.exec_query(query, 'fetchone')
            if data == 'error':
                return ErrWindow(connect_err).exec_()
        supplier_id = int(data[0])

        query = f"insert into goods values (default, '{name}', '{type}', '{manufacturer}',\
            '{country}', {price}, {warranty}, {quantity}, {supplier_id}, {self.userid})"
        data = self.exec_query(query)
        if data == 'error':
            return ErrWindow(connect_err).exec_()

        self.ui.catalog_name.clear()
        self.ui.catalog_type.clear()
        self.ui.catalog_manufacturer.clear()
        self.ui.catalog_country.clear()
        self.ui.catalog_price.setValue(0)
        self.ui.catalog_warranty.setValue(0)
        self.ui.catalog_quantity.setValue(0)
        self.ui.catalog_supplier.clear()
        self.ui.catalog_address.clear()
        self.ui.catalog_phone.clear()           
        self.show_catalog()
        QtWidgets.QMessageBox.information(self, "Каталог товаров", "Новый товар успешно добавлен!")

    @staticmethod
    def update_table(table: QtWidgets.QTableWidget, data, resize_cols: bool = True):
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
