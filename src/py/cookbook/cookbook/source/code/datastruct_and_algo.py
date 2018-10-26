import custom


def sequence_unpacking():
    """任何包含N个元素序列都可以拆分为<=N个对象

    >>> seq = [1, 2, 3, 4]
    >>> first, second, third, forth = seq
    >>> print(first, second, third, forth)
    1 2 3 4
    >>> first, *mid, last = seq
    >>> print(first, mid, last)
    1 [2, 3] 4
    """
    pass


def save_last_n_elements():
    """保留最后N个元素

    >>> from collections import deque
    >>> history = deque(maxlen=3)
    >>> for i in range(10):
    ...     history.append(i)
    >>> print(history)
    deque([7, 8, 9], maxlen=3)
    """
    pass


def find_n_maximum_or_minimum_elements():
    """找到最大或最小的N个元素

    >>> from heapq import nlargest, nsmallest
    >>> seq = [i for i in range(10)]
    >>> print(nlargest(3, seq))
    [9, 8, 7]
    >>> print(nsmallest(3, seq, key=lambda x: -x))
    [9, 8, 7]
    """
    pass


def user_define_priority_queue():
    """实现优先队列

    >>> from custom import PriorityQueue
    >>> que = PriorityQueue()
    >>> que.push(1, 1)
    >>> que.push(2, 2)
    >>> print(que.pop())
    2
    """
    pass


def map_key_to_multi_value():
    """将一个键映射到多个值上

    >>> from collections import defaultdict
    >>> d = defaultdict(list)
    >>> d['key'].append(1)
    >>> d['key'].append(2)
    >>> print(d['key'])
    [1, 2]
    """
    pass


def keep_dict_in_order():
    """让字典保持按照插入顺序保持有序

    >>> from collections import OrderedDict
    >>> d = OrderedDict()
    >>> d['foo'] = 1
    >>> d['bar'] = 2
    >>> d['spam'] = 3
    >>> d['qrok'] = 4
    >>> for key in d:
    ...     print(key, end=',')
    foo,bar,spam,qrok,
    """
    pass


def calc_about_dict():
    """与字典相关的计算

    >>> prices = {
    ... 'ACME': 45.23,
    ... 'AAPL': 612.78
    ... }
    >>> min_key = min(prices, key=lambda k: prices[k]) #  最小值以及关联的键
    >>> print('{}: {}'.format(min_key, prices[min_key]))
    ACME: 45.23
    >>> min(zip(prices.values(), prices.keys())) # 更优雅的写法
    (45.23, 'ACME')
    """
    pass


def looking_for_same_in_dict():
    """在两个字典里寻找相同点

    >>> a = {'x': 1, 'y': 2, 'z': 3}
    >>> b = {'x': 4, 'y': 2, 'w': 4}
    >>> sorted(a.keys() & b.keys()) # dict.keys返回对象支持集合操作
    ['x', 'y']
    >>> sorted(a.items() - b.items()) # dict.items同理
    [('x', 1), ('z', 3)]
    """
    pass


def remove_duplicates_from_sequence_and_keep_them_in_order():
    """移除序列里的重复项并保持其有序

    >>> from custom import dedupe
    >>> a = [1, 5, 2, 1, 9, 1, 5, 0]
    >>> list(dedupe(a))
    [1, 5, 2, 9, 0]
    """
    pass


def name_the_slice():
    """对切片命名

    >>> record = '......100...200'
    >>> SHARES = slice(6,9)
    >>> PRICES = slice(12,15)
    >>> print('shares={},price={}'.format(record[SHARES], record[PRICES]))
    shares=100,price=200
    >>> s = slice(1, 100, 2)
    >>> print(s.indices(len(record))) # 根据序列长度返回合适的切片
    (1, 15, 2)
    """
    pass


def find_the_most_frequently_occurring_elements():
    """找出序列里出现次数最多的元素

    >>> from collections import Counter
    >>> words = [1, 2, 1, 3, 1, 5, 6, 5, 5, 2, 1]
    >>> words_counts = Counter(words)
    >>> words_counts.most_common(3)
    [(1, 4), (5, 3), (2, 2)]
    >>> words_counts + words_counts # Counter支持算术运算
    Counter({1: 8, 5: 6, 2: 4, 3: 2, 6: 2})
    """
    pass


