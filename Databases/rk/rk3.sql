--Вариант 3

create database rk3;

CREATE TABLE employee (
    id SERIAL PRIMARY KEY,
    fio VARCHAR NOT NULL,
    dob DATE NOT NULL,
    dep VARCHAR NOT NULL
);

CREATE TABLE inout (
    emp_id INT NOT NULL,
    ddate DATE NOT NULL,
    wday VARCHAR NOT NULL,
    ttime TIME NOT NULL,
    ttype INT NOT NULL,
    FOREIGN KEY (emp_id) REFERENCES employee(id)
);

INSERT INTO employee(fio, dob, dep) VALUES
('Иванов Иван Иванович', '1990-09-25', 'ИТ'),
('Петров Петр Петрович', '1987-11-12', 'Бухгалтерия'),
('Сидоров Петр Петрович', '1995-11-17', 'Бухгалтерия');

INSERT INTO inout(emp_id, ddate, wday, ttime, ttype) VALUES
(1, '2018-12-14', 'Суббота', '9:00', 1),
(1, '2018-12-14', 'Суббота', '9:20', 2),
(1, '2018-12-14', 'Суббота', '9:25', 1),
(2, '2018-12-14', 'Суббота', '9:05', 1),
(3, '2018-12-14', 'Суббота', '9:15', 1);

select * from employee;
select * from inout;
---------------

SELECT e.id, fio, dob, dep, age from employee e join (SELECT  id, EXTRACT (year from age(dob)) as age
from employee) s on e.id = s.id;

SELECT  *, row_number() over (PARTITION BY emp_id)
from inout;

CREATE TABLE emp (
    id SERIAL PRIMARY KEY,
    fio VARCHAR NOT NULL,
    dob DATE NOT NULL,
    dep VARCHAR NOT NULL,
	age int
);

INSERT INTO emp(fio, dob, dep, age)
SELECT  fio, dob, dep, EXTRACT (year from age(dob))
from employee;

select * from emp;

CREATE OR REPLACE FUNCTION young_late() RETURNS int AS $$
$$ LANGUAGE ;

SELECT DISTINCT e.fio FROM employee e JOIN inoutt i ON e.id = i.emp_id
WHERE i.ddate = '2018-12-14' AND i.ttype = 1 AND rnum = 1 AND
DATE_PART('minute', i.ttime::TIME - '9:00'::TIME) > 10
WHERE DATE_PART('year', age(dob)) = (SELECT MIN());