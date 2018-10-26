def manual_travelsal_iterator():
    """手动遍历迭代器

    >>> items = [1, 2]
    >>> it = iter(items) # Invokes items.__iter__()
    >>> next(it) # Invokes it.__next__()
    1
    >>> next(it)
    2
    >>> next(it) #doctest +ELLIPSIS
    Traceback (most recent call last):
    ...
    StopIteration
    """
    pass


def proxy_iteration():
    """代理迭代

    >>> class Node:
    ...     def __init__(self):
    ...         self.array = [1,2,3]
    ...     def __iter__(self):
    ...         return iter(self.array)
    >>> root = Node()
    >>> for i in root:
    ...     print(i)
    1
    2
    3
    """
    pass


def create_a_new_iteration_pattern_by_using_the_generator():
    """使用生成器创建新的迭代模式

    >>> def countdown():
    ...     yield 3
    ...     yield 2
    ...     yield 1
    ...     return 'OK'
    >>> c = countdown() # Create the generator
    >>> next(c) # yield 3
    3
    >>> next(c) # yield 2
    2
    >>> next(c) # yield 1
    1
    >>> # return 'OK'
    >>> next(c) #doctest +ELLIPSIS
    Traceback (most recent call last):
    ...
    StopIteration: OK
    """
    pass


def reverse_iteration():
    """反向迭代

    >>> a = [1, 2, 3]
    >>> for x in reversed(a):
    ...     print(x, end=',')
    3,2,1,
    >>> class Countdown:
    ...     def __init__(self):
    ...         self.array = [1, 2, 3]
    ...     def __reversed__(self): # 自己实现此接口可以提高性能
    ...         for x in self.array[::-1]:
    ...             yield x
    >>> c = Countdown()
    >>> for i in reversed(c):
    ...     print(i, end=',')
    3,2,1,
    """
    pass


def generator_function_with_external_state():
    """带有外部状态的生成器函数

    >>> from custom import LineHistory
    >>> lines = LineHistory([1, 2, 3, 4, 5])
    >>> for line in lines:
    ...     pass
    >>> print(lines.history)
    deque([(3, 3), (4, 4), (5, 5)], maxlen=3)
    """
    pass


def iterator_slice():
    """迭代器切片

    >>> from itertools import islice
    >>> def count(n):
    ...     while True:
    ...         yield n
    ...         n += 1
    >>> for x in islice(count(0), 10, 12):
    ...     print(x)
    10
    11
    """
    pass


def skip_the_beginning_of_an_iterable_object():
    """跳过可迭代对象的开始部分

    >>> from itertools import dropwhile
    >>> arr = [1, 2, 1]
    >>> for x in dropwhile(lambda x: x == 1, arr): # 遇到第一个非1元素停止过滤
    ...     print(x)
    2
    1
    """
    pass


def combination_and_permutation_of_iteration():
    """排列组合的迭代

    >>> from itertools import (permutations, combinations,
    ...                        combinations_with_replacement)
    >>> items = [1, 2, 3]
    >>> for c in combinations(items, 2): # 从3个元素中取两个组合
    ...     print(c)
    (1, 2)
    (1, 3)
    (2, 3)
    >>> for c in permutations(items, 2): # 从3个元素中取两个排列
    ...     print(c)
    (1, 2)
    (1, 3)
    (2, 1)
    (2, 3)
    (3, 1)
    (3, 2)
    >>> for c in combinations_with_replacement(items, 2):
    ...     print(c)
    (1, 1)
    (1, 2)
    (1, 3)
    (2, 2)
    (2, 3)
    (3, 3)
    """
    pass


def iterate_index_value_on_sequence():
    """在序列上迭代索引值

    >>> my_list = [1, 2, 3]
    >>> for idx, val in enumerate(my_list):
    ...     print((idx, val))
    (0, 1)
    (1, 2)
    (2, 3)
    >>> for idx, val in enumerate(my_list, 1): # 指定开始值
    ...     print((idx, val))
    (1, 1)
    (2, 2)
    (3, 3)
    """
    pass


def iterating_multiple_sequences_simultaneously():
    """同时迭代多个序列

    >>> a = [1, 2, 3]
    >>> for x, y, z in zip(a, a + a, a):
    ...     print(x, y, z)
    1 1 1
    2 2 2
    3 3 3
    >>> from itertools import zip_longest
    >>> for x, y in zip_longest(a, a + a, fillvalue=0):
    ...     print(x, y)
    1 1
    2 2
    3 3
    0 1
    0 2
    0 3
    """
    pass


def iterating_on_different_collections():
    """在不同的集合上迭代

    >>> from itertools import chain
    >>> a = [1, 2, 3]
    >>> b = ('a', 'b', 'c')
    >>> for item in chain(a, b):
    ...     print(item, end=',')
    1,2,3,a,b,c,
    """
    pass
