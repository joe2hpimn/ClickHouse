DROP TABLE IF EXISTS t0;

CREATE TABLE t0 (c0 Int) ENGINE = MergeTree() ORDER BY (c0 * NULL) SETTINGS allow_nullable_key = 1;
INSERT INTO TABLE t0 (c0) VALUES (1);
SELECT * FROM t0;

DROP TABLE t0;
