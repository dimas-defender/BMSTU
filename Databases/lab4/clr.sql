--Скалярная функция
CREATE OR REPLACE FUNCTION get_founder_name(br_name varchar) RETURNS varchar
AS $$
brands = plpy.execute('SELECT * FROM brand')
for b in brands:
	if b['name'] == br_name:
		return b['founder']
return None
$$ LANGUAGE plpython3u;

SELECT get_founder_name('Ford');
SELECT get_founder_name('Nissan');
select get_founder_name('Some random name');


--Агрегатная функция
CREATE OR REPLACE FUNCTION get_brand_avg_prod_volume(id_ int) 
RETURNS decimal
AS $$
plan = plpy.prepare('SELECT product_volume FROM brand b JOIN factory f ON b.id = f.brand_id WHERE brand_id = $1', ['int'])
vol_values = plpy.execute(plan, [id_])
cnt = len(vol_values)
total_vol = 0
for v in vol_values:
	total_vol += v["product_volume"]
avg_vol = 0 if cnt == 0 else total_vol/cnt
return avg_vol
$$ LANGUAGE plpython3u;
			
SELECT * FROM get_brand_avg_prod_volume(35);

select brand_id, count(*) c 
from brand b JOIN factory f ON b.id = f.brand_id 
group by brand_id 
order by c desc limit 10;
select * from factory where brand_id = 35;


--Табличная функция
CREATE OR REPLACE FUNCTION get_brand_models(brand_id_ int)
RETURNS TABLE (
    name VARCHAR,
    engine_capacity numeric,
	horsepower int
) AS $$
plan = plpy.prepare('SELECT m.name, m.engine_capacity, m.horsepower\
					FROM model m join brand b on m.brand_id = b.id where b.id = $1', ['int'])
res = plpy.execute(plan, [brand_id_])
return res
$$ LANGUAGE PLPYTHON3U;

select * from get_brand_models(147);

SELECT b.id, count(*) c
from model m join brand b on m.brand_id = b.id
group by b.id
order by c desc;

select * from model where brand_id = 147;


--Хранимая процедура
CREATE OR REPLACE PROCEDURE change_pr_volume(br_id int, coeff decimal)
AS $$
    plan = plpy.prepare('UPDATE factory SET product_volume = product_volume * $1\
						WHERE brand_id = $2', ['decimal', 'int'])
    plpy.execute(plan, [coeff, br_id])
$$ LANGUAGE PLPYTHON3U;

CALL change_pr_volume(212, 0.9);
select * from factory where brand_id = 212;

select b.id, count(*) c 
from brand b join factory f on b.id = f.brand_id
group by b.id
order by c desc;


--Триггер
CREATE OR REPLACE FUNCTION new_factory_info() RETURNS TRIGGER AS $$
	if TD['event'] == 'INSERT':
		plpy.notice(f"{TD['new']['name']} is the new factory in {TD['new']['city']}!")
$$ LANGUAGE PLPYTHON3U;

CREATE TRIGGER factory_added
AFTER INSERT ON factory
FOR ROW EXECUTE PROCEDURE new_factory_info();

INSERT INTO factory(name, city, open_year, product_volume, brand_id)
VALUES ('Jack&Jones', 'London', 2008, 255000, 130);

DELETE FROM factory WHERE id > 1000;


--Тип данных
CREATE TYPE model_specs AS (
	horsepower int,
	engine_capacity numeric
);

CREATE OR REPLACE FUNCTION get_model_specs(id_ int) 
RETURNS model_specs AS $$
plan = plpy.prepare('SELECT horsepower, engine_capacity FROM model WHERE id = $1', ['int'])
res = plpy.execute(plan, [id_])
return res[0]
$$ language plpython3u;

SELECT * from get_model_specs(100);


