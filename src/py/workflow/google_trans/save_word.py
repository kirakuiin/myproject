# -*- encoding: utf-8 -*-
# author: wang.zhuowei@foxmail.com
# date: Nov 9, 2018
# license: GPL.v3

"""存储Google翻译的单词结果到sqlite数据库之中

这段代码并不是单独作为一个程序运行, 而是作为Alfred的一个组件完成调用存储
结果这一工作

"""

# pylint: disable=invalid-name, no-else-return

import os
import sqlite3
import sys
from collections import namedtuple


WordInfo = namedtuple('WordInfo', 'word, s_lang, t_lang, meaning, dict_def')


class DataBaseManager:
    """数据库的连接和管理的类

    可以创建, 删除数据库; 向数据库里插入单词信息
    属性为:
    +--------- +--------+------------+
    | 名称     | 属性   |   注释     |
    +----------+--------+------------+
    | id       | int    | 唯一标识符 |
    +----------+--------+------------+
    | word     | string | 单词的拼写 |
    +----------+--------+------------+
    | s_lang   | string | 源语言     |
    +----------+--------+------------+
    | t_lang   | string | 目标语言   |
    +----------+--------+------------+
    | meaning  | string | 目标语翻译 |
    +----------+--------+------------+
    | dict_def | string | 单词释义   |
    +----------+--------+------------+
    | frequency| int    | 查询次数   |
    +----------+--------+------------+

    >>> DataBaseManager.create(DataBaseManager.DEFAULT_DB)
    >>> data_base = DataBaseManager()
    >>> with data_base as db:
    ...     db.post(WordInfo('a', 'en', 'ja', 'ひとつ', ''))
    >>> DataBaseManager.remove(DataBaseManager.DEFAULT_DB)

    """

    DEFAULT_DB = 'word_info.db'

    @classmethod
    def create(cls, name=DEFAULT_DB):
        """创建数据库

        name: 数据库路径
        """
        SCHEMA = """
            CREATE TABLE wordinfo (
                ID INTEGER PRIMARY KEY AUTOINCREMENT,
                WORD TEXT NOT NULL,
                S_LANG TEXT NOT NULL,
                T_LANG TEXT NOT NULL,
                MEANING TEXT NOT NULL,
                DICT_DEF TEXT NOT NULL,
                FREQUENCY INTEGER NOT NULL
            )
        """
        conn = sqlite3.connect(name)
        cursor = conn.cursor()
        try:
            cursor.execute(SCHEMA)
            conn.commit()
        except sqlite3.Error as error:
            print(error)
        finally:
            conn.close()

    @classmethod
    def remove(cls, name=DEFAULT_DB):
        """删除数据库

        name: 数据库路径
        """
        try:
            os.remove(name)
        except FileNotFoundError:
            print('database file {} not found.'.format(name))

    def __init__(self, name=DEFAULT_DB):
        self._name = name
        self._conn = None

    def __enter__(self):
        self.open()
        return self

    def __exit__(self, err_type, value, trace):
        self.close()

    def __repr__(self):
        return 'DataBaseManager(name={})'.format(self._name)

    def open(self):
        """打开一个数据库连接"""
        self._conn = sqlite3.connect(self._name)

    def close(self):
        """关闭当前数据库连接"""
        try:
            self._conn.close()
            self._conn = None
        except sqlite3.Error:
            pass

    def connected(self):
        """是否和数据库在连接状态"""
        return self._conn is not None

    def post(self, word_info: WordInfo):
        """向数据库里插入一项单词信息

        如果单词已经存在, 则会将其FREQUENCY字段增加1

        word_info: 单词信息
        """
        res, freq = self._word_exists(word_info)
        if res:
            self._update_frequency(word_info, freq + 1)
        else:
            INSERT_SQL = 'INSERT INTO wordinfo VALUES (?, ?, ?, ?, ?, ?, ?)'
            self._execute(INSERT_SQL, (None, ) + tuple(word_info) + (1,))
        self._conn.commit()

    def _word_exists(self, word_info: WordInfo):
        """判断某个单词在数据库中是否存在"""
        SELECT_SQL = ('SELECT frequency FROM wordinfo WHERE word LIKE ? '
                      'AND s_lang LIKE ? AND t_lang LIKE ?')
        result = self._execute(SELECT_SQL, tuple(word_info[:3])).fetchone()
        if result:
            return (True, result[0])
        else:
            return (False, 0)

    def _update_frequency(self, word_info: WordInfo, freq):
        """更新某个单词的查询频率"""
        UPDATE_SQL = ('UPDATE wordinfo SET frequency = ? '
                      'WHERE word LIKE ? AND s_lang LIKE ? '
                      'AND t_lang LIKE ?')
        self._execute(UPDATE_SQL, (freq, ) + tuple(word_info[:3]))

    def _execute(self, cmd, *args):
        """执行sql命令，返回结果"""
        try:
            cursor = self._conn.cursor()
            return cursor.execute(cmd, *args)
        except AttributeError:
            raise sqlite3.ProgrammingError('Cannot operate on '
                                           'a closed database.')


def parse_argv(args: list = None) -> WordInfo:
    """解析输入的命令行参数

    从左到右依次为: 源单词, 源语言, 目标语言, 目标翻译, 字典定义

    argv: 命令行参数
    return: 单词信息

    """

    if args is None:
        args = sys.argv
    argv_list = args[1:]    # 排除脚本名称
    return WordInfo(*argv_list)


def save_word(word_info: WordInfo):
    """将单词信息保存到数据库中"""
    DB_PATH = '/Users/lambda/Documents/Alfred/google_trans/dict.db'
    if not os.path.exists(os.path.dirname(DB_PATH)):
        os.mkdir(os.path.dirname(DB_PATH))

    DataBaseManager.create(DB_PATH)
    with DataBaseManager(DB_PATH) as db:
        if word_info.dict_def:
            db.post(word_info)


def main():
    """处理google翻译结果"""
    save_word(parse_argv())


if __name__ == '__main__':
    main()
