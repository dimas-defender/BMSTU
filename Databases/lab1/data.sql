copy brand(name, country, found_year, founder) from 'C:\Users\Public\DB\lab1\brands.csv' with (format csv);
copy model(name, engine_capacity, horsepower, brand_id) from 'C:\Users\Public\DB\lab1\models.csv' with (format csv);
copy factory(name, city, open_year, product_volume, brand_id) from 'C:\Users\Public\DB\lab1\factories.csv' with (format csv);
copy model_to_factory(model_id, factory_id, year) from 'C:\Users\Public\DB\lab1\connections.csv' with (format csv);