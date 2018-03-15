CREATE DATABASE Blog;

/*
Category 是博客文章的类别,如"历史","编程"
*/
CREATE TABLE Blog.Category(
id INT NOT NULL AUTO_INCREMENT,
description CHAR(50),
name VARCHAR(50) NOT NULL,
PRIMARY KEY(id)
);

/*
Series 是同一系列的文章,如果 rank 为 0,则表示散文(只有一篇 ArticleDTO 的 id 会指向它)
*/
CREATE TABLE Blog.Series(
id INT NOT NULL AUTO_INCREMENT,
name VARCHAR(64) NOT NULL ,
description CHAR(64),
datetime DATETIME DEFAULT NOW(),
timestamp INT(30) DEFAULT 0,
rank INT(1) DEFAULT 0,
category_id INT,
PRIMARY KEY (id),
FOREIGN KEY (category_id) REFERENCES Blog.Category(id)
);


/*
ArticleDTO 表示博客文章,rank 表示这篇 ArticleDTO 在一系列文章在网页中应该呈现的先后顺序
*/

CREATE TABLE Blog.Article(
id INT NOT NULL AUTO_INCREMENT,
title VARCHAR(64) NOT NULL,
datetime DATETIME DEFAULT NOW(),
timestamp BIGINT(30),
content VARCHAR(20000),
category_id INT,
series_id INT,
rank INT(3) DEFAULT 0,
PRIMARY KEY(id),
FOREIGN KEY(category_id) REFERENCES Blog.Category(id),
FOREIGN KEY(series_id) REFERENCES Blog.Series(id)
);


/*Image 用于存储博客文章用到的本地图片的路径,以便于在博客文章删除时将本地图片也删除*/

CREATE TABLE Blog.Image(
id INT AUTO_INCREMENT,
series_id INT,
category_id INT,
path VARCHAR(1024),
PRIMARY KEY(id),
FOREIGN KEY(series_id) REFERENCES Blog.Series(id),
FOREIGN KEY(category_id) REFERENCES Blog.Category(id)
);