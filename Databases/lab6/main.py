import psycopg2

def scalar(cursor):
    cursor.execute('SELECT AVG(product_volume) from factory')
    print("Средний объем производства по всем заводам:", end = ' ')
    print(cursor.fetchone()[0], '\n')

def joins(cursor):
    cursor.execute('SELECT f.name, count(*) from model m JOIN model_to_factory mf\
                    on m.id = mf.model_id JOIN factory f on mf.factory_id = f.id\
                    GROUP BY f.name\
                    HAVING count(*) > 3')
    
    print("Заводы, выпускающие более 3 моделей машин:")
    for row in cursor:
        print(row)

def cte_partition(cursor):
    print('Заводы, открытые после определенного года')
    year = int(input("Введите год: "))
    cursor.execute("with cte(name, city, open_year, product_volume) as(\
                   select name, city, open_year, product_volume from factory\
	            where open_year >= %s)\
                    SELECT name, city, open_year,\
                    MAX(product_volume) OVER(PARTITION BY open_year) AS MaxVol\
                    FROM cte", (year, ))
    for row in cursor:
        print(row)

def metadata(cursor):
    cursor.execute("SELECT count(*) FROM pg_proc\
                    where pronamespace =\
                    (SELECT oid\
                    FROM pg_namespace\
                    WHERE nspname = 'public')")
        
    print("Количество созданных процедур и функций:", end = ' ')
    print(cursor.fetchone()[0], '\n')

def call_scalar(cursor):
    cursor.execute("SELECT OLD_BRANDS_NUMBER();")
    print("Количество брендов, основанных до 1900 года:", end = ' ')
    print(cursor.fetchone()[0], '\n')

def call_table(cursor):
    print("Бренды из заданной страны")
    cntr = input("Введите название страны: ")
    cursor.execute('SELECT Brands_By_Country(%s)', (cntr, ))
    for row in cursor:
        print(row)

def procedure(cursor, conn):
    print("Изменяет значение объема производства для заданного завода")
    f_id = int(input("Введите id завода: "))
    new_val = int(input("Введите новое значение объема производства: "))
    cursor.execute("CALL update_product_volume(%s, %s);", [f_id, new_val, ])
    conn.commit()

def call_system(cursor):
    print("Имя текущей базы данных:", end = ' ')
    cursor.callproc('current_database')
    print(cursor.fetchone()[0], '\n')

def create_table(cursor, conn):
    print("Создать таблицу автодилеров")
    cursor.execute("CREATE TABLE IF NOT EXISTS dealer(\
                   id SERIAL PRIMARY KEY NOT NULL, name VARCHAR(40),\
                   address VARCHAR(50))")
    print("Таблица была создана.")
    conn.commit()

def insert_into(cursor, conn):
    print("Вставить данные в таблицу")
    name = input("Введите название дилера: ")
    address = input("Введите адрес автосалона: ")
    cursor.execute("INSERT INTO dealer(name, address) VALUES(%s, %s)", (name, address, ))
    print("Данные были добавлены.")
    conn.commit()

if __name__ == "__main__":
    conn = psycopg2.connect(dbname='cars', user='postgres', password='3563', host='localhost')
    cursor = conn.cursor()
    while 1:
        print("\nВыберите пункт меню:")
        print("1. Скалярный запрос")
        print("2. Запрос с несколькими JOIN")
        print("3. Запрос c ОТВ и оконными функциями")
        print("4. Запрос к метаданным")
        print("5. Вызвать скалярную функцию")
        print("6. Вызвать подставляемую табличную функцию")
        print("7. Вызвать хранимую процедуру")
        print("8. Вызвать системную функцию")
        print("9. Создать таблицу")
        print("10. Выполнить вставку данных в созданную таблицу")
        print("0. Выход\n")
        choice = int(input("Номер пункта: "))
        if not choice:
            cursor.close()
            conn.close()
            break
        elif choice == 1:
            scalar(cursor)
        elif choice == 2:
            joins(cursor)
        elif choice == 3:
            cte_partition(cursor)
        elif choice == 4:
            metadata(cursor)
        elif choice == 5:
            call_scalar(cursor)
        elif choice == 6:
            call_table(cursor)
        elif choice == 7:
            procedure(cursor, conn)
        elif choice == 8:
            call_system(cursor)
        elif choice == 9:
            create_table(cursor, conn)
        elif choice == 10:
            insert_into(cursor, conn)
