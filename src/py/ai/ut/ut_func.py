"""单元测试辅助函数
"""

import unittest


class TestCase(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        print('========== {} begin =========='.format(cls.__name__))

    @classmethod
    def tearDownClass(cls) -> None:
        print('========== {} end =========='.format(cls.__name__))


def run_dir(dir: str = './', pattern: str = 'ut_*.py'):
    """运行指定目录下的所有ut

    @param dir: 目录
    @param pattern: 匹配文件格式
    @return:
    """
    loader = unittest.TestLoader()
    discover = loader.discover(dir, pattern)
    runner = unittest.TextTestRunner()
    runner.run(discover)