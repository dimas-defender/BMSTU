CREATE TABLE EmplVisits(
dept varchar,
fio varchar,
dt date,
status varchar
);

INSERT INTO EmplVisits VALUES
('ИТ','Иванов Иван Иванович','2020-01-15 ','Больничный'),
('ИТ','Иванов Иван Иванович','2020-01-16 ','На работе'),
('ИТ','Иванов Иван Иванович','2020-01-17 ','На работе'),
('ИТ','Иванов Иван Иванович','2020-01-18 ','На работе'),
('ИТ','Иванов Иван Иванович','2020-01-19 ','Оплачиваемый отпуск'),
('ИТ','Иванов Иван Иванович','2020-01-20 ','Оплачиваемый отпуск'),
('Бухгалтерия','Петрова Ирина Ивановна','2020-01-15 ','Оплачиваемый отпуск'),
('Бухгалтерия','Петрова Ирина Ивановна','2020-01-16 ','На работе'),
('Бухгалтерия','Петрова Ирина Ивановна','2020-01-17 ','На работе'),
('Бухгалтерия','Петрова Ирина Ивановна','2020-01-18 ','На работе'),
('Бухгалтерия','Петрова Ирина Ивановна','2020-01-19 ','Оплачиваемый отпуск'),
('Бухгалтерия','Петрова Ирина Ивановна','2020-01-20 ','Оплачиваемый отпуск');

SELECT * FROM EmplVisits ORDER BY fio, dt;

SELECT dept, fio, MIN(DateFrom) DateFrom, MAX(DateTo) DateTo, status
FROM(
    SELECT q.*, SUM(flg) OVER (PARTITION BY dept, fio, status ORDER BY DateFrom) rnk
    FROM(
        SELECT q.*,
            CASE 
            WHEN COALESCE(
				lag(DateTo) OVER (PARTITION BY dept, fio, status ORDER BY DateFrom),
                DateFrom - 1) = (DateFrom - 1)
			THEN 0
            ELSE 1
			END flg
        FROM (SELECT dept, fio, dt DateFrom, dt DateTo, status FROM EmplVisits)q
    )q
)q
GROUP BY dept, fio, rnk, status
ORDER BY fio, DateFrom;


INSERT INTO EmplVisits VALUES
('ИТ','Иванов Иван Иванович','2020-01-25 ','Оплачиваемый отпуск');

DELETE FROM EmplVisits WHERE dt > '2020-01-20';

	
