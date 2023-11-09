CREATE TABLE Table1(
    id int,
    var1 varchar(10) ,
    valid_from_dttm DATE,
    valid_to_dttm DATE
);

CREATE TABLE Table2(
    id int,
    var2 varchar(10) ,
    valid_from_dttm DATE,
    valid_to_dttm DATE
);

INSERT INTO Table1(id, var1, valid_from_dttm, valid_to_dttm)
VALUES (1, 'A', '2018-09-01', '2018-09-15');
INSERT INTO Table1(id, var1, valid_from_dttm, valid_to_dttm)
VALUES (1, 'B', '2018-09-16', '5999-12-31');
INSERT INTO Table2(id, var2, valid_from_dttm, valid_to_dttm)
VALUES (1, 'A', '2018-09-01', '2018-09-18');
INSERT INTO Table2(id, var2, valid_from_dttm, valid_to_dttm)
VALUES (1, 'B', '2018-09-19', '5999-12-31');

select * from table1;
select * from table2;

select t1.id, var1, var2, 
	GREATEST(t1.valid_from_dttm,  t2.valid_from_dttm) valid_from_dttm, 
	LEAST(t1.valid_to_dttm,  t2.valid_to_dttm) valid_to_dttm
from table1 t1 join table2 t2 on t1.id = t2.id
where GREATEST(t1.valid_from_dttm,  t2.valid_from_dttm) < LEAST(t1.valid_to_dttm,  t2.valid_to_dttm);
