def split_into_string_for_any_number_separators():
    """针对任意多的分隔符拆分字符串

    >>> import re
    >>> line = 'asdf; afed, fjek,asdf,  foo'
    >>> re.split(r'[,;\s]\s*', line)
    ['asdf', 'afed', 'fjek', 'asdf', 'foo']
    >>> re.split(r'(;|,|\s)\s*', line) # 使用捕获组会将分隔符捕获
    ['asdf', ';', 'afed', ',', 'fjek', ',', 'asdf', ',', 'foo']
    >>> re.split(r'(?:,|;|\s)\s*', line) # 使用?:记号
    ['asdf', 'afed', 'fjek', 'asdf', 'foo']
    """
    pass


def do_text_matching_at_begin_or_end_of_string():
    """在字符串的开头或者结尾处做文本匹配

    >>> filename = 'spam.txt'
    >>> filename.endswith('.txt')
    True
    >>> filename.startswith(('a', 'b')) # 仅接受元组而不是可迭代类型
    False
    """
    pass


def use_shell_wildchars_for_string_matching():
    """利用shell通配符做字符串匹配

    >>> from fnmatch import fnmatch, fnmatchcase
    >>> fnmatchcase('foo.txt', '*.Txt')
    False
    >>> fnmatchcase('*123*', '[*]123[*]') #  用[c]的语法来禁止转义字符c
    True
    """
    pass


def text_pattern_matching_and_lookup():
    """文本模式的匹配和查找

    >>> import re
    >>> text = 'yeah oh my god'
    >>> text == 'yeah'
    False
    >>> text.startswith('yeah')
    True
    >>> text.find('oh')
    5
    >>> re.match(r'\w+', text).group() # 从字符串起始进行匹配, 匹配到一个结果
    'yeah'
    >>> pat = re.compile(r'\w+')
    >>> pat.findall(text) # 在字符串中查找所有符合表达式的结果
    ['yeah', 'oh', 'my', 'god']
    >>> pat = re.compile(r'(\w+)\s(\w+)') # 分组
    >>> m = pat.match(text)
    >>> m.group() # 正则表达式匹配结果
    'yeah oh'
    >>> m.group(1) # 第一个分组
    'yeah'
    >>> m.group(2)
    'oh'
    >>> m.groups() # 全部分组
    ('yeah', 'oh')
    >>> for i in pat.finditer(text): # 迭代式的全部匹配
    ...     print(i.groups())
    ('yeah', 'oh')
    ('my', 'god')
    """
    pass


def find_and_replace_text():
    """查找和替换文本

    >>> import re
    >>> text = 'a b c d'
    >>> text.replace('a', 'e')
    'e b c d'
    >>> re.sub(r'\s', r'-', text)
    'a-b-c-d'
    >>> pat = re.compile(r'(\w)\s*(\w)')
    >>> def callback(m):
    ...     return '{}-{}'.format(m.group(1), m.group(2))
    >>> pat.subn(callback, text)
    ('a-b c-d', 2)
    """
    pass


def replace_text_in_a_case_insensitive_manner():
    """以不区分大小写的方式对文本进行替换

    >>> import re
    >>> text = 'Abcd aBcd'
    >>> re.findall('abcd', text, flags=re.IGNORECASE)
    ['Abcd', 'aBcd']
    >>> re.sub('abcd', 'dcba', text, flags=re.IGNORECASE)
    'dcba dcba'
    >>> def matchcase(word):
    ...     def replace(m):
    ...         text = m.group()
    ...         if text.isupper():
    ...             return word.upper()
    ...         elif text.islower():
    ...             return word.lower()
    ...         elif text[0].isupper():
    ...             return word.capitalize()
    ...         else:
    ...             return word
    ...     return replace
    >>> re.sub(r'(?ai)ABCD', matchcase('dcba'), text)
    'Dcba dcba'
    """
    pass


def define_a_regular_expression_that_implements_the_shortest_match():
    """定义实现最短匹配的正则表达式

    >>> import re
    >>> str_pat = re.compile(r'\"(.*)\"')
    >>> text1 = 'Computer says "no."'
    >>> str_pat.findall(text1)
    ['no.']
    >>> text2 = 'Computer says "no.", "no."'
    >>> str_pat.findall(text2)
    ['no.", "no.']
    >>> str_pat = re.compile(r'\"(.*?)\"')
    >>> str_pat.findall(text2)
    ['no.', 'no.']
    """
    pass


