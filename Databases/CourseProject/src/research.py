import psycopg2 as db
from time import monotonic_ns
connection = db.connect(user="postgres",
                        password="3563",
                        host="localhost",
                        port="5432",
                        database="techshop")

N = 1000
query = 'SELECT COUNT(ID) FROM GOODS WHERE PRICE BETWEEN 4000 AND 5000;'

def query_time():
    res = 0
    for i in range(N):
        start = monotonic_ns()
        with connection.cursor() as curs:
            curs.execute(query)
        end = monotonic_ns()
        res += end - start

    print(res / 1000000 / 10)

query_time()

with connection.cursor() as curs:
        curs.execute('CREATE INDEX IND ON GOODS(PRICE);')

query_time()

with connection.cursor() as curs:
        curs.execute('DROP INDEX IND;')
