# -*- encoding: utf-8 -*-
# book_info.py -
# Version: 1.0
# Author: Wang Zhuowei wang.zhuowei@foxmail.com
# Last Change: 2020 Apr 10
# License: GPL.v3

"""从yousuu网读取网络小说的信息
"""


class BookInfo:
    """定义了书籍信息的结构体

    +-------------+--------+------------+
    | 名称        | 属性   |   注释     |
    +-------------+--------+------------+
    | id          | int    | 唯一标识符 |
    +-------------+--------+------------+
    | title       | string | 书名       |
    +-------------+--------+------------+
    | description | string | 简介       |
    +-------------+--------+------------+
    | tags        | string | 分类标签   |
    +-------------+--------+------------+
    | last_update | string | 更新时间   |
    +-------------+--------+------------+
    | word_count  | int    | 字数       |
    +-------------+--------+------------+
    | list_count  | int    | 加入书单数 |
    +-------------+--------+------------+
    | scorer_count| int    | 打分人数   |
    +-------------+--------+------------+
    | score       | real   | 评分       |
    +-------------+--------+------------+
    | finished    | int    | 是否完结   |
    +-------------+--------+------------+
    """

    CREATE_LIBRARY_SQL = """
        CREATE TABLE LIBRARY (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            TITLE TEXT NOT NULL,
            AUTHOR TEXT NOT NULL,
            DESCRIPTION TEXT NOT NULL,
            TAGS TEXT NOT NULL,
            LAST_UPDATE TEXT NOT NULL,
            WORD_COUNT INTEGER NOT NULL,
            LIST_COUNT INTEGER NOT NULL,
            SCORER_COUNT INTEGER NOT NULL,
            SCORE REAL NOT NULL,
            FINISHED INTEGER NOT NULL
        )
        """
    INSERT_SQL = 'INSERT INTO LIBRARY VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)'

    PATTERN = {
        'TITLE': r'.+recom_ignore.+?title":"(?P<TITLE>.+?)"',
        'AUTHOR': r'.+recom_ignore.+?author":"(?P<AUTHOR>.+?)"',
        'DESCRIPTION': r'.+recom_ignore.+?introduction":"(?P<DESCRIPTION>.+?)"',
        'TAGS': r'.+"bookInfo":{.+?tags":\[(?P<TAGS>.+?)\]',
        'LAST_UPDATE': r'.+recom_ignore.+?updateAt":"(?P<LAST_UPDATE>.+?)"',
        'WORD_COUNT': r'.+recom_ignore.+?countWord":(?P<WORD_COUNT>\d+?),',
        'LIST_COUNT': r'.+"bookInfo":{.+?addListTotal":(?P<LIST_COUNT>\d+?),',
        'SCORER_COUNT': r'.+"bookInfo":{.+?scorerCount":(?P<SCORER_COUNT>.+?),',
        'SCORE': r'.+"bookInfo":{.+?score":(?P<SCORE>.+?),',
        'FINISHED': r'.+"bookInfo":{.+?status":(?P<FINISHED>\d),'
    }
