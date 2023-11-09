-- Извлечь данные из таблицы в JSON
COPY (SELECT row_to_json(model) FROM model)
TO 'C:\Users\Public\DB\lab5\model.json';

-- Загрузка JSON-файла в таблицу
CREATE TABLE model_json (
    id int PRIMARY KEY,
    name varchar(100) NOT NULL,
    engine_capacity numeric,
    horsepower int,
    brand_id int NOT NULL
);

-- Создать таблицу с атрибутом JSON
CREATE TABLE model_temp (data json);
COPY model_temp FROM 'C:\Users\Public\DB\lab5\model.json';
select * from model_temp;

INSERT INTO model_json(id, name, engine_capacity, horsepower, brand_id)
SELECT (data->>'id')::int, data->>'name', (data->>'engine_capacity')::numeric,
(data->>'horsepower')::int, (data->>'brand_id')::int FROM model_temp;

select * from model_json;

-- Извлечь JSON фрагмент из JSON документа
SELECT data->'name' model_name, data->'brand_id' brand FROM model_temp;

-- Извлечь значения конкретных узлов или атрибутов JSON документа
SELECT data->'name' model_name FROM model_temp WHERE (data->>'horsepower')::int > 480;

-- Выполнить проверку существования узла или атрибута
CREATE OR REPLACE FUNCTION check_key(data json, key text) RETURNS BOOLEAN 
AS $$
    SELECT data::jsonb ? key;
$$ LANGUAGE SQL;

SELECT check_key(model_temp.data, 'engine_capacity') from model_temp limit 1;
SELECT check_key(model_temp.data, 'price') from model_temp limit 1;

-- Изменить JSON документ
UPDATE model_temp SET data = data::jsonb || '{"horsepower": 80}'::jsonb WHERE (data->>'engine_capacity')::numeric < 1.3;
select * from model_temp where (data->>'engine_capacity')::numeric < 1.3;

-- Разделить JSON документ на несколько строк по узлам
SELECT * FROM json_array_elements('[{"id" : 1, "name": "Elantra", "engine_capacity": 2.0, "horsepower": 150, "brand_id" : 51},
{"id" : 2, "name": "Solaris", "engine_capacity": 1.6, "horsepower": 122, "brand_id" : 132}]');