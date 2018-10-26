// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-22

#ifndef _ALGORITHM_STRUCT_PERFECT_HASH_H_
#define _ALGORITHM_STRUCT_PERFECT_HASH_H_

namespace algorithm {
namespace advanced_struct {

// 完美散列
// 是指当关键字集合静态时(一点插入就不再修改), 可以提供Ο(1)的访存性能
// 一般使用二次全域散列来实现, 一级散列表的槽位和关键字个数相同, 二级
// 散列表的槽位等于一级散列后分配到此槽位关键字个数的平方, 其中的散列
// 函数均使用全域散列函数组
template<typename T, int _TableSize>
class PerfectHash : public HashInterface<T> {
  public:
    explicit PerfectHash(const int* keylist) {
        CalcSlotSize(keylist);
    }
    virtual ~PerfectHash() {
        for (int i = 0; i < _TableSize; ++i) {
            if (_table[i]) {
                delete[] _table[i];
            }
        }
    }

    void SetHashMethod(int (*hash)(const int&)) override {
        throw "hash func is fixed";
    }

    void HashInsert(const int& key, const T& value) override {
        _table[FirstHash(key)][SecondHash(key)] = value;
    }

    void HashDelete(const int& key, const T& value) override {
        throw "can not delete, this is a static set";
    }

    int HashSearch(const int& key, const T& value) const override {
        if (value == _table[FirstHash(key)][SecondHash(key)]) {
            return FirstHash(key);
        } else {
            return HashInterface<T>::NoPosition;
        }
    }

    void Print() const override {
        Printf("perfecttable:\n");
        for (int i = 0; i < _TableSize; ++i) {
            Printf("slot[%]:", i);
            for (int j = 0; j < _size_table[i]; ++j) {
                Printf("% ", _table[i][j]);
            }
            Printf("\n");
        }
    }
  private:
    // non copyable
    PerfectHash(const PerfectHash&) = delete;
    PerfectHash& operator=(const PerfectHash&) = delete;

    // 一级散列的槽位号等于关键字的数量
    int FirstHash(const int& key) const {
        constexpr int64_t kPrime = 10000019;
        static const int64_t kA = RandomInt(1, kPrime-1);
        static const int64_t kB = RandomInt(0, kPrime-1);
        return ((kA*key+kB)%kPrime)%_TableSize;
    }

    // 二级散列的槽位不确定
    int SecondHash(const int& key) const {
        constexpr int64_t kPrime = 10000019;
        static const int64_t kA = RandomInt(1, kPrime-1);
        static const int64_t kB = RandomInt(0, kPrime-1);
        return ((kA*key+kB)%kPrime)%_size_table[FirstHash(key)];
    }

    // 计算每个二级散列表的槽位数, 并分配空间
    void CalcSlotSize(const int* keylist) {
        for (int i = 0; i < _TableSize; ++i) {
            _size_table[i] = 0;
            _table[i] = nullptr;
        }
        // 计算分配到每个槽位关键字的个数
        for (int i = 0; i < _TableSize; ++i) {
            ++_size_table[FirstHash(keylist[i])];
        }
        // 对每个size做平方操作
        for (int i = 0; i < _TableSize; ++i) {
            if (_size_table[i]) {
                _size_table[i] = _size_table[i]*_size_table[i];
                _table[i] = new T[_size_table[i]];
                memset(_table[i], -1, sizeof(T)*_size_table[i]);
            }
        }
    }

    int _size_table[_TableSize];    // 记录分配到每一个二级表的关键字的个数
    T* _table[_TableSize];          // 存放数据的二重散列表
};

} // namespace advanced_struct
} // namespace algorithm

#endif // _ALGORITHM_STRUCT_PERFECT_HASH_H_
