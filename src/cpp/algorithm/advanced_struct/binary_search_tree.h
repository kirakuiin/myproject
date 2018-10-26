// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-13

#ifndef _ALGORITHM_STRUCT_BINARY_TREE_H_
#define _ALGORITHM_STRUCT_BINARY_TREE_H_

#include "include/tree_interface.h"

namespace algorithm {
namespace advanced_struct {

// 二叉搜索树
// 所有节点符合以下性质, 节点的左孩子的值小于节点本身的值, 节点本身的值
// 小于节点右孩子的值
// 其所有支持的操作时间复杂度均为Ο(h), h为二叉树的高度
template <typename T>
class BinarySearchTree : public TreeInterface<T> {
  public:
    BinarySearchTree() : _root(nullptr) {}
    virtual ~BinarySearchTree() { DeleteTree(_root); }

    // pseudo code
    // TREE-SEARCH(x, k)
    //   if x == NIL or k == x.key
    //     return x
    //   if k < x.key
    //     return TREE-SEARCH(x.left, k)
    //   else
    //     return TREE-SEARCH(x.right. k)
    TreeNode<T>* Search(const T& key) const override {
        TreeNode<T>* node = _root;
        while (nullptr != node) {
            if (key == node->value) {
                break;
            } else if (key < node->value) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return node;
    }

    TreeNode<T>* Minimum() const override {
        return InnerMinimum(_root);
    }

    TreeNode<T>* Maximum() const override {
        return InnerMaximum(_root);
    }

    // pseudo code
    // TREE-SUCCESSOR(x)
    //   if x.right != NIL
    //     return TREE-MINIMUM(x.right)
    //   y = x.parent
    //   while y.right == x and y != NIL
    //     x = y
    //     y = y.parent
    //   return y
    TreeNode<T>* Successor(const TreeNode<T>* node) const override {
        assert(nullptr != node);
        if (nullptr != node->right) {
            return InnerMinimum(node->right);
        } else {
            TreeNode<T>* parent = node->parent;
            while ((node == parent->right) && (nullptr!= parent)) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }

    // pseudo code
    // TREE-PREDECESSOR(x)
    //   if x.left != NIL
    //     return TREE-MAXIMUM(x.left)
    //   y = x.parent
    //   while y.left == x and y != NIL
    //     x = y
    //     y = y.parent
    //   return y
    TreeNode<T>* Predecessor(const TreeNode<T>* node) const override {
        assert(nullptr != node);
        if (nullptr != node->left) {
            return InnerMaximum(node->left);
        } else {
            TreeNode<T>* parent = node->parent;
            while ((node == parent->left) && (nullptr!= parent)) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }

    // pseudo code
    // TREE-INSERT(T, z)
    //   y = NIL
    //   x = T.root
    //   while x != NIL
    //     y = x
    //     if k.key < x.key
    //       x = x.left
    //     else
    //       x = x.right
    //   z.p = y
    //   if y == NIL
    //     T.root = z
    //   elseif x.key < x.key
    //     y.left = z
    //   else
    //     y.right = z
    void Insert(const T& key) override {
        TreeNode<T>* parent = nullptr;
        TreeNode<T>* curr = _root;
        while (nullptr != curr) {
            parent = curr;
            if (key < curr->value) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        curr = new TreeNode<T>{};
        curr->parent = parent;
        if (nullptr == parent) {
            _root = curr;
        } else if (key < parent->value) {
            parent->left = curr;
        } else {
            parent->right = curr;
        }
        curr->value = key;
    }

    // pseudo code
    // TREE-DELETE(T,z)
    //   if z.left == NIL
    //     TRANSPLANT(T, z, z.right)
    //   elseif z.right == NIL
    //     TRANSPLANT(T, z, z.left)
    //   else
    //      y = TREE-MINIMUM(z.right)
    //      if y.p != z
    //        TRANSPLANT(T, y, y.right)
    //        y.right = z.right
    //        y.right.p = y
    //      TRANSPLANT(T,z,y)
    //      y.left = z.left
    //      y.left.p = y
    void Delete(TreeNode<T>* node) override {
        if (nullptr == node) {
            return;
        } else {
            if (nullptr == node->left) {
                Transplant(node, node->right);
            } else if (nullptr == node->right) {
                Transplant(node, node->left);
            } else {
                TreeNode<T>* rightmin = InnerMinimum(node->right);
                if (rightmin->parent != node) {
                    Transplant(rightmin, rightmin->right);
                    rightmin->right = node->right;
                    rightmin->right->parent = rightmin;
                }
                Transplant(node, rightmin);
                rightmin->left = node->left;
                rightmin->left->parent = rightmin;
            }
            delete node;
        }
    }

    void Traverse() const override {
        InorderTreeWalk(_root);
    }

  private:
    // non copyable
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    // 中序遍历, 即按照左->中->右的顺序显示
    // pseudo code
    // INORDER-TREE-WALK(x)
    //   if x != NIL
    //     INORDER-TREE-WALK(x.left)
    //     print x.key
    //     INORDER-TREE-WALK(x.right)
    void InorderTreeWalk(TreeNode<T>* node) const {
        if (nullptr != node) {
            InorderTreeWalk(node->left);
            Printf("% ", node->value);
            InorderTreeWalk(node->right);
        }
    }

    // 先序遍历, 即按照中->左->右的顺序显示
    // pseudo code
    // PREORDER-TREE-WALK(x)
    //   if x != NIL
    //     print x.key
    //     INORDER-TREE-WALK(x.left)
    //     INORDER-TREE-WALK(x.right)
    void PreorderTreeWalk(TreeNode<T>* node) const {
        if (nullptr != node) {
            Printf("% ", node->value);
            PreorderTreeWalk(node->left);
            PreorderTreeWalk(node->right);
        }
    }

    // 后序遍历, 即按照左->右->中的顺序显示
    // pseudo code
    // POSTORDER-TREE-WALK(x)
    //   if x != NIL
    //     POSTORDER-TREE-WALK(x.left)
    //     POSTORDER-TREE-WALK(x.right)
    //     print x.key
    void PostorderTreeWalk(TreeNode<T>* node) const {
        if (nullptr != node) {
            PostorderTreeWalk(node->left);
            PostorderTreeWalk(node->right);
            Printf("% ", node->value);
        }
    }

    // pseudo code
    // TREE-MAXIMUM(x)
    //   while x.right!= NIL
    //     x = x.right
    //   return x
    TreeNode<T>* InnerMaximum(TreeNode<T>* node) const {
        if (nullptr == node) {
            return nullptr;
        }
        while (nullptr != node->right) {
            node = node->right;
        }
        return node;
    }

    // pseudo code
    // TREE-MINIMUM(x)
    //   while x.left != NIL
    //     x = x.left
    //   return x
    TreeNode<T>* InnerMinimum(TreeNode<T>* node) const {
        if (nullptr == node) {
            return nullptr;
        }
        while (nullptr != node->left) {
            node = node->left;
        }
        return node;
    }

    // pseudo code
    // 将u子树用v子树替换
    // TRANSPLANT(T,u,v)
    //   if u.p == NIL
    //     T.root = v
    //   elseif u == u.p.left
    //     u.p.left = v
    //   else u.p.right = v
    //   if v != NIL
    //     v.p = u.p
    void Transplant(TreeNode<T>* u, TreeNode<T>* v) {
        assert(nullptr != u);
        if (u == v) {
            return;
        }
        if (u == _root) {
            _root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        if (nullptr != v) {
            v->parent = u->parent;
        }
    }

    // 释放某一节点的所有子树及本身
    // pseudo code
    // TREE-RELEASE(x)
    //   if x != NIL
    //     TREE-RELEASE(x.left)
    //     TREE-RELEASE(x.right)
    //     Release x self
    void DeleteTree(TreeNode<T>* node) noexcept {
        if (nullptr != node) {
            DeleteTree(node->left);
            DeleteTree(node->right);
            delete node;
        }
    }

    TreeNode<T>* _root;         // 树根
};

} // namespace advanced_struct
} // namespace algorithm

#endif // _ALGORITHM_STRUCT_BINARY_TREE_H_
