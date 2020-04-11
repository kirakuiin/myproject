# -*- encoding: utf-8 -*-
# main.py -
# Version: 1.0
# Author: Wang Zhuowei wang.zhuowei@foxmail.com
# Last Change: 2020 Apr 10
# License: GPL.v3

"""读取龙空所有书籍
"""

import re
import sys
import asyncio
import aiohttp

from book_info import BookInfo
from sqlite_mgm import SqliteDbManager

# pylint: disable=invalid-name, no-else-return, no-else-break

debug_flag = False

async def fetch(client, url):
    """异步获取网页信息"""
    async with client.get(url) as resp:
        assert resp.status == 200
        return await resp.text()

async def main():
    """执行主循环"""
    base_url = 'http://www.yousuu.com/book'
    db_name = 'library.db'
    SqliteDbManager.create(name=db_name, schema=BookInfo.CREATE_LIBRARY_SQL)
    mgm = SqliteDbManager(db_name)
    with mgm as db:
        async with aiohttp.ClientSession() as client:
            for i in range(100):
                url = '{}/{}'.format(base_url, i)
                print('handleing {} ...'.format(url))
                await asyncio.sleep(1)
                html = await fetch(client, url)
                text = html.splitlines()[-1]
                if debug_flag:
                    print(text)
                save_res(db, text)

def save_res(db, text):
    """打印结果"""
    values = []
    for k, p in BookInfo.PATTERN.items():
        p = re.compile(p)
        res = p.search(text)
        if not res:
            break
        else:
            values.append(res.group(k))
    else:
        print(values)
        db.execute(BookInfo.INSERT_SQL, (None, ) + tuple(values))


if __name__ == '__main__':
    if '-d' in sys.argv[1:]:
        print('--debug enable--')
        debug_flag = True
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
