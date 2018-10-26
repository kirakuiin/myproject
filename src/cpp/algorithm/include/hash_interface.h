// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-02

#ifndef _ALGORITHM_INCLUDE_HASH_INTERFACE_H_
#define _ALGORITHM_INCLUDE_HASH_INTERFACE_H_

namespace algorithm {
namespace advanced_struct {

// 散列表类接口, 所有类型的散列表均要实现此接口
// example:
//   HashInterface<int> table;
//   table.SetHashMethod([](const int& key){ return key; });
//   table.HashInsert(1, 2);
//   table.HashSearch(1);   // return 2
//   table.HashDelete(1);   // table is empty now
template<typename T>
class HashInterface {
  public:
      static constexpr int NoPosition = -1; // 未找到值返回的下标

      virtual ~HashInterface(){};

      // 设置散列表的散列函数
      // hash接受一个int值并返回一个散列的数组下标
      virtual void SetHashMethod(int (*hash)(const int&)) = 0;

      // 根据关键字key在散列表插入一个值, key为value计算出来的关键字
      virtual void HashInsert(const int& key, const T& value) = 0;

      // 根据关键字key在散列表删除一个值
      virtual void HashDelete(const int& key, const T& value) = 0;

      // 在根据关键字key散列表查询一个值的位置
      virtual int HashSearch(const int& key, const T& value) const = 0;

      // 打印散列表
      virtual void Print() const = 0;
};

} // namespace data_struct
} // namespace algorithm

#endif // _ALGORITHM_INCLUDE_HASH_INTERFACE_H_
