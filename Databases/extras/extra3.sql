CREATE TABLE numbers(
a decimal
);

INSERT INTO numbers VALUES (5), (4.5), (-2.01), (-1), (2.27), (0.621), (-2.37), (1.4);
SELECT * FROM numbers;

--ver.1
CREATE OR REPLACE FUNCTION prod(i int) RETURNS decimal AS $$
WITH nums AS (
	SELECT a, row_number() OVER () AS id FROM numbers ORDER BY id)
SELECT CASE
	WHEN i < (SELECT COUNT(*) FROM NUMBERS)
	THEN (SELECT a * prod(i + 1) FROM nums WHERE id = i)
	ELSE (SELECT a FROM nums WHERE id = i)
	   END prod
$$ LANGUAGE SQL;

SELECT * FROM prod(1);

--ver.2
WITH cnt AS (
SELECT SUM(CASE WHEN a < 0 THEN 1 ELSE 0 END) neg,
SUM(CASE WHEN a > 0 THEN 1 ELSE 0 END) pos,
COUNT(a) total FROM numbers)
SELECT CASE
	WHEN total <> pos+neg THEN 0
	ELSE (CASE WHEN neg%2 = 1 THEN -1 ELSE 1 END) * exp(SUM(ln(abs(a))))
END prod
FROM numbers, cnt
WHERE a <> 0
GROUP BY total, pos, neg;