def sorted_the_dict_by_public_key():
    """通过公共键对字典排序

    >>> from operator import itemgetter
    >>> data = [
    ...     {'name': 'a', 'size': 2},
    ...     {'name': 'b', 'size': 1}
    ... ]
    >>> # 任何实现了__getitem__()的对象均可以使用operator.itemgetter
    >>> rows_by_name = sorted(data, key=itemgetter('name'))
    >>> rows_by_size = sorted(data, key=itemgetter('size'))
    >>> print(rows_by_name)
    [{'name': 'a', 'size': 2}, {'name': 'b', 'size': 1}]
    >>> print(rows_by_size)
    [{'name': 'b', 'size': 1}, {'name': 'a', 'size': 2}]
    """
    pass


def sorting_object_that_dont_support_native_comparsion():
    """排序不支持原生比较的对象

    >>> from operator import attrgetter
    >>> class NonComp:
    ...     def __init__(self, num):
    ...         self.num = num
    ...         self.idx = -num
    >>> l = [NonComp(2), NonComp(1)]
    >>> l.sort()
    Traceback (most recent call last):
    ...
    TypeError: '>' not supported between instances of 'NonComp' and 'NonComp'
    >>> l.sort(key=attrgetter('num', 'idx')) # 和itemgetter的概念类似
    """
    pass


def group_records_by_a_field():
    """通过某个字段将记录分组

    >>> from operator import itemgetter
    >>> from itertools import groupby
    >>> dates = [
    ...     {'addr': '/a/b/c', 'date':'2017'},
    ...     {'addr': '/a/b/d', 'date':'2018'},
    ...     {'addr': '/a/b/e', 'date':'2017'},
    ...     {'addr': '/a/b/e', 'date':'2018'},
    ...     {'addr': '/a/b/e', 'date':'2017'},
    ... ]
    >>> dates.sort(key=itemgetter('date')) # 使用groupby要先排序, 因为他根据连续的元素分组
    >>> for date, items in groupby(dates, itemgetter('date')):
    ...     print(date, list(items))
    2017 [{'addr': '/a/b/c', 'date': '2017'}, {'addr': '/a/b/e', 'date': '2017'}, {'addr': '/a/b/e', 'date': '2017'}]
    2018 [{'addr': '/a/b/d', 'date': '2018'}, {'addr': '/a/b/e', 'date': '2018'}]
    """
    pass


def filter_sequence_elements():
    """过滤序列元素

    >>> from itertools import compress
    >>> mylist = [0, 1, 3, 5, 7, 9]
    >>> [n for n in mylist if n % 2 == 0]
    [0]
    >>> list(filter(lambda n: n % 2 == 0, mylist))
    [0]
    >>> even = [n % 2 == 0 for n in mylist]
    >>> even
    [True, False, False, False, False, False]
    >>> list(compress(mylist, even))
    [0]
    """
    pass


def extract_the_subset_from_a_dictionary():
    """从字典中提取子集

    >>> prices = {'a': 1, 'b': 2, 'c': 3}
    >>> {k: v for k, v in prices.items() if v > 2}
    {'c': 3}
    """
    pass


def map_name_to_elements_of_a_sequence():
    """将名称映射到序列的元素中

    >>> from collections import namedtuple
    >>> Position = namedtuple('Position', 'longitude latitude')
    >>> home = Position(100.15, 223.23)
    >>> home.longitude
    100.15
    >>> home._fields
    ('longitude', 'latitude')
    >>> home._asdict()
    OrderedDict([('longitude', 100.15), ('latitude', 223.23)])
    >>> home._make([10, 20])
    Position(longitude=10, latitude=20)
    >>> home._replace(longitude=150, latitude=30)
    Position(longitude=150, latitude=30)
    """
    pass


def convert_and_transform_data_at_same_time():
    """同时对数据进行转换和换算

    >>> sum(i**2 for i in range(5)) # 使用生成器而不是列表推导
    30
    """
    pass


def merge_multimaps_into_a_single_map():
    """将多个映射合并为单个映射

    >>> from collections import ChainMap
    >>> a = {'x': 1, 'y': 2}
    >>> b = {'y': 3, 'z': 4}
    >>> c = ChainMap(a, b)
    >>> c
    ChainMap({'x': 1, 'y': 2}, {'y': 3, 'z': 4})
    >>> c['y'] = 4
    >>> a, b
    ({'x': 1, 'y': 4}, {'y': 3, 'z': 4})
    >>> c = c.new_child()
    >>> c['x'] = 2
    >>> c
    ChainMap({'x': 2}, {'x': 1, 'y': 4}, {'y': 3, 'z': 4})
    >>> c.parents
    ChainMap({'x': 1, 'y': 4}, {'y': 3, 'z': 4})
    """
