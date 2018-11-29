# -*- encoding: utf-8 -*-
# author: wang.zhuowei@foxmail.com
# date: Oct 29, 2018
# license: GPL.v3

"""Google Translate功能的封装

通过向网站translate.google.cn发送get请求，根据返回的json结果获得翻译的文本
实现时依赖javascript, 因此环境中必须有js的解释器
"""

# pylint: disable=invalid-name, access-member-before-definition

import re
import collections

import execjs
import requests


__all__ = ['TranslateRequest', 'GoogleTranslate']


class TranslateRequest:
    """翻译配置请求

    用来每次翻译请求的参数进行包装, 默认从自动识别->简体中文

    >>> request = TranslateRequest('hello world')
    >>> request
    TranslateRequest(query='hello world', s_lang='auto', t_lang='zh-CN')
    >>> query, *_ = request
    >>> query
    'hello world'
    >>> request.s_lang = 'en'
    >>> request
    TranslateRequest(query='hello world', s_lang='en', t_lang='zh-CN')

    """

    DEFAULT_S_LANG = 'auto'    # 自动识别
    DEFAULT_T_LANG = 'zh-CN'   # 简体中文

    def __init__(self, query: str,
                 s_lang=DEFAULT_S_LANG, t_lang=DEFAULT_T_LANG):
        self.query = query      # 待翻译文本
        self.s_lang = s_lang    # 源语言
        self.t_lang = t_lang    # 目标语言

    def __repr__(self):
        cls_name = type(self).__name__
        query = self.query
        if len(query) > 30:
            query = query[:27] + '...'
        return "{}(query='{}', s_lang='{}', t_lang='{}')".format(
            cls_name, query, self.s_lang, self.t_lang)

    def __iter__(self):
        yield from (self.query, self.s_lang, self.t_lang)


class GoogleTranslate:
    """GoogleTranslate翻译工具

    支持Google翻译所支持的部分语言, 如下所示:
        - 简体中文[zh-CN]
        - 繁体中文[zh-TW]
        - 英语[en]
        - 日语[ja]
        - 德语[de]
        - 法语[fr]
        - 俄语[ru]
        - 西班牙语[es]

    >>> api = GoogleTranslate()
    >>> result = api.translate(TranslateRequest('hello world'))
    >>> result.trans_text       # 翻译出来的文本
    '你好，世界'
    >>> result.t_transcription  # 目标发音
    'Nǐ hǎo, shìjiè'
    >>> GoogleTranslate.is_valid_lang('ja') # 判断语言缩写是否有效
    True

    """

    GOOGLE_TRANSLATE_URL = 'https://translate.google.cn'

    _SUPPORT_LANG = ('zh-CN', 'zh-TW', 'en', 'ja', 'ru', 'fr', 'es', 'de')
    _GOOGLE_TRANSLATE_SINGLE = 'https://translate.google.cn/translate_a/single'
    _HTTPS_OK = 200

    @classmethod
    def is_valid_lang(cls, lang: str) -> bool:
        """判断语言代码是否有效

        lang: 语言缩写
        return: 缩写是否有效

        >>> GoogleTranslate.is_valid_lang('de') # 德语
        True
        >>> GoogleTranslate.is_valid_lang('ko') # 韩语(暂不支持)
        False

        """
        return lang in cls._SUPPORT_LANG

    def __init__(self):
        with open('google_tk.js') as fp:
            js_code = fp.read()
            self._js_ctx = execjs.compile(js_code)
        self._TKK = self._get_TKK_value()

    def translate(self, request: TranslateRequest) -> list:
        """将内容从源语言翻译为目标语言

        request: 翻译对象, 包含源语言, 目标语言等信息
        return: TranslateResult对象

        >>> api = GoogleTranslate()
        >>> request = TranslateRequest('hello world', 'en', 'ja')
        >>> api.translate(request)
        TranslateResult(target_lang='ja', result=[..., 'hello world', ...])

        """
        get_param = self._construct_request(request)
        result = requests.get(self._GOOGLE_TRANSLATE_SINGLE, get_param)

        if result.status_code == self._HTTPS_OK:
            trans_ins = TranslateResult(request.t_lang, result.json())
        else:
            trans_ins = TranslateResult(request.t_lang)
        return trans_ins

    def __repr__(self):
        cls_name = type(self).__name__
        return '{}()'.format(cls_name)

    def _construct_request(self, request: TranslateRequest) -> dict:
        """构造向google翻译网站发送的get请求所需要的参数"""
        token = self._get_tk_value(request.query)
        request_dict = {
            'client': 't',
            'sl': request.s_lang,     # 源语言
            'tl': request.t_lang,     # 目标语言
            'ie': 'UTF-8',      # 输入编码
            'oe': 'UTF-8',      # 输出编码
            'tk': token,        # 校验值
            'q': request.query, # 被翻译的字符串
            'dt': [             # 翻译选项
                't', 'rm', 'bd',    # 翻译字符串, 带音标, 字典含义(单词)
                'md', 'ss',         # 单词定义, 同义词
                #'at', 'rw', 'ex',  # 备选翻译, 列表, 例子
            ],
        }
        return request_dict

    def _get_tk_value(self, translate_text: str) -> str:
        """根据翻译内容获得google传输get请求里的tk的值"""
        return self._js_ctx.call('tk', translate_text, self._TKK)

    def _get_TKK_value(self) -> str:
        """在Google翻译的页面里获得TKK的值"""
        text = requests.get(self.GOOGLE_TRANSLATE_URL).text
        PATTERN = r"tkk:'(?P<TKK>.+?)'"
        result = re.search(PATTERN, text)
        return result.group('TKK') if result else ''


