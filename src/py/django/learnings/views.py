"""视图实现模块"""

from django.shortcuts import render, get_object_or_404
from django.http import HttpResponseRedirect, Http404
from django.core.urlresolvers import reverse
from django.contrib.auth.decorators import login_required

from .models import Topic, Entry
from .forms import TopicForm, EntryForm

def _check_topic_owner(a_topic, user) -> bool:
    """检查当前主题的用户是否为user"""
    return a_topic.owner == user

def index(request):
    """学习笔记的主页"""
    return render(request, 'learnings/index.html')


@login_required
def topics(request):
    """显示用户所有的所有主题"""
    topicset = Topic.objects.order_by('date_added').filter(owner=request.user)
    context = {'topics': topicset}
    return render(request, 'learnings/topics.html', context)


@login_required
def topic(request, topic_id):
    """显示单个主题及其条目"""
    single_topic = get_object_or_404(Topic, id=topic_id)
    if not _check_topic_owner(single_topic, request.user):
        raise Http404
    entries = single_topic.entry_set.order_by('-date_added')
    context = {'topic': single_topic, 'entries': entries}
    return render(request, 'learnings/topic.html', context)


@login_required
def new_topic(request):
    """添加新主题"""
    if request.method != 'POST':
        form = TopicForm()
    else:
        form = TopicForm(request.POST)
        if form.is_valid():
            a_topic = form.save(commit=False)
            a_topic.owner = request.user
            a_topic.save()
            return HttpResponseRedirect(reverse('learnings:topics'))

    context = {'form': form}
    return render(request, 'learnings/new_topic.html', context)


@login_required
def new_entry(request, topic_id):
    """在特定主题里添加新条目"""
    a_topic = Topic.objects.get(id=topic_id)
    if not _check_topic_owner(a_topic, request.user):
        raise Http404

    if request.method != 'POST':
        form = EntryForm()
    else:
        form = EntryForm(data=request.POST)
        if form.is_valid():
            a_entry = form.save(commit=False)
            a_entry.topic = a_topic
            a_entry.save()
            return HttpResponseRedirect(reverse('learnings:stopic',
                                                args=[topic_id]))

    context = {'topic': a_topic, 'form': form}
    return render(request, 'learnings/new_entry.html', context)


@login_required
def edit_entry(request, entry_id):
    """编辑存在的条目"""
    a_entry = Entry.objects.get(id=entry_id)
    a_topic = a_entry.topic
    if not _check_topic_owner(a_topic, request.user):
        raise Http404

    if request.method != 'POST':
        form = EntryForm(instance=a_entry)
    else:
        form = EntryForm(instance=a_entry, data=request.POST)
        if form.is_valid():
            form.save()
            return HttpResponseRedirect(reverse('learnings:stopic',
                                                args=[a_topic.id]))

    context = {'entry': a_entry, 'topic': a_topic, 'form': form}
    return render(request, 'learnings/edit_entry.html', context)
