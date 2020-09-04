// queue.h - 线程安全队列
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Sep 01
// License: GPL.v3

#include <boost/circular_buffer.hpp>
#include <boost/thread/shared_mutex.hpp>

#ifndef __EASY_ENGINE_INCLUDE_COMMON_QUEUE_H__
#define __EASY_ENGINE_INCLUDE_COMMON_QUEUE_H__

namespace easy_engine {

using ReadLock  = boost::shared_lock<boost::shared_mutex>;  // 读取锁
using WriteLock = boost::unique_lock<boost::shared_mutex>;  // 写入锁

template <typename T>
class MessageQueue {
 public:
  explicit MessageQueue(size_t max_size)
      : _max_size(max_size), _buffer(max_size) {}

  // 入队
  //
  // @param value: 入队元素
  void Enqueue(const T& value) {
    WriteLock lock(_mutex);
    _buffer.push_back(value);
  }

  // 出队
  void Dequeue() {
    WriteLock lock(_mutex);
    _buffer.pop_front();
  }

  // 查看队列开头元素
  //
  // @return T: 开头元素
  T Top() {
    ReadLock lock(_mutex);
    return _buffer.front();
  }

  // 队列是否为空
  //
  // @return bool: 队列为空
  bool Empty() {
    ReadLock lock(_mutex);
    return _buffer.empty();
  }

  // 查看队列元素数量
  //
  // @return size_t: 队列元素数量
  size_t Size() {
    ReadLock lock(_mutex);
    return _buffer.size();
  }

 private:
  size_t                    _max_size;  // 消息队列最大容量
  boost::circular_buffer<T> _buffer;    // 数据容器

  boost::shared_mutex _mutex;  //锁
};

}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_COMMON_QUEUE_H__