def write_regular_expression_for_multline_mode():
    """编写多行模式的正则表达式

    >>> import re
    >>> comment = re.compile(r'(?s)/\*(.*?)\*/')
    >>> # or re.complie(r'/\*(.*?)\*/', flags=re.DOTALL)
    >>> text = '''/*
    ... this is a multilie
    ... */
    ... '''
    >>> bool(comment.findall(text))
    True
    """
    pass


def unified_representation_of_unicode_text_as_canonical_form():
    """将unicode文本表示为规范形式

    >>> import unicodedata
    >>> # 表示相同字符串, 码位不同
    >>> s1 = 'Spicy Jalape\u00f1o'
    >>> s2 = 'Spicy Jalapen\u0303o'
    >>> s1, s2
    ('Spicy Jalapeño', 'Spicy Jalapeño')
    >>> s1 == s2
    False
    >>> len(s1), len(s2)
    (14, 15)
    >>> unicodedata.normalize('NFC', s1)
    'Spicy Jalapeño'
    >>> unicodedata.normalize('NFC', s2)
    'Spicy Jalapeño'
    >>> len(unicodedata.normalize('NFD', s1))
    15
    >>> ''.join(c for c in unicodedata.normalize('NFD', s2)
    ...         if not unicodedata.combining(c)) # 去掉组合符号
    'Spicy Jalapeno'
    """
    pass


def handleing_unicode_characters_with_regular_expressions():
    """用正则表达式处理Unicode字符

    >>> import re
    >>> num = re.compile('\d+') # 匹配unicode数字
    >>> num.match('123')
    <_sre.SRE_Match object; span=(0, 3), match='123'>
    >>> num.match('\u0661\0662') # re模块对一些unicode字符有一定的处理
    <_sre.SRE_Match object; span=(0, 3), match='١62'>
    >>> pat = re.compile('stra\u00dfe', re.IGNORECASE) # Ignore case
    >>> s = 'straße'
    >>> pat.match(s) # Matches
    <_sre.SRE_Match object; span=(0, 6), match='straße'>
    >>> pat.match(s.upper()) # Doesn't match
    >>> # 推荐使用第三方regex模块来处理unicode字符串
    """
    pass


def remove_unwanted_characters_from_the_string():
    """从字符串中去掉不需要的字符

    >>> s = ' hello world-'
    >>> s.strip()
    'hello world-'
    >>> s.lstrip()
    'hello world-'
    >>> s.rstrip('-')
    ' hello world'
    >>> s.replace(' ', '-')
    '-hello-world-'
    >>> import re
    >>> re.sub(r'\s+|-', '', s)
    'helloworld'
    """
    pass


def text_filtering_and_cleanup():
    """文本过滤和清理

    >>> s = 'python is awesome'
    >>> remap = {
    ...     ord('p'): 'b',
    ...     ord('w'): 'm',
    ...     ord('o'): None,
    ... }
    >>> s.translate(remap)
    'bythn is amesme'
    >>> import unicodedata
    >>> import sys
    >>> # 过滤所有组合字符
    >>> cmb_chars = dict.fromkeys(c for c in range(sys.maxunicode)
    ...                           if unicodedata.combining(chr(c)))
    >>> s = unicodedata.normalize('NFD', s)
    >>> s.translate(cmb_chars)
    'python is awesome'
    >>> # unicode数字转ascii数字
    >>> digitmap = { c: ord('0') + unicodedata.digit(chr(c))
    ...             for c in range(sys.maxunicode)
    ...             if unicodedata.category(chr(c)) == 'Nd'}
    >>> x = '\u0661\u0662'
    >>> x.translate(digitmap)
    '12'
    >>> # 通过忽略非ascii字符来清理
    >>> b = unicodedata.normalize('NFD', s)
    >>> s.encode('ascii', 'ignore').decode('ascii')
    'python is awesome'
    """
    pass


def align_text_strings():
    """对齐文本字符串

    >>> text = 'Hello World'
    >>> text.ljust(20)
    'Hello World         '
    >>> text.rjust(20)
    '         Hello World'
    >>> text.center(20, '*')
    '****Hello World*****'
    >>> format(text, '=^20')
    '====Hello World====='
    >>> format(123.34, '<+20.1f')
    '+123.3              '
    """
    pass


