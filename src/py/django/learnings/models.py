"""由Django自动生成的模型py文件

处理应用中的数据
"""

from django.db import models
from django.contrib.auth.models import User

class Topic(models.Model):
    """用户学习的主题"""
    text = models.CharField(max_length=200)
    date_added = models.DateTimeField(auto_now_add=True)
    owner = models.ForeignKey(User)

    def __str__(self):
        return self.text

class Entry(models.Model):
    """学到的关于某个主题的具体知识"""
    topic = models.ForeignKey(Topic, on_delete=models.CASCADE)
    text = models.TextField()
    date_added = models.DateTimeField(auto_now_add=True)

    class Meta:
        """存储模型的额外信息"""
        verbose_name_plural = 'entries'

    def __str__(self):
        ret = ''
        if len(self.text) <= 50:
            ret = self.text
        else:
            ret = self.text[:50] + '...'
        return ret
