// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-11

#ifndef _ALGORITHM_STRUCT_LIST_H_
#define _ALGORITHM_STRUCT_LIST_H_

#include "common/algo_util.h"

namespace algorithm {
namespace basic_struct {

// 链表里的每一个节点
template <typename T>
struct ListNode {
    T value;                    // 节点的值
    ListNode* prev = nullptr;   // 前驱节点
    ListNode* next = nullptr;   // 后继节点
};

// 创建节点
template <typename T>
ListNode<T>* CreateListNode(const T& key) {
    return new ListNode<T>{key, nullptr, nullptr};
}

// 链表, 简单的顺序容器
// 采用带有哨兵的双向循环链表进行实现
// example:
//   List<int> mylist;
//   mylist.Insert(1);  // sen<->1<->sen
//   mylist.Insert(3);  // sen<->3<->1<->sen
//   ListNode<int>* node = mylist.Search(1);
//   mylist.Delete(node); // sen<->3<->sen
template <typename T>
class List {
  public:
    List() : _sentinel{CreateListNode<T>(T())} {
        _sentinel->prev = _sentinel->next = _sentinel;
    };

    ~List() {
        ListNode<T>* curr_ptr = _sentinel->next;
        while (curr_ptr != _sentinel) {
            Delete(curr_ptr);
            curr_ptr = _sentinel->next;
        }
        Delete(_sentinel);
    };

    // 向链表里插入新的元素
    //
    // pseudo code
    // LIST-INSERT(L, x)
    //   x.next = L.nil.next
    //   L.nil.next.prev = x
    //   x.prev = L.nil
    //   L.nil.next = x
    void Insert(ListNode<T>* new_node) {
        new_node->prev = _sentinel;
        new_node->next = _sentinel->next;
        _sentinel->next->prev = new_node;
        _sentinel->next = new_node;
    }

    // 查询链表里的某个元素并返回其指针
    //
    // pseudo code
    // LIST-SEARCH(L, k)
    //   x = L.nil.next
    //   while x != L.nil and x.key != k
    //     x = x.next
    //   return x
    ListNode<T>* Search(const T& elem) const {
        for (ListNode<T>* iter = _sentinel->next;
                iter != _sentinel; iter = iter->next) {
            if (iter->value == elem) {
                return iter;
            }
        }
        return nullptr;
    }

    // 根据search返回的指针删除链表里的某个元素
    //
    // pseudo code
    // LIST-DELETE(L, x)
    //   x.next.prev = x.prev
    //   x.prev.next = x.next
    void Delete(ListNode<T>* ptr) {
        if (ptr) {
            ptr->next->prev = ptr->prev;
            ptr->prev->next = ptr->next;
            delete ptr;
            ptr = nullptr;
        }
    }

    // 打印链表内容
    void Print() const {
        ListNode<T>* curr_ptr = _sentinel->next;
        while (curr_ptr != _sentinel) {
            Printf("% ", curr_ptr->value);
            curr_ptr = curr_ptr->next;
        }
        Printf("\n");
    }

  private:
    // non copyable
    List(const List&) = delete;
    List& operator=(const List&) = delete;

    ListNode<T>*   _sentinel;   // 哨兵
};

} // namespace basic_struct
} // namespace algorithm

#endif // _ALGORITHM_STRUCT_LIST_H_
