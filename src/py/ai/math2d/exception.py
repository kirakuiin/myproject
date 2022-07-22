"""数学异常
"""

class MathException(Exception):
    pass


class NoSolutionException(MathException):
    def __str__(self):
        return '无解: {}'.format(self.args)