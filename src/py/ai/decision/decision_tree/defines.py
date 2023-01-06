"""决策树基础对象定义
"""

import random


class DecisionTreeNode(object):
    """决策树节点"""
    def make_decision(self):
        """做决策

        @return:
        """
        pass


class Action(DecisionTreeNode):
    """决策树动作

    对应一个实际的行动, 一般是叶子
    """
    def make_decision(self):
        return None


class Decision(DecisionTreeNode):
    """决策
    """
    def __init__(self, true_node: DecisionTreeNode, false_node: DecisionTreeNode):
        self._true_node = true_node  # 满足条件节点
        self._false_node = false_node  # 不满足条件节点

    def make_decision(self):
        branch = self.get_branch()
        return branch.make_decision()

    def get_branch(self) -> DecisionTreeNode:
        """返回分支

        @return:
        """
        if self.test_value():
            return self._true_node
        else:
            return self._false_node

    def test_value(self) -> bool:
        """测试值

        @return:
        """
        return True


class RandomDecision(Decision):
    """随机决策
    """
    def __init__(self, true_node: DecisionTreeNode, false_node: DecisionTreeNode):
        super(RandomDecision, self).__init__(true_node, false_node)
        self._cur_decision = True  # 当前决策

    def test_value(self) -> bool:
        if self.is_need_update_decision():
            self._cur_decision = self.random_boolean()
        return self._cur_decision

    def random_boolean(self) -> bool:
        return bool(random.randint(0, 1))

    def is_need_update_decision(self) -> bool:
        return True