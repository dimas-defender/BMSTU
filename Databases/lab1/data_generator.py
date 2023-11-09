from faker import Faker
from faker_vehicle import VehicleProvider
from random import randint, random

F = Faker()
F.add_provider(VehicleProvider)

N = 1000

f = open('models.csv', 'w')
for i in range(N):
    brand_id = randint(1, N)
    name = F.vehicle_model()
    engine_capacity = 0.8 + 4.2 * random()
    horsepower = randint(80,500)
    string = "{0},{1},{2},{3}\n".format(name, engine_capacity, horsepower, brand_id)
    f.write(string)
f.close()

f = open('brands.csv', 'w')
for i in range(N):
    name = F.vehicle_make()
    name = name.replace(',', ' &')
    name = name.replace('and', '&')
    country = F.country()
    country = country.replace(',', ' &')
    country = country.replace('and', '&')
    found_year = randint(1870,1990)
    founder = F.name()
    string = "{0},{1},{2},{3}\n".format(name, country, found_year, founder)
    f.write(string)
f.close()

f = open('factories.csv', 'w')
for i in range(N):
    name = F.company()
    name = name.replace(',', ' &')
    name = name.replace('and', '&')
    city = F.city()
    city = city.replace(',', ' &')
    city = city.replace('and', '&')
    open_year = randint(1900,2020)
    product_volume = randint(5000,500000)
    brand_id = randint(1,N)
    string = "{0},{1},{2},{3},{4}\n".format(name, city, open_year, product_volume, brand_id)
    f.write(string)
f.close()

f = open('connections.csv', 'w')
for i in range(N):
    model_id = randint(1,1000)
    factory_id = randint(1,1000)
    year = randint(1950,2021)
    string = "{0},{1},{2}\n".format(model_id, factory_id, year)
    f.write(string)
f.close()

