--Скалярная функция
CREATE OR REPLACE FUNCTION OLD_BRANDS_NUMBER() RETURNS int AS $$
	SELECT COUNT(*)
	FROM brand
	WHERE found_year < 1900;
$$ LANGUAGE SQL;

SELECT old_brands_number();

--Подставляемая табличная функция
CREATE OR REPLACE FUNCTION Brands_By_Country(cntry varchar) RETURNS TABLE(id int, name varchar, found_year int) AS $$
	SELECT id, name, found_year
	FROM brand
	WHERE country = cntry;
$$  LANGUAGE SQL;

SELECT * from Brands_By_Country('Bangladesh');

---Многооператорная табличная функция
CREATE OR REPLACE FUNCTION City_Factories(cty varchar)
RETURNS TABLE(name varchar, year int, product_volume int) AS $$
BEGIN
    CREATE TEMP TABLE tbl(name varchar, year int, product_volume int);
    INSERT INTO tbl(name, year, product_volume)
    SELECT f.name, f.open_year, f.product_volume FROM factory f WHERE f.city = cty;
    RETURN QUERY
    SELECT * FROM tbl;
END;
$$ LANGUAGE PLPGSQL;

SELECT * FROM City_Factories('Keithfort');

--Функция с рекурсивным ОТВ
CREATE OR REPLACE FUNCTION get_near_brands(brand_id int, year_gap int)
RETURNS TABLE (id int, name varchar, country varchar, found_year int) AS $$
BEGIN
    RETURN QUERY
    WITH RECURSIVE closest_brands(id, name, country, found_year) AS (
        SELECT b.id, b.name, b.country, b.found_year, 0 AS level FROM brand b
        WHERE b.id = brand_id
        UNION
        SELECT br.id, br.name, br.country, br.found_year, level + 1 FROM brand br
        JOIN closest_brands cb ON cb.found_year + 1 = br.found_year AND level < year_gap
    )
    SELECT cb.id, cb.name, cb.country, cb.found_year from closest_brands cb;
END;
$$ LANGUAGE PLPGSQL;

SELECT * FROM get_near_brands(250, 10);

--Хранимая процедура с параметрами
CREATE OR REPLACE PROCEDURE update_product_volume(ident int, value int)
AS $$
BEGIN
    UPDATE factory
    SET product_volume = value
    WHERE id = ident;
END;
$$ LANGUAGE PLPGSQL;

CALL update_product_volume(50, 257000);
select * from factory where id = 50;

--Рекурсивная хранимая процедура
CREATE OR REPLACE PROCEDURE update_horsepower(first_id int, last_id int, max_capacity numeric, value int) AS
$$
DECLARE
	cur_id int = first_id;
BEGIN
	IF (cur_id <= last_id)
THEN
	UPDATE model
	SET horsepower = least(horsepower, value)
	WHERE id = cur_id AND engine_capacity <= max_capacity;
	CALL update_horsepower(cur_id + 1, last_id, max_capacity, value);
END IF;
END;
$$ language plpgsql;

CALL update_horsepower(200, 300, 2.0, 180);
select * from model where id between 200 and 300;


--Хранимая процедура с курсором
CREATE OR REPLACE PROCEDURE fetch_latest_factories(min_year int)
AS $$
DECLARE
    row RECORD;
    cur CURSOR FOR
        SELECT * FROM factory where open_year >= min_year;
BEGIN
    OPEN cur;
    LOOP
        FETCH cur INTO row;
		EXIT WHEN NOT FOUND;
        RAISE NOTICE '% was opened in % in %.', row.name, row.city, row.open_year;
    END LOOP;
    CLOSE cur;
END;
$$ LANGUAGE PLPGSQL;

CALL fetch_latest_factories(2000);

--Хранимая процедура доступа к метаданным
CREATE OR REPLACE PROCEDURE get_schema_tables(sch varchar) AS $$
DECLARE
    r RECORD;
    cnt INT;
BEGIN
    FOR r IN
        SELECT table_name FROM information_schema.TABLES
        WHERE table_schema = sch AND table_type != 'VIEW'
        ORDER BY table_name
    LOOP
        EXECUTE 'select count(*) cnt FROM ' || r.table_name INTO cnt;
        RAISE NOTICE '% %', r.table_name, cnt;
    END LOOP;
END;
$$ language plpgsql;

CALL get_schema_tables('public');
CALL get_schema_tables('pg_catalog');

--Триггер AFTER
CREATE OR REPLACE FUNCTION increase_eng_capacity() RETURNS TRIGGER AS $$
	BEGIN
		IF (TG_OP = 'UPDATE') THEN
			UPDATE model
			SET engine_capacity = engine_capacity * 1.1
			WHERE model.id = old.id;
			RETURN new;
		END IF;
	END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER model_power_changed
	AFTER UPDATE OF horsepower ON model
	FOR EACH ROW
	WHEN (new.horsepower > old.horsepower)
	EXECUTE PROCEDURE increase_eng_capacity();
	
select * from model where id = 43;
update model set horsepower = 265 where id = 43;


--Триггер INSTEAD OF
CREATE OR REPLACE FUNCTION check_model() RETURNS TRIGGER AS $$
DECLARE
	fixed_capacity numeric;
BEGIN
	IF (new.horsepower > 200 and new.engine_capacity < 2.0) THEN
		fixed_capacity = 2.0;
	ELSE
		fixed_capacity = new.engine_capacity;
	END IF;
	INSERT INTO model(name, engine_capacity, horsepower, brand_id)
	VALUES (new.name, fixed_capacity, new.horsepower, new.brand_id);
	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

CREATE VIEW model_view AS
SELECT * FROM model order by id desc LIMIT 30;

drop view model_view;

CREATE TRIGGER model_insert
INSTEAD OF INSERT ON model_view
FOR EACH ROW
EXECUTE PROCEDURE check_model();

select * from model_view;
delete from model where id > 1000;

INSERT INTO model_view(name, engine_capacity, horsepower, brand_id)
	VALUES ('Logan', 1.6, 102, 5);

INSERT INTO model_view(name, engine_capacity, horsepower, brand_id)
	VALUES ('Qashqai', 1.6, 220, 6);

