// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-17

#ifndef _ALGORITHM_STRUCT_CHAINING_HASH_H_
#define _ALGORITHM_STRUCT_CHAINING_HASH_H_

#include "include/hash_interface.h"
#include "basic_struct/list.h"
#include "common/algo_util.h"

namespace algorithm {
namespace advanced_struct {

// 线性表散列表
// 每当产生冲突时, 将元素插入到对应槽位号的表里, 这也是这种散列法名字的由来
// 装载因子可以大于1
template <typename T, int _TableSize>
class ChainingHash : public HashInterface<T> {
  public:
    ChainingHash() : _hash{[](const int& key){return key%_TableSize;}} {}

    virtual ~ChainingHash() {}

    void SetHashMethod(int (*hash)(const int&)) override {
        _hash = hash;
    }

    // pseudo code
    // CHAINED-HASH-INSERT(T, x)
    //   insert x at the head of list T[h(x.key)]
    void HashInsert(const int& key, const T& value) override {
        basic_struct::ListNode<T>* node = basic_struct::CreateListNode(value);
        _table[_hash(key)].Insert(node);
    }

    // pseudo code
    // CHAINED-HASH-DELETE(T, x)
    //   delete x from the list T[h(x.key)]
    void HashDelete(const int& key, const T& value) override {
        int pos = _hash(key);
        basic_struct::ListNode<T>* node = _table[pos].Search(value);
        _table[pos].Delete(node);
    }

    // pseudo code
    // CHANED-HASH-SEARCH(T, k)
    //   search for an element with key k in list T[h(k)]
    int HashSearch(const int& key, const T& value) const override {
        int pos = _hash(key);
        basic_struct::ListNode<T>* node = _table[pos].Search(value);
        if (node) {
            return pos;
        } else {
            return HashInterface<T>::NoPosition;
        }
    }

    void Print() const override {
        for (int i = 0; i < _TableSize; ++i) {
            Printf("hashtable[%]:", i);
            _table[i].Print();
        }
    }

  private:
    // non copyable
    ChainingHash(const ChainingHash&) = delete;
    ChainingHash& operator=(const ChainingHash&) = delete;

    basic_struct::List<T> _table[_TableSize];   // 链式散列表
    int (*_hash)(const int&);                   // 散列函数
};

} // namespace advanced_struct
} // namespace algorithm

#endif // _ALGORITHM_STRUCT_CHAINING_HASH_H_
