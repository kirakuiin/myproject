"""表单管理"""

from django import forms

from .models import Topic, Entry

class TopicForm(forms.ModelForm):
    """主题表单"""
    class Meta:
        """根据Topic生成表单"""
        model = Topic
        fields = ['text']
        labels = {'text': 'Topic name:'}

class EntryForm(forms.ModelForm):
    """条目表单"""
    class Meta:
        """根据Entry生成表单"""
        model = Entry
        fields = ['text']
        labels = {'text': 'Content:'}
        widgets = {'text': forms.Textarea(attrs={'cols': 80})}
