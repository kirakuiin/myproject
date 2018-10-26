"""定义learnings的URL模式"""

from django.conf.urls import url

from . import views

# pylint: disable=invalid-name
urlpatterns = [
    # 主页
    url(r'^$', views.index, name='index'),
    # topics
    url(r'^topics/$', views.topics, name='topics'),
    # topic
    url(r'^topics/(?P<topic_id>\d+)/$', views.topic, name='stopic'),
    # new_topic
    url(r'^new_topic/$', views.new_topic, name='new_topic'),
    # new_entry
    url(r'^(?P<topic_id>\d+)/new_entry/$', views.new_entry, name='new_entry'),
    # edit_entry
    url(r'^edit_entry/(?P<entry_id>\d+)/$', views.edit_entry, name='edit_entry')
]