class TranslateResult:
    """存储翻译结果

    翻译结果的原始信息为一个数组, 构成如下:
    [
        [翻译文本信息],
        [单词字典信息],
        [源语言类型],
        null,
        null,
        [作用未知],
        0.xxx,  # 可能是某种概率
        null,
        [作用未知],  # 后面的5个, 如果翻译的是句子有可能不存在
        null,
        null,
        [同义词],
        [定义],
        [例句],
    ]

    >>> result = GoogleTranslate().translate(TranslateRequest('hello'))
    >>> result.trans_text
    '你好'
    >>> result.dictionary #doctest: +ELLIPSIS
    [WordInfo(type='感叹词', text='你好!', synonym='Hello!,Hi!,Hallo!'), ...
    >>> result.s_transcription  # 源语言音标
    'heˈlō,həˈlō'

    """

    WordInfo = collections.namedtuple('WordInfo', 'type text synonym')

    def __init__(self, target_lang, result=None):
        self._result = result
        self._t_lang = target_lang

    def __repr__(self):
        cls_name = type(self).__name__
        origin_text = self.origin_text
        if len(origin_text) > 30:
            origin_text = origin_text[:27] + '...'
        result = "[..., '{}', ...]".format(origin_text)
        return "{}(target_lang='{}', result={})".format(cls_name,
                                                        self.t_lang,
                                                        result)


    def __iter__(self):
        yield from (self.origin_text, self.s_lang, self.t_lang, self.trans_text,
                    ';'.join(self.dictionary))


    @property
    def trans_text(self) -> str:
        """翻译文本部分"""
        result = ''
        try:
            text = self._result[0]
            index = -1 if len(text) > 1 else 2 # 处理没有音标项的情况
            result = ''.join([trans[0] for trans in text[:index]])
        except TypeError:
            pass
        return result

    @property
    def origin_text(self) -> str:
        """原始文本部分"""
        result = ''
        try:
            text = self._result[0]
            index = -1 if len(text) > 1 else 2
            result = ''.join([trans[1] for trans in text[:index]])
        except (IndexError, TypeError):
            pass
        return result

    @property
    def s_transcription(self) -> str:
        """获得源语言的音标(如果有的话)"""
        result = ''
        try:
            if len(self._result[0]) > 1:
                transcription = self._result[0][-1]
                result = transcription[3]
        except (IndexError, TypeError):
            pass
        return result

    @property
    def t_transcription(self) -> str:
        """获得目标语言的音标"""
        result = ''
        try:
            if len(self._result[0]) > 1:
                transcription = self._result[0][-1]
                result = transcription[2]
        except (IndexError, TypeError):
            pass
        return result

    @property
    def s_lang(self) -> str:
        """返回源语言的类型"""
        result = ''
        try:
            result = self._result[2]
        except (TypeError, IndexError):
            pass
        return result

    @property
    def t_lang(self) -> str:
        """返回目标语言的类型"""
        return self._t_lang

    @property
    def dictionary(self) -> list:
        """返回字典含义(如果是单词的话)"""
        results = []
        try:
            dicts = self._result[1]
            for items in dicts:
                for words in items[2]:
                    word = self.WordInfo(items[0], words[0],
                                         ','.join(words[1]))
                    results.append(word)
        except (IndexError, TypeError):
            pass
        return results

    @property
    def definition(self) -> list:
        """返回单词的定义(如果是单词的话)"""
        results = []
        try:
            word_defs = self._result[12]
            for items in word_defs:
                for words in items[1]:
                    synonym = self._get_definition_synonym(items[0], words[1])
                    word = self.WordInfo(items[0], words[0],
                                         ','.join(synonym))
                    results.append(word)
        except (IndexError, TypeError):
            pass
        return results

    def _get_definition_synonym(self, word_type: str, word_id: str) -> list:
        """根据词性和id寻找该定义的同义词"""
        results = []
        word_synonym = self._result[11]
        for items in word_synonym:
            if word_type == items[0]:
                for words in items[1]:
                    if word_id == words[1]:
                        results = words[0]
                        break
        return results


if __name__ == '__main__':
    req = TranslateRequest('hello world', 'en', 'fr')
    trans = GoogleTranslate()
    ret = trans.translate(req)
