// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-28

#ifndef _ALGORITHM_STRUCT_RED_AND_BLACK_TREE_H_
#define _ALGORITHM_STRUCT_RED_AND_BLACK_TREE_H_

#include "include/tree_interface.h"
#include "common/algo_util.h"

namespace algorithm {
namespace advanced_struct {

// 红黑树
// 一种平衡搜索树, 可以保证没有一条路径会比其他的路径长两倍, 因而是近似平衡的
// 对于支持的所有操作, 在最坏的情况下运行时间复杂度Ο(lgn)
// 因为每个节点都有红黑两种颜色, 所以被称为红黑树
// 对于没有孩子或者父亲的节点视为叶节点(外部节点), 其余视为内部节点
// 红黑树满足以下性质:
//   1. 每个节点不是红色就是黑色的
//   2. 根结点和叶节点是黑色的
//   3. 如果一个节点是红色的, 则它的两个字节点都是黑色的
//   4. 对每个节点, 从该节点到其所有后代叶节点的简单路径上, 均包含相同数目的黑色
//      节点(路径不包含该节点本身)
template<typename T>
class RedBlackTree : public TreeInterface<T> {
  public:
    RedBlackTree() : _nil_node{new ColorTreeNode<T>()}, _root(nullptr) {
        _root = _nil_node;
    }
    virtual ~RedBlackTree() {
        DeleteTree(_root);
        if (_nil_node) {
            delete _nil_node;
            _nil_node = nullptr;
        }
    }

