# -*- encoding: utf-8 -*-
# sqlite_mgm.py -
# Version: 1.0
# Author: Wang Zhuowei wang.zhuowei@foxmail.com
# Last Change: 2020 Apr 09
# License: GPL.v3

"""对python sqlite3库的封装
"""

# pylint: disable=invalid-name, no-else-return

import os
import sqlite3

class SqliteDbManager:
    """数据库的连接和管理的类

    执行对数据库的创建, 删除, 编辑操作

    >>> SqliteDbManager.create(name='e.db', schema='create table t (id int)')
    >>> data_base = SqliteDbManager('e.db')
    >>> with data_base as db:
    ...     db.execute('select * from t')
    >>> SqliteDbManager.remove('e.db')
    """

    DEFAULT_DB = ':memory:' # 默认为内存数据库

    @classmethod
    def create(cls, *, name: str = DEFAULT_DB, schema: str = ''):
        """根据初始sql语句创建数据库

        schema: 创建数据库时运行的语句
        name: 数据库路径
        """
        conn = sqlite3.connect(name)
        cursor = conn.cursor()
        try:
            cursor.executescript(schema)
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

    @classmethod
    def schema(cls, name=DEFAULT_DB) -> str:
        """查看数据库结构

        name: 数据库路径

        ret: 数据库结构
        """
        conn = sqlite3.connect(name)
        cursor = conn.cursor()
        result = 'schema:\n'
        try:
            c = cursor.execute('select name, sql from sqlite_master '
                               'where type = "table"')
            template = 'name: {}|sql: {}\n'
            for r in c.fetchall():
                result += template.format(r[0], r[1])
        except sqlite3.Error as error:
            print(error)
        finally:
            conn.close()
        return result

    def __init__(self, name=DEFAULT_DB):
        self._name = name
        self._conn = None

    def __enter__(self):
        self.open()
        return self

    def __exit__(self, err_type, value, trace):
        self.close()

    def __repr__(self):
        return '{}(name={})'.format(self.__class__.__name__, self._name)

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

    def connected(self) -> bool:
        """是否和数据库在连接状态"""
        return self._conn is not None

    def execute(self, cmd: str, *args):
        """执行sql命令，返回结果

        cmd: sql语句
        arg: sql语句所需要的完整参数
        """
        try:
            cursor = self._conn.cursor()
            return cursor.execute(cmd, *args)
        except AttributeError:
            raise sqlite3.ProgrammingError('Cannot operate on '
                                           'a closed database.')

    def executemany(self, cmd: str, arg_iter):
        """迭代执行sql命令, 返回结果

        cmd: sql语句
        arg_iter: 由完整执行一次sql命令所需参数所组成的迭代器
        """
        try:
            cursor = self._conn.cursor()
            return cursor.executemany(cmd, arg_iter)
        except AttributeError:
            raise sqlite3.ProgrammingError('Cannot operate on '
                                           'a closed database.')
