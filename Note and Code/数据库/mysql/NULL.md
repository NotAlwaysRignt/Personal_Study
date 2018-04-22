NULL 是mysql中一种特殊的值,特别要注意的是使用 NULL 作为判断条件时要用 IS  和 IS NOT 而不是 = 或 !=

举例:

mysql> select NULL = NULL;
+-------------+
| NULL = NULL |
+-------------+
|        NULL |
+-------------+
1 row in set (0.00 sec)

mysql> select NULL IS NULL;
+--------------+
| NULL IS NULL |
+--------------+
|            1 |
+--------------+
1 row in set (0.00 sec)

NULL 常常作为表连接的判断条件,比如:
使用join查询方式找出没有分类的电影id以及名称

```sql
# 两个表如下
CREATE TABLE IF NOT EXISTS film (
film_id smallint(5)  NOT NULL DEFAULT '0',
title varchar(255) NOT NULL,
description text,
PRIMARY KEY (film_id));

CREATE TABLE film_category  (
film_id  smallint(5)  NOT NULL,
category_id  tinyint(3)  NOT NULL, `last_update` timestamp);

# sql语句如下:
select film.film_id,film.title from film left join film_category on film.film_id =
film_category.film_id where category_id IS NULL;
```