    // pseudo code
    // TREE-SEARCH(x, k)
    //   if x == NIL-NODE
    //     return NIL
    //   if k == x.key
    //     return x
    //   elseif k < x.key
    //     return TREE-SEARCH(x.left, k)
    //   else
    //     return TREE-SEARCH(x.right. k)
    TreeNode<T>* Search(const T& key) const override {
        TreeNode<T>* node = _root;
        while (_nil_node != node) {
            if (key == node->value) {
                break;
            } else if (key < node->value) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        if (_nil_node == node) {
            return nullptr;
        } else {
            return node;
        }
    }

    TreeNode<T>* Minimum() const override {
        return InnerMinimum(_root);
    }

    TreeNode<T>* Maximum() const override {
        return InnerMaximum(_root);
    }

    // pseudo code
    // TREE-SUCCESSOR(x)
    //   if x.right != NIL-NODE
    //     return TREE-MINIMUM(x.right)
    //   y = x.parent
    //   while y.right == x and y != NIL-NODE
    //     x = y
    //     y = y.parent
    //   return y
    TreeNode<T>* Successor(const TreeNode<T>* node) const override {
        assert(nullptr != node);
        if (_nil_node != node->right) {
            return InnerMinimum(node->right);
        } else {
            TreeNode<T>* parent = node->parent;
            while ((node == parent->right) && (_nil_node != parent)) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }

    // pseudo code
    // TREE-PREDECESSOR(x)
    //   if x.left != NIL-NODE
    //     return TREE-MAXIMUM(x.left)
    //   y = x.parent
    //   while y.left == x and y != NIL-NODE
    //     x = y
    //     y = y.parent
    //   return y
    TreeNode<T>* Predecessor(const TreeNode<T>* node) const override {
        assert(nullptr != node);
        if (_nil_node != node->left) {
            return InnerMaximum(node->left);
        } else {
            TreeNode<T>* parent = node->parent;
            while ((node == parent->left) && (_nil_node != parent)) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }

    // pseudo code
    // RB-INSERT(T, z)
    //   y = NIL-NODE
    //   x = T.root
    //   while x != NIL-NODE
    //     y = x
    //     if z.key < x.key
    //       x = x.left
    //     else
    //       x = x.right
    //   z.p = y
    //   if y == NIL-NODE
    //     T.root = z
    //   elseif z.key < y.key
    //     y.left = z
    //   else
    //     y.right = z
    //   z.left = NIL-NODE
    //   z.right = NIL-NODE
    //   z.color = RED
    //   RB-INSERT-FIXUP(T, z)
    void Insert(const T& key) override {
        TreeNode<T>* parent = _nil_node;
        TreeNode<T>* curr = _root;
        while (_nil_node != curr) {
            parent = curr;
            if (key < curr->value) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }

        curr = new ColorTreeNode<T>{};
        curr->parent = parent;
        if (_nil_node == parent) {
            _root = static_cast<ColorTreeNode<T>*>(curr);
        } else if (key < parent->value) {
            parent->left = curr;
        } else {
            parent->right = curr;
        }

        curr->value = key;
        curr->left = _nil_node;
        curr->right = _nil_node;
        static_cast<ColorTreeNode<T>*>(curr)->color = NodeColor::RED;

        InsertFixup(curr);
    }

    // pseudo code
    void Delete(TreeNode<T>* node) override {
    }

    void Traverse() const override {
        InorderTreeWalk(_root, 0);
    }

  private:
    // non copyable
    RedBlackTree(const RedBlackTree&) = delete;
    RedBlackTree& operator=(const RedBlackTree&) = delete;

    // 中序遍历, 即按照左->中->右的顺序显示
    // pseudo code
    // INORDER-TREE-WALK(x)
    //   if x != NIL-NODE
    //     INORDER-TREE-WALK(x.left)
    //     print x.key
    //     INORDER-TREE-WALK(x.right)
    void InorderTreeWalk(TreeNode<T>* node, int level) const {
        if (_nil_node != node) {
            InorderTreeWalk(node->left, level+1);
            const ColorTreeNode<T>* colnode = static_cast<ColorTreeNode<T>*>(
                    node);
            char col = colnode->color == NodeColor::RED ? 'r' : 'b';
            for (int i = 0; i < level; ++i) {
                Printf("--");
            }
            Printf("%(%)\n", node->value, col);
            InorderTreeWalk(node->right, level+1);
        }
    }

    // pseudo code
    // TREE-MAXIMUM(x)
    //   while x.right != NIL-NODE
    //     x = x.right
    //   return x
    TreeNode<T>* InnerMaximum(TreeNode<T>* node) const {
        if (_nil_node == node) {
            return nullptr;
        }
        while (_nil_node != node->right) {
            node = node->right;
        }
        return node;
    }

    // pseudo code
    // TREE-MINIMUM(x)
    //   while x.left != NIL-NODE
    //     x = x.left
    //   return x
    TreeNode<T>* InnerMinimum(TreeNode<T>* node) const {
        if (_nil_node == node) {
            return nullptr;
        }
        while (_nil_node != node->left) {
            node = node->left;
        }
        return node;
    }

    // 旋转操作图例, x, y为内部节点
    //      |                            |
    //      y       left-rotate(x)       x
    //     / \     <---------------     / \
    //    x   γ    --------------->    α   y
    //   / \        right-rotate(y)       / \
    //  α   β                            β   γ

    // 左旋x节点，要求x的右节点不为叶子结点
    // pseudo code
    // LEFT-ROTATE(T,x)
    //   y = x.right
    //   x.right = y.left           处理y-β边
    //   if y.left != NIL-NODE
    //     y.left.p = x
    //   y.p = x.p                  处理x.p-x边
    //   if x.p == NIL-NODE
    //     T.root = y
    //   elseif x == x.p.left
    //     x.p.left = y
    //   else
    //     x.p.right = y
    //   y.left = x                 处理x-y边
    //   x.p = y
    void LeftRotate(TreeNode<T>* node) {
        TreeNode<T>* right_child = node->right;
        // y-β
        node->right = right_child->left;
        if (right_child->left != _nil_node) {
            right_child->left->parent = node;
        }
        // x.p-x
        right_child->parent = node->parent;
        if (node->parent == _nil_node) {
            _root = right_child;
        } else if (node->parent->left == node) {
            node->parent->left = right_child;
        } else if (node->parent->right == node) {
            node->parent->right = right_child;
        }
        // x-y
        right_child->left = node;
        node->parent = right_child;
    }

    // 右旋y节点, 要求y的左节点不为叶子节点
    // pseudo code
    // RIGHT-ROTATE(T,y)
    //   x = y.left
    //   y.left = x.right           处理x-β边
    //   if x.right != NIL-NODE
    //     x.right.p = y
    //   x.p = y.p                  处理y.p-y边
    //   if y.p == NIL-NODE
    //     T.root = x
    //   elseif y.p.left == y
    //     y.p.left = x
    //   else
    //     y.p.right = x
    //   x.right = y                处理y-x边
    //   y.p = x
    void RightRotate(TreeNode<T>* node) {
        TreeNode<T>* left_child = node->left;
        // x-β
        node->left = left_child->right;
        if (left_child->right != _nil_node) {
            left_child->right->parent = node;
        }
        // y.p-y
        left_child->parent = node->parent;
        if (node->parent == _nil_node) {
            _root = left_child;
        } else if (node->parent->left == node) {
            node->parent->left = left_child;
        } else if (node->parent->right == node) {
            node->parent->right = left_child;
        }
        // y-x
        left_child->right = node;
        node->parent = left_child;
    }

    // 保持红黑树的性质
    // pseudo code
    // RB-INSERT-FIXUP(T, z)
    //   while z.p.color == RED
    //     if z.p == z.p.p.left
    //       y = z.p.p.right
    //       if y.color == RED
    //         z.p.color = BLACK
    //         y.color = BLACK
    //         z.p.p.color = RED
    //         z = z.p.p
    //       else
    //         if z == z.p.right
    //           z = z.p
    //           LEFT-ROTATE(T, z)
    //         z.p.color = BLACK
    //         z.p.p.color = RED
    //         RIGHT-ROTATE(T, z.p.p)
    //     else
    //       y = z.p.p.left
    //       if y.color = RED
    //         z.p.color = BLACK
    //         z.p.p.color = RED
    //         z = z.p.p
    //       else
    //         if z == z.p.left
    //           z = z.p
    //           RIGHT-ROTATE(T, z)
    //         z.p.color = BLACK
    //         z.p.p.color = RED
    //         LEFT-ROTATE(T, z.p.p)
    //   T.root.color = BLACk
    void InsertFixup(TreeNode<T>* node) {
        while (GetColorNode(node->parent)->color == NodeColor::RED) {
            if (node->parent == node->parent->parent->left) {
                ColorTreeNode<T>* parent_sibling =
                    GetColorNode(node->parent->parent->right);
                if (parent_sibling->color == NodeColor::RED) {
                    GetColorNode(node->parent)->color = NodeColor::BLACK;
                    parent_sibling->color = NodeColor::BLACK;
                    GetColorNode(node->parent->parent)->color = NodeColor::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        LeftRotate(node);
                    }
                    GetColorNode(node->parent)->color = NodeColor::BLACK;
                    GetColorNode(node->parent->parent)->color = NodeColor::RED;
                    RightRotate(node->parent->parent);
                }
            } else {
                ColorTreeNode<T>* parent_sibling =
                    GetColorNode(node->parent->parent->left);
                if (parent_sibling->color == NodeColor::RED) {
                    GetColorNode(node->parent)->color = NodeColor::BLACK;
                    parent_sibling->color = NodeColor::BLACK;
                    GetColorNode(node->parent->parent)->color = NodeColor::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        RightRotate(node);
                    }
                    GetColorNode(node->parent)->color = NodeColor::BLACK;
                    GetColorNode(node->parent->parent)->color = NodeColor::RED;
                    LeftRotate(node->parent->parent);
                }
            }
        }
        GetColorNode(_root)->color = NodeColor::BLACK;
    }

    // 转换指针类型
    ColorTreeNode<T>* GetColorNode(TreeNode<T>* node) {
        return static_cast<ColorTreeNode<T>*>(node);
    }

    // 释放某一节点的所有子树及本身
    // pseudo code
    // TREE-RELEASE(x)
    //   if x != NIL-NODE and x != NIL
    //     TREE-RELEASE(x.left)
    //     TREE-RELEASE(x.right)
    //     Release x self
    void DeleteTree(TreeNode<T>* node) noexcept {
        if (_nil_node != node && nullptr != node) {
            DeleteTree(node->left);
            DeleteTree(node->right);
            delete node;
        }
    }

    TreeNode<T>*   _nil_node;      // 代表NIL的黑色哨兵节点
    TreeNode<T>*   _root;          // 树根节点
};

} // namespace advanced_struct
} // namespace algorithm

#endif // _ALGORITHM_STRUCT_RED_AND_BLACK_TREE_H_
