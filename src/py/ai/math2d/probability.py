"""概率相关"""

import random


randint = random.randint  # 随机整数
rand = random.random  # 随机数


def rand_bio(range=1):
    """返回[-range, range)的随机数

    @param scale:
    @return:
    """
    return range*(rand() - rand())
