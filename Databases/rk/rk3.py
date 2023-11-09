import psycopg2
import py_linq

def task1_1(cursor):
    cursor.execute('SELECT fio from employee ')
    
    for row in cursor:
        print(row)


def task2_1(cursor):
    cursor.execute('SELECT fio from employee e join inoutt i on e.id = i.emp_id\
                    group by e.id, i.ttype\
                    having i.ttype = 2 and count(*) > 3')
    
    for row in cursor:
        print(row)


def task2_2(cursor):
    def count_times(emp):
        return Enumerable(emp).where(lambda s: s['ttype'] == 2).count()

    query = "select * from employee e join inoutt i on e.id = i.e_id"
    cursor.execute(query)
    emp = cursor.fetchall()

    return Enumerable(emp) \
        .where(lambda t: count_times(emp) > 3) \
        .to_list()

if __name__ == "__main__":
    conn = psycopg2.connect(dbname='rk3', user='postgres', password='3563', host='localhost')
    cursor = conn.cursor()

    task2_1(cursor)
    print(task2_2(cursor))
    
    cursor.close()
    conn.close()
