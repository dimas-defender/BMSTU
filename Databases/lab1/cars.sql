--CREATE DATABASE cars;

CREATE TABLE public.brand (
    id int PRIMARY KEY GENERATED ALWAYS AS IDENTITY (START WITH 1),
    name varchar(100) NOT NULL,
    country varchar(100),
    found_year int,
    founder varchar(100)
);

CREATE TABLE public.factory (
    id int PRIMARY KEY GENERATED ALWAYS AS IDENTITY (START WITH 1),
    name varchar(100) NOT NULL,
    city varchar(100),
    open_year int,
    product_volume int,
	brand_id int NOT NULL references brand(id)
);

CREATE TABLE public.model (
    id int PRIMARY KEY GENERATED ALWAYS AS IDENTITY (START WITH 1),
    name varchar(100) NOT NULL,
    engine_capacity numeric,
    horsepower int,
    brand_id int NOT NULL references brand(id)
);


CREATE TABLE public.model_to_factory (
    id int PRIMARY KEY GENERATED ALWAYS AS IDENTITY (START WITH 1),
    model_id int NOT NULL REFERENCES model(id),
    factory_id int NOT NULL REFERENCES factory(id),
    year int
);

