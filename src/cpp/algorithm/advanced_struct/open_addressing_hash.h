// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-19

#ifndef _ALGORITHM_STRUCT_OPEN_ADDRESSING_HASH_H_
#define _ALGORITHM_STRUCT_OPEN_ADDRESSING_HASH_H_

#include "include/hash_interface.h"

namespace algorithm {
namespace advanced_struct {

// 开放定址法散列表
// 当产生冲突时, 根据辅助散列函数将元素插入到后续的位置
// 所以整个表的装载因子小于等于1
template <typename T, int _TableSize>
class OpenAddressingHash : public HashInterface<T> {
  public:
    OpenAddressingHash() : _hash{[](const int& key){return key%_TableSize;}} {
        for (int i = 0; i < _TableSize; ++i) {
            _flag_table[i] = TableFlag::EMPTY;
        }
    }
    virtual ~OpenAddressingHash() {};

    void SetHashMethod(int (*hash)(const int&)) override {
        _hash = hash;
    }

    // pseudo code
    // HASH-INSERT(T, k)
    //   i = 0
    //   repeat
    //     j=h(k,i)
    //     if T[j] == NIL
    //       T[j] = k
    //       return j
    //     else i = i + 1
    //   until i == m
    //   error "hash table overflow"
    void HashInsert(const int& key, const T& value) override {
        int index = 0;
        do {
            int new_index = DoubleHashing(key, index);
            if (TableFlag::USED != _flag_table[new_index]) {
                _table[new_index] = value;
                _flag_table[new_index] = TableFlag::USED;
                return;
            } else {
                ++index;
            }
        } while (index != _TableSize);

        throw std::runtime_error("hash table overflow");
    }

    void HashDelete(const int& key, const T& value) override {
        int index = HashSearch(key, value);
        if (HashInterface<T>::NoPosition != index) {
            _flag_table[index] = TableFlag::DELETE;
        }
    }

    // pseudo code
    // HASH-SEARCH(T, k)
    //   i = 0
    //   repeat
    //     j = h(k,i)
    //     if T[j] == k
    //       return j
    //     else i = i + 1
    //   until i == m or T[j] == NIL
    //   return NIL
    int HashSearch(const int& key, const T& value) const override {
        int index = 0;
        do {
            int new_index = DoubleHashing(key, index);
            if ((value == _table[new_index]) &&
                    TableFlag::USED == _flag_table[new_index]) {
                return new_index;
            } else if (TableFlag::EMPTY == _flag_table[new_index]) {
                return HashInterface<T>::NoPosition;
            } else {
                ++index;
            }
        } while (index != _TableSize);

        return HashInterface<T>::NoPosition;
    }

    void Print() const override {
        Printf("hashtable:\n");
        for (int i = 0; i < _TableSize; ++i) {
            Printf("%[flag=%]\n", _table[i],
                    static_cast<int>(_flag_table[i]));
        }
        Printf("\n");
    }

  private:
    // 用于表示散列表里每一个槽位的状态
    enum class TableFlag {
        EMPTY = 0,  // 此槽位未被使用
        USED = 1,   // 此槽位已被使用
        DELETE = 2, // 此槽位已被删除
    };

    // non copyable
    OpenAddressingHash(const OpenAddressingHash&) = delete;
    OpenAddressingHash& operator=(const OpenAddressingHash&) = delete;

    // 线性探查
    // h(k,i) = (h'(k)+i) mod m
    int LinerProbing(const int& key, const int& index) const {
        return (_hash(key) + index)%_TableSize;
    }

    // 二次探查
    // h(k, i) = (h'(k)+c1*i+c2*i^2) mod m
    // c1, c2要根据m合理取值才能利用整个散列表的空间
    int QuadraticProbing(const int& key, const int& index) const {
        // 简单实现
        return (_hash(key) + index + pow(index,2))%_TableSize;
    }

    // 双重散列
    // h(k, i) = (h1(k) + i*h2(k)) mod m
    // 确保m和h2(k)产生的数互素可以达到较好的效果
    int DoubleHashing(const int& key, const int& index) const {
        // 这里假设m为2的幂, h2产生奇数即可互素
        auto help_func = [](const int& key) { return key%2?key+1:key; };
        return (_hash(key) + index*help_func(key))%_TableSize;
    };

    T   _table[_TableSize];             // 开放定址散列表
    TableFlag _flag_table[_TableSize];  // 状态辅助表
    int (*_hash)(const int&);           // 散列函数
};

} // namespace advanced_struct
} // namespace algorithm

#endif // _ALGORITHM_STRUCT_OPEN_ADDRESSING_HASH_H_
