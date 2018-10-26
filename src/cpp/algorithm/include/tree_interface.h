// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-26

#ifndef _ALGORITHM_INCLUDE_TREE_INTERFACE_H_
#define _ALGORITHM_INCLUDE_TREE_INTERFACE_H_

namespace algorithm {
namespace advanced_struct {

// 红黑树节点的颜色类型定义
enum class NodeColor {
    RED = 0,                            // 红色
    BLACK = 1,                          // 黑色
};

// 一般树的节点定义
template<typename T>
struct TreeNode {
    T value;                            // 节点存放的值
    TreeNode* left = nullptr;           // 左孩子
    TreeNode* right = nullptr;          // 右孩子
    TreeNode* parent= nullptr;          // 双亲
};

// 红黑树节点定义, 每个树节点带有颜色
template<typename T>
struct ColorTreeNode : public TreeNode<T> {
    NodeColor color = NodeColor::BLACK; // 红黑树每个节点的颜色
};

// 树形数据结构类的接口, 所有树型结构类都要实现此接口
// example:
//   TreeInterface<int> tree;
//   tree.Insert(2);
//   auto node = tree.Search(2);
//   tree.Delete(node);
template<typename T>
class TreeInterface {
  public:
    virtual ~TreeInterface(){};

    // 寻找树里值等于关键字的节点, 返回其指针, 如果没有返回空
    virtual TreeNode<T>* Search(const T& key) const = 0;

    // 返回树里值最小的节点指针, 树为空返回空
    virtual TreeNode<T>* Minimum() const = 0;

    // 返回树里值最大的节点的指针, 树为空返回空
    virtual TreeNode<T>* Maximum() const = 0;

    // 返回树里指定节点的后继节点指针, 如果不存在返回空
    virtual TreeNode<T>* Successor(const TreeNode<T>* node) const = 0;

    // 返回树里指定节点的前驱节点指针, 如果不存在返回空
    virtual TreeNode<T>* Predecessor(const TreeNode<T>* node) const = 0;

    // 向树里插入一个节点
    virtual void Insert(const T& key) = 0;

    // 删除树里的指定节点, 注意删除完毕之后node指针无效
    virtual void Delete(TreeNode<T>* node) = 0;

    // 遍历树
    virtual void Traverse() const = 0;
};

} // namespace advanced_struct
} // namespace algorithm

#endif // _ALGORITHM_INCLUDE_TREE_INTERFACE_H_
