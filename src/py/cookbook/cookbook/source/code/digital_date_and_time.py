def rounding_the_value():
    """对数值取整

    >>> round(1.23, 1)
    1.2
    >>> round(-1.273, 2)
    -1.27
    >>> round(231, 1)
    231
    >>> round(231, -1) # 十位取整
    230
    """
    pass


def perform_accurate_decimal_calculations():
    """执行精确的小数计算

    >>> a = 4.2
    >>> b = 2.1
    >>> a + b
    6.300000000000001
    >>> a + b == 6.3
    False
    >>> from decimal import Decimal, localcontext
    >>> a = Decimal('4.2')
    >>> b = Decimal('2.1')
    >>> a + b
    Decimal('6.3')
    >>> a + b == Decimal('6.3')
    True
    >>> a, b = Decimal(1.3), Decimal(1.7)
    >>> print(a / b)
    0.7647058823529412225698141713
    >>> with localcontext() as ctx:
    ...     ctx.prec = 3
    ...     print(a / b)
    0.765
    """
    pass


def formating_output_values():
    """对数值做格式化输出

    >>> from decimal import Decimal
    >>> x = 1234.4567
    >>> format(x, '0.2f')
    '1234.46'
    >>> format(x, '^10.1f')
    '  1234.5  '
    >>> format(x, '0,.1f')
    '1,234.5'
    >>> x = Decimal('123.4')
    >>> format(x, '.2e')
    '1.23e+2'
    """
    pass


def interact_with_bin_oct_and_hex():
    """同二进制、八进制和十六进制打交道

    >>> x = 1234
    >>> bin(x)
    '0b10011010010'
    >>> oct(x)
    '0o2322'
    >>> hex(x)
    '0x4d2'
    >>> format(x, 'o')
    '2322'
    >>> x = -1234
    >>> format(x, 'x')
    '-4d2'
    >>> format(x + 2**32, 'x')
    'fffffb2e'
    >>> int('4d2', 16)
    1234
    """
    pass


def packing_and_unpackingj_big_integer_from_strings():
    """从字节串中打包和解包大整数

    >>> # int.to_bytes(len, byteorder, *, signed) int -> bytes
    >>> # int.from_bytes(len, byteorder, *, signed) bytes -> int
    >>> # int.bit_length() get bytes length
    >>> # import struct
    >>> # struct.unpack('>QQ', data) # '>QQ': little endian, unsigned long long
    """
    pass


def complex_operation():
    """复数运算

    >>> a = complex(2, 4)
    >>> b = 3 - 5j
    >>> a
    (2+4j)
    >>> b
    (3-5j)
    >>> a.real
    2.0
    >>> a.imag
    4.0
    >>> a.conjugate()
    (2-4j)
    >>> a * b
    (26+2j)
    >>> import math
    >>> try:
    ...     math.sqrt(-1)
    ... except ValueError as e:
    ...     print(e)
    math domain error
    >>> import cmath
    >>> cmath.sqrt(-1)
    1j
    """
    pass


def handling_infinity_and_nan():
    """处理无穷大和NAN

    >>> a = float('inf')
    >>> b = float('nan')
    >>> a
    inf
    >>> b
    nan
    >>> import math
    >>> math.isinf(a)
    True
    >>> math.isnan(b)
    True
    >>> a + 1
    inf
    >>> 1 / a
    0.0
    >>> a / a
    nan
    >>> b - 1
    nan
    >>> a / a == b
    False
    """
    pass


def calculation_of_fraction():
    """分数的计算

    >>> from fractions import Fraction
    >>> a = Fraction(5, 4)
    >>> b = Fraction(7, 16)
    >>> print(a + b)
    27/16
    >>> c = a * b
    >>> c.numerator
    35
    >>> c.denominator
    64
    >>> float(c)
    0.546875
    >>> c.limit_denominator(4)
    Fraction(1, 2)
    """
    pass


def random_selection():
    """随机选择

    >>> import random
    >>> values = [1, 2, 3, 4, 5]
    >>> v = random.choice(values) # 随机选择一个值
    >>> a = random.sample(values, 3) # 随机选择三个值
    >>> i = random.randint(1, 2) # 产生随机整数
    >>> f = random.random() # 产生随机浮点数
    >>> b = random.getrandbits(200) # 产生200位的随机整数
    >>> random.seed() # 设置生成种子
    """
    pass


def basic_date_and_time_conversion():
    """基本的日期与时间换算

    >>> from datetime import timedelta, datetime
    >>> a = timedelta(days=2, hours=6)
    >>> b = timedelta(hours=4.5)
    >>> c = a + b
    >>> c.days
    2
    >>> c.seconds
    37800
    >>> a = datetime(2012, 2, 28) # 自动处理闰年
    >>> b = datetime(2012, 3, 1)
    >>> (b - a).days
    2
    """
    pass


def calculate_date_of_the_last_friday():
    """计算最后一个周五的日期

    >>> from custom import DatetimeUtil
    >>> from datetime import datetime
    >>> DatetimeUtil.get_previous_by_day('Monday', datetime(2012, 8, 7))
    datetime.datetime(2012, 8, 6, 0, 0)
    """
    pass


def calculate_the_date_range_for_the_current_month():
    """计算当前月份的日期范围

    >>> from custom import DatetimeUtil
    >>> from datetime import datetime, timedelta
    >>> first, last = DatetimeUtil.get_month_range(datetime(2012, 8, 7))
    >>> a_day = timedelta(days=1)
    >>> while first < last: #doctest: +ELLIPSIS
    ...     print(first)
    ...     first += a_day
    2012-08-01 00:00:00
    ...
    2012-08-31 00:00:00
    """
    pass


def convert_string_to_date():
    """字符串转换为日期

    >>> from datetime import datetime
    >>> text = '2012-08-07'
    >>> y = datetime.strptime(text, '%Y-%m-%d')
    >>> print(y)
    2012-08-07 00:00:00
    >>> print(datetime.strftime(y, '%d %m% %Y'))
    07 08 2012
    """
    pass
