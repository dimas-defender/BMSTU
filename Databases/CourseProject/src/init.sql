drop database if exists techshop;
drop user if exists admindb;
drop user if exists manager;
drop user if exists customer;
create database techshop;
\c techshop;

create table users (
	id serial primary key,
	login varchar(32) not null,
	password varchar(32) not null,
	unique (login, password),
	firstname varchar(32) not null,
	lastname varchar(32) not null,
	post varchar(32),
	email varchar(32) not null,
	check (post in ('Admin', 'Manager'))
);

create table supplier (
	id serial primary key,
	name varchar(64) not null,
	address varchar(64) not null,
	phone varchar(32) not null,
	unique (name)
);

create table goods (
	id serial primary key,
	name varchar(64) not null,
	type varchar(64) not null,
	manufacturer varchar(64) not null,
	country varchar(64) not null,
	price int not null,
	warranty int not null,
	quantity int not null,
	supplier_id int references supplier,
	user_id int references users,
	unique (name)
);

create table client (
	id serial primary key,
	name varchar(32) not null,
	phone varchar(32) not null,
	total int default 0,
	unique (phone)
);

create table sale (
	id serial primary key,
	client_id int references client,
	price int not null,
	address varchar(64) not null,
	date date not null
);

create table goods_to_sale (
	id serial primary key,
	goods_id int references goods,
	quantity int not null,
	sale_id int references sale,
	unique (sale_id, goods_id)
);

insert into users values (default, 'zhabindv', '50ecdaa17384f3c064071b896f28a4b5', 'Dmitriy', 'Zhabin', 'Admin', 'studystuff@mail.ru');
\copy users(login, password, firstname, lastname, post, email) from 'C:\Users\Public\DB_CP\src\csv\users.csv' delimiter ',' csv;
\copy supplier(name, address, phone) from 'C:\Users\Public\DB_CP\src\csv\supplier.csv' delimiter ',' csv;
\copy goods(name, type, manufacturer, country, price, warranty, quantity, supplier_id, user_id) from 'C:\Users\Public\DB_CP\src\csv\goods.csv' delimiter ',' csv;				
\copy client(name, phone, total) from 'C:\Users\Public\DB_CP\src\csv\client.csv' delimiter ',' csv;
\copy sale(client_id, price, address, date) from 'C:\Users\Public\DB_CP\src\csv\sale.csv' delimiter ',' csv;
\copy goods_to_sale(goods_id, quantity, sale_id) from 'C:\Users\Public\DB_CP\src\csv\goods_to_sale.csv' delimiter ',' csv;

--triggers
create function update_quantity()
returns trigger as $$
	begin
		update goods set quantity = quantity - new.quantity where id = new.goods_id;
		return new;
	end
$$ language plpgsql;

create trigger new_sale after insert on goods_to_sale
	for each row execute procedure update_quantity();

create function del_user()
returns trigger as
$$
	begin
		update users set post = null where id = old.id;
		return null;
	end
$$
language plpgsql;

create trigger dismissal_user before delete on users
	for each row execute procedure del_user();


--functions
create extension plpython3u;
create type connect_str as (
  	id int,
  	post varchar(64),
  	login_to_connect varchar(64),
	pswd_to_connect varchar(64)
);
create function get_connect_str(lgn varchar, pswd varchar)
returns setof connect_str as
$$
	if not lgn and not pswd:
		return ([None, 'Customer', 'customer', 'techshop_customer'],)
	plan = plpy.prepare("select id, post from users\
						where login = $1 and password = $2", ["text", "text"])
	rv = plpy.execute(plan, [lgn, pswd], 1)
	res = ([None, None, None, None],)
	if rv:
		res = ([rv[0]['id'], rv[0]['post'], None, None],)
		if rv[0]['post'] == 'Manager':
			res[0][2] = 'manager'
			res[0][3] = 'techshop_manager'
		elif rv[0]['post'] == 'Admin':
			res[0][2] = 'admindb'
			res[0][3] = 'techshop_admin'
	return res
$$
language plpython3u;


--roles
create user customer with password 'techshop_customer';
grant select, update on goods to customer;
grant usage, select on sequence goods_id_seq to customer;
grant insert on goods_to_sale to customer;
grant usage, select on sequence goods_to_sale_id_seq to customer;
grant insert, select on sale to customer;
grant usage, select on sequence sale_id_seq to customer;
grant insert, update, select on client to customer;
grant usage, select on sequence client_id_seq to customer;

create user manager with password 'techshop_manager';
grant select, insert, update on goods to manager;
grant usage, select on sequence goods_id_seq to manager;
grant select on goods_to_sale to manager;
grant usage, select on sequence goods_to_sale_id_seq to manager;
grant select on sale to manager;
grant usage, select on sequence sale_id_seq to manager;
grant insert, select on supplier to manager;
grant usage, select on sequence supplier_id_seq to manager;
grant select on client to manager;
grant usage, select on sequence client_id_seq to manager;

create user admindb with password 'techshop_admin' connection limit 1;
grant select, update, insert, delete on users to admindb;
grant usage, select on sequence users_id_seq to admindb;