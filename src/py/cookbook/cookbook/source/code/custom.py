"""自定义函数以及类"""


import re
import heapq
import collections
import datetime
import calendar


class PriorityQueue:
    """以heapq模块为基础实现的优先队列"""

    def __init__(self):
        self._queue = []    # 存储数据的容器
        self._index = 0     # 当两个item优先级相等时, 根据push次序弹出元素

    def push(self, item, priority):
        """根据优先级priority存储item"""
        heapq.heappush(self._queue, (-priority, self._index, item))
        self._index += 1

    def pop(self):
        """弹出优先级最高的元素"""
        return heapq.heappop(self._queue)[2]


def dedupe(items, key=None):
    """移除序列重复项并保持其有序[yield]"""
    seen = set()
    for item in items:
        val = item if key is None else key(item)
        if val not in seen:
            yield item
            seen.add(val)


class ExpressionEvaluator:
    """简单表达式求值器, 实现了简单的递归下降解析功能"""

    NUM = r'(?P<NUM>\d+)'
    PLUS = r'(?P<PLUS>\+)'
    MINUS = r'(?P<MINUS>-)'
    TIMES = r'(?P<TIMES>\*)'
    DIVIDE = r'(?P<DIVIDE>/)'
    LPAREN = r'(?P<LPAREN>\()'
    RPAREN = r'(?P<RPAREN>\))'
    WS = r'(?P<WS>\s+)'

    Token = collections.namedtuple('Token', 'type value')
    master_pat = re.compile('|'.join([NUM, PLUS, MINUS, TIMES, DIVIDE, WS,
                                      LPAREN, RPAREN]))

    def parse(self, text):
        """解析输入的表达式, 并返回结果"""
        self.tokens = self._generate_tokens(text)
        self.tok = None
        self.next_tok = None
        self._advance()
        return self.expr()

    def _generate_tokens(self, text):
        """解析输入表达式中每个元素对应的类型"""
        scanner = self.master_pat.scanner(text)
        for mat in iter(scanner.match, None):
            tok = self.Token(mat.lastgroup, mat.group())
            if tok.type != 'WS':
                yield tok

    def _advance(self):
        """向前处理一个字符"""
        self.tok, self.next_tok = self.next_tok, next(self.tokens, None)

    def _accept(self, toktype):
        """如果匹配标记类型则消耗一个标记"""
        if self.next_tok and self.next_tok.type == toktype:
            self._advance()
            return True
        else:
            return False

    def _expect(self, toktype):
        """如果匹配则消耗一个标记, 否则抛出异常"""
        if not self._accept(toktype):
            raise SyntaxError('Expected ' + toktype)

    def expr(self):
        "expression ::= term { ('+'|'-') term }*"

        exprval = self.term() # 这里是下降部分, 委托给另一个方法进行计算
        while self._accept('PLUS') or self._accept('MINUS'):
            op = self.tok.type
            right = self.term()
            if op == 'PLUS':
                exprval += right
            elif op == 'MINUS':
                exprval -= right
        return exprval

    def term(self):
        """term ::= factor { ('*'|'/') factor }*"""

        termval = self.factor()
        while self._accept('TIMES') or self._accept('DIVIDE'):
            op = self.tok.type
            right = self.factor()
            if op == 'TIMES':
                termval *= right
            elif op == 'DIVEDE':
                termval /= right
        return termval

    def factor(self):
        """factor ::= NUM | ( expr )"""
        if self._accept('NUM'):
            return int(self.tok.value)
        elif self._accept('LPAREN'):
            exprval = self.expr() # 这里是递归部分
            self._expect('RPAREN')
            return exprval
        else:
            raise SyntaxError('Expected NUMBER or "("')


class DatetimeUtil:
    """日期相关实用函数的集合"""

    @classmethod
    def get_previous_by_day(cls, dayname, start_date=None):
        """查找从start_date之前的上一个某一天"""
        WEEKDAYS = ['Monday', 'Tuesday', 'Wednessay', 'Thusday', 'Friday',
                    'Saturday', 'Sunday']
        if start_date is None:
            start_date = datetime.datetime.today()
        day_num = start_date.weekday()
        day_num_target = WEEKDAYS.index(dayname)
        days_ago = (7 + day_num - day_num_target) % 7
        if days_ago == 0:
            days_ago = 7
        return start_date - datetime.timedelta(days=days_ago)

    @classmethod
    def get_month_range(cls, start_date=None):
        """获得某个日期所在月的上下界"""
        if start_date is None:
            start_date = datetime.datetime.today()
        start_date = start_date.replace(day=1)
        _, days_in_month = calendar.monthrange(start_date.year,
                                               start_date.month)
        end_date = start_date + datetime.timedelta(days=days_in_month)
        return (start_date, end_date)


class LineHistory:
    """一个带有历史信息的按行迭代器"""

    def __init__(self, lines, his_len=3):
        self.lines = lines
        self.history = collections.deque(maxlen=his_len)

    def __iter__(self):
        for lineno, line in enumerate(self.lines, 1):
            self.history.append((lineno, line))
            yield line

    def clear(self):
        self.history.clear()
