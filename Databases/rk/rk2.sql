--Вариант 4

--create database RK2;

create table emp(
	id int PRIMARY KEY GENERATED ALWAYS AS IDENTITY (START WITH 1),
	fio varchar(50),
	birth_year int,
	expr int,
	ph_number varchar(30)	
);

create table sec_post(
	id int PRIMARY KEY GENERATED ALWAYS AS IDENTITY (START WITH 1),
	name varchar(50),
	address varchar(50),
	emp_id int references emp(id)
);

create table duty(
	id int PRIMARY KEY GENERATED ALWAYS AS IDENTITY (START WITH 1),
	dttm date,
	hours varchar(30)
);

create table emp_to_duty(
	id int PRIMARY KEY GENERATED ALWAYS AS IDENTITY (START WITH 1),
	emp_id int,
	duty_id int
);

INSERT INTO emp(fio, birth_year, expr, ph_number) VALUES ('Ivanov A.A.', 1980, 20, '5456431');
INSERT INTO emp(fio, birth_year, expr, ph_number) VALUES ('Petrov A.A.', 1987, 15, '213151');
INSERT INTO emp(fio, birth_year, expr, ph_number) VALUES ('Sidorov B.V.', 1979, 25, '841131');
INSERT INTO emp(fio, birth_year, expr, ph_number) VALUES ('Gurin P.M.', 1992, 7, '75131491');
INSERT INTO emp(fio, birth_year, expr, ph_number) VALUES ('Mishin V.V.', 1980, 18, '3214211');
INSERT INTO emp(fio, birth_year, expr, ph_number) VALUES ('Volkov E.A.', 1995, 5, '11231831');
INSERT INTO emp(fio, birth_year, expr, ph_number) VALUES ('Barinov A.V.', 1970, 29, '6531331');
INSERT INTO emp(fio, birth_year, expr, ph_number) VALUES ('Larin D.V.', 1994, 5, '7521931');
INSERT INTO emp(fio, birth_year, expr, ph_number) VALUES ('Vasilev G.R.', 1999, 2, '1385931');
INSERT INTO emp(fio, birth_year, expr, ph_number) VALUES ('Gorin S.G.', 1975, 17, '6163931');

INSERT INTO sec_post(name, address, emp_id) VALUES ('Main bulding', 'Pushkina, 9', 3);
INSERT INTO sec_post(name, address, emp_id) VALUES ('Headquarters', 'Pushkina, 7', 1);
INSERT INTO sec_post(name, address, emp_id) VALUES ('Main entrance', 'Lenina, 23', 8);
INSERT INTO sec_post(name, address, emp_id) VALUES ('Extra entrance', 'Lenina, 25', 10);
INSERT INTO sec_post(name, address, emp_id) VALUES ('Hall', 'Pushkina, 7', 1);
INSERT INTO sec_post(name, address, emp_id) VALUES ('Carpark', 'Petrova, 5', 5);
INSERT INTO sec_post(name, address, emp_id) VALUES ('Subway entrance', 'Petrova, 2', 6);
INSERT INTO sec_post(name, address, emp_id) VALUES ('Gate 1', 'Pushkina, 1', 3);
INSERT INTO sec_post(name, address, emp_id) VALUES ('Gate 2', 'Pushkina, 11', 7);
INSERT INTO sec_post(name, address, emp_id) VALUES ('Gate 3', 'Pushkina, 25', 2);

INSERT INTO duty(dttm, hours) VALUES ('2021-05-15', '09:00-15:00');
INSERT INTO duty(dttm, hours) VALUES ('2021-05-15', '15:00-21:00');
INSERT INTO duty(dttm, hours) VALUES ('2021-05-16', '09:00-15:00');
INSERT INTO duty(dttm, hours) VALUES ('2021-05-16', '15:00-21:00');
INSERT INTO duty(dttm, hours) VALUES ('2021-05-17', '09:00-15:00');
INSERT INTO duty(dttm, hours) VALUES ('2021-05-17', '15:00-21:00');
INSERT INTO duty(dttm, hours) VALUES ('2021-05-18', '09:00-15:00');
INSERT INTO duty(dttm, hours) VALUES ('2021-05-18', '15:00-21:00');
INSERT INTO duty(dttm, hours) VALUES ('2021-05-19', '09:00-15:00');
INSERT INTO duty(dttm, hours) VALUES ('2021-05-19', '15:00-21:00');

INSERT INTO emp_to_duty(emp_id, duty_id) VALUES (1, 2);
INSERT INTO emp_to_duty(emp_id, duty_id) VALUES (1, 7);
INSERT INTO emp_to_duty(emp_id, duty_id) VALUES (2, 5);
INSERT INTO emp_to_duty(emp_id, duty_id) VALUES (7, 5);
INSERT INTO emp_to_duty(emp_id, duty_id) VALUES (3, 3);
INSERT INTO emp_to_duty(emp_id, duty_id) VALUES (6, 9);
INSERT INTO emp_to_duty(emp_id, duty_id) VALUES (5, 10);
INSERT INTO emp_to_duty(emp_id, duty_id) VALUES (2, 8);
INSERT INTO emp_to_duty(emp_id, duty_id) VALUES (8, 1);
INSERT INTO emp_to_duty(emp_id, duty_id) VALUES (10, 6);

--Инструкция SELECT, использующая поисковое выражение CASE
--Вывести ФИО сотрудников с указанием степени их опытности в зависимости от стажа работы
SELECT fio,
	CASE
		WHEN expr < 6 then 'Inexperienced'
		WHEN expr > 15 then 'Expert'
		ELSE 'Skilled'
	END as Experience
FROM emp;

--Инструкция UPDATE со скалярным подзапросом в предложении SET
--Изменить значение стажа Горина С.Г. на значение, на год большее, чем у самого опытного сотрудника
UPDATE emp
SET expr = (SELECT max(expr)+1 from emp)
WHERE fio = 'Gorin S.G.';

--Инструкция SELECT, консолидирующая данные с помощью предложения GROUP BY и предложения HAVING
--Вывести id, ФИО и кол-во дежурств сотрудников, которым назначено несколько дежурств
select e.id, e.fio, count(*) as duty_cnt
from emp e join emp_to_duty ed on e.id = ed.emp_id join duty d on ed.duty_id = d.id
group by e.id, e.fio
having count(*) > 1;

CREATE OR REPLACE PROCEDURE drop_ddl_triggers(INOUT cnt int) AS $$
DECLARE
	r RECORD;
	cnt int = 0;
BEGIN
	FOR r IN
		SELECT evtname FROM pg_catalog.pg_event_trigger
	LOOP
		EXECUTE 'drop event trigger ' || r.evtname;
		cnt = cnt + 1;
	END LOOP;
	RETURN;
END;
$$ LANGUAGE PLPGSQL;