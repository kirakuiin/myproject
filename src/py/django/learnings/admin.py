"""管理应用的模型注册"""

from django.contrib import admin

# Register your models here.
from learnings.models import Topic, Entry

# 注册自定义模型
admin.site.register(Topic)
admin.site.register(Entry)
