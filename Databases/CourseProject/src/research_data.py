from faker import Faker
from faker_vehicle import VehicleProvider
from faker_commerce import Provider
from random import randint, choice
from datetime import date
from hashlib import md5

fake = Faker('en')
fake.add_provider(VehicleProvider)
fake.add_provider(Provider)
N = 10000

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
    name = fake.unique.pystr(min_chars = 5, max_chars = 10)
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
    name = fake.unique.pystr(min_chars = 5, max_chars = 10)
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