def string_concatenation_and_merging():
    """字符串的连接和合并

    >>> parts = ('a', 'e', 'i', 'o', 'u')
    >>> ''.join(parts)
    'aeiou'
    >>> 'a' + 'e' + 'i' + 'o' + 'u'
    'aeiou'
    >>> txt = 'aei'
    >>> txt += 'ou'
    >>> txt
    'aeiou'
    """
    pass


def interpolate_variable_names_in_strings():
    """给字符串中的变量名做插值处理

    >>> import string
    >>> s = string.Template('$name has $n messages.')
    >>> name, n = 'Guido', 13
    >>> s.substitute(vars())
    'Guido has 13 messages.'
    >>> s = '{name} has {n} messages.'
    >>> s.format(name='Guido', n=13)
    'Guido has 13 messages.'
    >>> s.format_map(vars())
    'Guido has 13 messages.'
    >>> class safesub(dict): # 优雅的处理缺少某个键的情况
    ...     def __missing__(self, key):
    ...         return '{' + key + '}'
    >>> del n
    >>> s.format_map(safesub(vars()))
    'Guido has {n} messages.'
    """
    pass


def reformat_text_with_a_fixed_number_of_columns():
    """以固定的列数重新格式化文本

    >>> s = '''I've been reading books of old. The legend and the myth. Hercules
    ...     and his gold. Achilles and his gift '''
    >>> import textwrap
    >>> print(textwrap.fill(s, 30))
    I've been reading books of
    old. The legend and the myth.
    Hercules     and his gold.
    Achilles and his gift
    """
    pass


def handling_html_and_xml_entities_in_text():
    """在文本中处理html和xml实体

    >>> s = '<tag>text</tag>'
    >>> import html
    >>> print(s)
    <tag>text</tag>
    >>> s = html.escape(s)
    >>> print(s)
    &lt;tag&gt;text&lt;/tag&gt;
    >>> t = '中国'
    >>> t.encode('ascii', errors='xmlcharrefreplace')
    b'&#20013;&#22269;'
    >>> from html.parser import HTMLParser
    >>> p = HTMLParser()
    >>> print(p.unescape(s))
    <tag>text</tag>
    """
    pass


def text_segmentation():
    """文本分词

    >>> text = 'Libra = 23 + 42 * 10'
    >>> import re
    >>> NAME = r'(?P<NAME>[a-zA-Z_][a-zA-Z_0-9]*)'
    >>> NUM = r'(?P<NUM>\d+)'
    >>> PLUS = r'(?P<PLUS>\+)'
    >>> TIMES = r'(?P<TIMES>\*)'
    >>> EQ = r'(?P<EQ>=)'
    >>> WS = r'(?P<WS>\s+)'
    >>> master_pat = re.compile('|'.join((NAME, NUM, PLUS, TIMES, EQ, WS)))
    >>> scanner = master_pat.scanner('Virgo = 3 + 4 * 5')
    >>> for m in iter(scanner.match, None): #doctest: +ELLIPSIS
    ...     print(m.lastgroup, '=', m.group())
    NAME = Virgo
    ...
    NUM = 5
    >>> # 如果一个分词是另一个分词的子串(或者是子模式), 注意每个分词的顺序
    >>>
    >>> PRINT = r'(P?<PRINT>print)' # NAME的子模式, PRINT在NAME前
    >>> GT = r'(P?<GT>>=)' # GT在EQ前
    """
    pass


def write_a_simple_recursive_descent_parser():
    """编写一个简单的递归下降解析器

    >>> from custom import ExpressionEvaluator
    >>> parser = ExpressionEvaluator()
    >>> parser.parse('1*5+2*(3-1)')
    9
    """
    pass


def perform_text_opertions_on_a_byte_string():
    """在字节串上执行文本操作

    >>> data = b'Hello World'
    >>> data[0:5]
    b'Hello'
    >>> data.startswith(b'Hello')
    True
    >>> data.split()
    [b'Hello', b'World']
    >>> # 同样的操作在字节数组上也有效
    >>> data = bytearray(b'Hello World')
    >>> data[0:5]
    bytearray(b'Hello')
    >>> data = b'FOO:BAR,SPAM'
    >>> import re
    >>> re.split(b'[:,]', data)
    [b'FOO', b'BAR', b'SPAM']
    >>> # 如果想对字节串做个格式化操作, 请先对字符串格式化然后编码
    >>> '{} nop'.format('nop').encode('ascii')
    b'nop nop'
    """
    pass
