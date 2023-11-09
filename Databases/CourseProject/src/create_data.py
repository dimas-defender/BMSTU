from faker import Faker
from faker_vehicle import VehicleProvider
from faker_commerce import Provider
from random import randint, choice
from datetime import date
from hashlib import md5

fake = Faker('en')
fake.add_provider(VehicleProvider)
fake.add_provider(Provider)
N = 1000

# users
again = []
i = 0
f = open('csv/users.csv', 'w')
while i < N:
    user = fake.simple_profile()
    name = fake.first_name()
    lname = fake.last_name()
    mail = user['mail']
    login = user['username']
    pswd = fake.pystr(min_chars = 5, max_chars = 10)
    user_post = 'Manager'
    if (login, pswd) in again:
        continue
    pswd = md5(pswd.encode()).hexdigest()
    f.write(f"{login},{pswd},{name},{lname},{user_post},{mail}\n")
    again.append((login, pswd))
    i += 1
f.close()
again.clear()


# supplier
i = 0
f = open('csv/supplier.csv', 'w')
while i < N:
    name = fake.unique.company()
    name = name.replace(", ", "&")
    address = fake.street_address()
    phone = '+7 ('+str(randint(900, 999))+') '+str(randint(100, 999))+'-'+str(randint(10, 99))+'-'+str(randint(10, 99))
    f.write(f"{name},{address},{phone}\n")
    i += 1
f.close()
fake.unique.clear()

# goods
i = 0
f = open('csv/goods.csv', 'w')
while i < N:
    name = fake.unique.vehicle_model()
    type = fake.ecommerce_name()
    manufacturer = fake.company()
    manufacturer = manufacturer.replace(", ", "&")
    country = fake.country()
    price = randint(100, 10000)
    warranty = randint(3, 36)
    quantity = randint(5, 500)
    supplier_id = randint(1, N)
    user_id = randint(1, N)
    f.write(f"{name},{type},{manufacturer},{country},{price},{warranty},{quantity},{supplier_id},{user_id}\n")
    i += 1
f.close()
fake.unique.clear()

# client
i = 0
f = open('csv/client.csv', 'w')
while i < N:
    name = fake.first_name()
    phone = '+7 ('+str(randint(900, 999))+') '+str(randint(100, 999))+'-'+str(randint(10, 99))+'-'+str(randint(10, 99))
    total = randint(0, 100000)
    f.write(f"{name},{phone},{total}\n")
    i += 1
f.close()
fake.unique.clear()

start_date = date(year=2020, month=1, day=1)

# sale
i = 0
f = open('csv/sale.csv', 'w')
while i < N:
    client_id = randint(1, N)
    price = randint(100, 50000)
    address = fake.street_address()
    address.replace(',', 'x')
    datte = fake.date_between(start_date=start_date, end_date='now')
    f.write(f"{client_id},{price},{address},{datte}\n")
    i += 1
f.close()

# goods_to_sale
i = 0
f = open('csv/goods_to_sale.csv', 'w')
while i < N:
    goods_id = randint(1, N)
    quantity = randint(1, 10)
    sale_id = randint(1, N)
    if (goods_id, sale_id) in again:
        continue
    f.write(f"{goods_id},{quantity},{sale_id}\n")
    again.append((goods_id, sale_id))
    i += 1
f.close()
again.clear()