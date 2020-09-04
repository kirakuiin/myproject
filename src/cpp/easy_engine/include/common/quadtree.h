// quadtree.h - 四叉树
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Sep 03
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_COMMON_QUADTREE_H__
#define __EASY_ENGINE_INCLUDE_COMMON_QUADTREE_H__

#include <sys/resource.h>

#include <cstdio>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include "format.h"
#include "math.h"

namespace easy_engine {

// 四叉树
template <typename T, typename Compare = std::less<T>>
class QuadTree final {
 public:
  // @param max_elem_size: 每个区域内最大元素数量
  // @param range: 四叉树的范围
  QuadTree(size_t max_elem_size, const Rect& range)
      : _max_elem_size(max_elem_size),
        _range(range),
        _current_size(0),
        _depth(0) {}
  ~QuadTree() {}

  // 在指定区域中插入一个元素
  //
  // @param elem: 元素本身
  // @param range: 元素的区域
  // @return bool: 是否成功插入
  bool Insert(const T& elem, const Rect& range);

  // 删除一个元素
  //
  // @param elem: 待删除的元素
  // @param range: 元素所在区域
  // @return bool: 元素是否删除
  bool Remove(const T& elem, const Rect& range);

  // 删除一个元素
  //
  // @param elem: 待删除的元素
  // @return bool: 元素是否删除
  bool Remove(const T& elem) { return Remove(elem, _range); }

  // 返回指定区域中的所有元素
  //
  // @param range: 指定区域
  // @return vector: 区域元素集合
  std::vector<T> Query(const Rect& range) const;

  // 查看元素数目
  size_t Size() const { return _current_size; }

  // 输出结构
  std::string TreeStruct() const;

 private:
  QuadTree(size_t max_elem_size, const Rect& range, int depth)
      : _max_elem_size(max_elem_size),
        _range(range),
        _current_size(0),
        _depth(depth) {}
  // 当元素数量超过限制时拆分当前节点
  void Partition();

  size_t _max_elem_size;  // 节点最大元素数量
  Rect   _range;          // 四叉树的范围
  size_t _current_size;   //当前四叉树内元素树木
  size_t _depth;          // 当前深度

  std::map<T, Rect, Compare>             _m_elems;   // 当前区域元素
  std::vector<std::shared_ptr<QuadTree>> _v_childs;  // 孩子区域
};

template <typename T, typename Compare>
bool QuadTree<T, Compare>::Insert(const T& elem, const Rect& range) {
  if (!_range.Contain(range)) {
    return false;
  }
  _current_size += 1;
  if (!_v_childs.empty()) {
    bool res(false);
    for (auto& child : _v_childs) {
      res |= child->Insert(elem, range);
    }
    if (res) {
      return true;
    }
  }
  _m_elems.insert(std::pair<T, Rect>(elem, range));
  if (_v_childs.empty() && _m_elems.size() > _max_elem_size) {
    Partition();
  }
  return true;
}

template <typename T, typename Compare>
bool QuadTree<T, Compare>::Remove(const T& elem, const Rect& range) {
  if (!_range.Intersect(range)) {
    return false;
  }
  if (_m_elems.count(elem)) {
    _m_elems.erase(elem);
    _current_size -= 1;
    return true;
  }
  bool res(false);
  for (auto& child : _v_childs) {
    res |= child->Remove(elem, range);
    if (res) {
      _current_size -= 1;
      break;
    }
  }
  // 如果子节点没有元素, 清除全部子节点
  if (res && !_current_size) {
    _v_childs.clear();
  }
  return res;
}

template <typename T, typename Compare>
std::vector<T> QuadTree<T, Compare>::Query(const Rect& range) const {
  std::vector<T> result;
  if (!_range.Intersect(range)) {
    return result;
  }
  for (const auto& child : _v_childs) {
    auto res = child->Query(range);
    result.insert(result.end(), res.begin(), res.end());
  }
  for (const auto& elem : _m_elems) {
    if (range.Intersect(elem.second)) {
      result.push_back(elem.first);
    }
  }
  return result;
}

template <typename T, typename Compare>
std::string QuadTree<T, Compare>::TreeStruct() const {
  std::string prefix;
  for (int i = 0; i < _depth; ++i) {
    prefix += "  ";
  }
  std::string result(prefix);
  result += Format("{Rect(%, %)\n", _range.Pos, _range.Size);
  result += prefix;
  result += Format(" elems: %\n", _current_size);
  for (const auto& child : _v_childs) {
    result += child->TreeStruct();
  }
  result += prefix;
  result += std::string("}\n");
  return result;
}

template <typename T, typename Compare>
void QuadTree<T, Compare>::Partition() {
  // 构造4个子节点
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      vec2 new_size = _range.Size / 2.0f;
      Rect range(
          vec2(_range.Pos.x + j * new_size.x, _range.Pos.y + i * new_size.y),
          new_size);
      _v_childs.push_back(std::shared_ptr<QuadTree>(
          new QuadTree<T, Compare>(_max_elem_size, range, _depth + 1)));
    }
  }
  std::vector<T> remove_list;
  // 移动元素
  for (auto& elem : _m_elems) {
    for (auto& child : _v_childs) {
      if (child->Insert(elem.first, elem.second)) {
        remove_list.push_back(elem.first);
        break;
      }
    }
  }
  // 删除父节点元素
  for (auto& remove_elem : remove_list) {
    _m_elems.erase(remove_elem);
  }
}

}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_COMMON_QUADTREE_H__
