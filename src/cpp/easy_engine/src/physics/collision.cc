// collision.cc - 碰撞实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 25
// License: GPL.v3

#include "include/physics/collision.h"

#include <cstdint>
#include <map>
#include <set>

#include "include/common/math.h"

namespace {

using easy_engine::length;
using easy_engine::normalize;
using easy_engine::vec2;

// 得到向量的垂直向量, x坐标一定大于等于0
//
// @param vec: 目标向量
// @return vec2: 垂直向量
vec2 Perpendicular(const vec2& vec) {
  if (vec.y > 0) {
    return vec2(vec.y, vec.x == 0 ? 0 : -vec.x);
  } else if (vec.y == 0) {
    return vec2(0, abs(vec.x));
  } else {
    return vec2(-vec.y, vec.x);
  }
}

// 得到直线上距离一点最近一点坐标
//
// @param begin: 线起点
// @param end: 线终点
// @param origin: 另一点位置
// @param vec2: 最近点坐标
vec2 GetNearestPointToPoint(const vec2& begin, const vec2& end,
                            const vec2& origin) {
  vec2 line_vec  = end - begin;
  vec2 nline_vec = normalize(line_vec);
  vec2 ori_vec   = origin - begin;
  vec2 diff      = nline_vec * dot(ori_vec, nline_vec);
  if (length(line_vec) >= length(diff)) {
    return begin + diff;
  } else {
    return length(origin - end) > length(ori_vec) ? end : begin;
  }
}

}  // namespace

namespace easy_engine {
namespace physics {

std::vector<vec2> DotBox::GetSeparateAxis() const {
  // 对于点来说, 返回一组正交的分离轴即可
  return std::vector<vec2>{vec2(0, 1), vec2(1, 0)};
}

Projection DotBox::GetProjectionOnAxis(const vec2& axis) const {
  double dot_res(dot(Pos, axis));
  return Projection(dot_res, dot_res);
}

DotBox& DotBox::Translate(const vec2& offset) {
  Pos += offset;
  return *this;
}

DotBox DotBox::Add(const vec2& offset) const { return DotBox(Pos + offset); }

std::vector<vec2> LineBox::GetSeparateAxis() const {
  // 对于线来说, 返回一组正交的分离轴即可
  return std::vector<vec2>{normalize(Perpendicular(End - Begin)),
                           normalize(End - Begin)};
}

Projection LineBox::GetProjectionOnAxis(const vec2& axis) const {
  double beg(dot(Begin, axis));
  double end(dot(End, axis));
  return Projection(beg, end);
}

LineBox& LineBox::Translate(const vec2& offset) {
  Begin += offset;
  End += offset;
  return *this;
}

Rect LineBox::GetAABB() const {
  vec2 tl(min(Begin.x, End.x), min(Begin.y, End.y));
  vec2 br(max(Begin.x, End.x), max(Begin.y, End.y));
  return Rect(tl, br - tl);
}

LineBox LineBox::Add(const vec2& offset) const {
  return LineBox(Begin + offset, End + offset);
}

std::vector<vec2> PolygonBox::GetSeparateAxis() const {
  std::vector<vec2>             axises;
  std::set<vec2, VecLess<vec2>> axis_set;
  for (int i = 0; i < Vertices.size(); ++i) {
    int  j    = (i == Vertices.size() - 1) ? 0 : i + 1;
    vec2 axis = normalize(Perpendicular(Vertices[j] - Vertices[i]));
    if (axis_set.count(axis) == 0) {
      axises.push_back(axis);
    }
    axis_set.insert(axis);
  }
  return axises;
}

Projection PolygonBox::GetProjectionOnAxis(const vec2& axis) const {
  double p_min, p_max;
  p_min = p_max = dot(Vertices[0], axis);
  for (int i = 1; i < Vertices.size(); ++i) {
    double dot_res = dot(Vertices[i], axis);
    p_min          = min(p_min, dot_res);
    p_max          = max(p_max, dot_res);
  }
  return Projection(p_min, p_max);
}

PolygonBox& PolygonBox::Translate(const vec2& offset) {
  for (auto& vert : Vertices) {
    vert += offset;
  }
  return *this;
}

vec2 PolygonBox::GetCenter() const {
  vec2 c(0);
  for (const auto& vert : Vertices) {
    c += vert;
  }
  return c / static_cast<float>(Vertices.size());
}

Rect PolygonBox::GetAABB() const {
  vec2 tl(Vertices[0]), br(Vertices[0]);
  for (const auto& vert : Vertices) {
    if (vert.x < tl.x) {
      tl.x = vert.x;
    }
    if (vert.x > br.x) {
      br.x = vert.x;
    }
    if (vert.y < tl.y) {
      tl.y = vert.y;
    }
    if (vert.y > br.y) {
      br.y = vert.y;
    }
  }
  return Rect(tl, br - tl);
}

PolygonBox PolygonBox::Add(const vec2& offset) const {
  std::vector<vec2> new_verts;
  for (const auto& iter : Vertices) {
    new_verts.push_back(iter + offset);
  }
  return PolygonBox(new_verts);
}

std::vector<vec2> CircleBox::GetSeparateAxis() const {
  // 圆没有明确的分离轴, 仅返回添加的分离轴
  return std::vector<vec2>{_axis};
}

Projection CircleBox::GetProjectionOnAxis(const vec2& axis) const {
  float dot_res(dot(Origin, axis));
  return Projection(dot_res - Radius, dot_res + Radius);
}

CircleBox& CircleBox::Translate(const vec2& offset) {
  Origin += offset;
  return *this;
}

CircleBox CircleBox::Add(const vec2& offset) const {
  return CircleBox(Origin + offset, Radius);
}

CollisionResult CheckCollision(const HitBox& lhs, const HitBox& rhs) {
  std::set<vec2, VecLess<vec2>> s_axis;
  for (const auto& axis : lhs.GetSeparateAxis()) {
    s_axis.insert(axis);
  }
  for (const auto& axis : rhs.GetSeparateAxis()) {
    s_axis.insert(axis);
  }
  CollisionResult res;
  float           minimum_overlap(static_cast<float>(INT32_MAX));
  for (const auto& axis : s_axis) {
    Projection l_proj(lhs.GetProjectionOnAxis(axis));
    Projection r_proj(rhs.GetProjectionOnAxis(axis));
    // 如果存在一个轴不重合, 那么两个碰撞盒不相交
    if (!l_proj.IsOverlap(r_proj)) {
      return res;
    } else {
      double curr_overlap = l_proj.GetOverlap(r_proj);
      // 处理包含情况
      if (l_proj.IsContain(r_proj) || r_proj.IsContain(l_proj)) {
        curr_overlap +=
            min(abs(l_proj.Min - r_proj.Min), abs(l_proj.Max - r_proj.Max));
      }
      if (curr_overlap < minimum_overlap) {
        minimum_overlap = curr_overlap;
        res.MTV         = axis * minimum_overlap;
        // 根据相对位置处理MTV的值
        if (dot(rhs.GetCenter() - lhs.GetCenter(), axis) > 0) {
          res.MTV = -res.MTV;
        }
      }
    }
  }
  // 代码到这里代表所有轴都有重合部分, 证明两碰撞盒相交
  res.IsIntersect = true;
  return res;
}

CollisionResult CheckCollision(const DotBox& lhs, const DotBox& rhs) {
  CollisionResult res;
  if (lhs.Pos == rhs.Pos) {
    res.IsIntersect = true;
    res.MTV         = vec2(0, 0);
  }
  return res;
}

CollisionResult CheckCollision(CircleBox& lhs, CircleBox& rhs) {
  if (lhs.Origin != rhs.Origin) {
    lhs.SetExtraAxis(normalize(lhs.Origin - rhs.Origin));
  } else {
    // 重合时任意轴均可
    lhs.SetExtraAxis(vec2(1, 0));
  }
  return CheckCollision(dynamic_cast<HitBox&>(lhs), dynamic_cast<HitBox&>(rhs));
}

CollisionResult CheckCollision(const DotBox& lhs, CircleBox& rhs) {
  if (lhs.Pos != rhs.Origin) {
    rhs.SetExtraAxis(normalize(lhs.Pos - rhs.Origin));
  }
  return CheckCollision(lhs, dynamic_cast<HitBox&>(rhs));
}

CollisionResult CheckCollision(CircleBox& lhs, const DotBox& rhs) {
  auto res = CheckCollision(rhs, lhs);
  if (res.IsIntersect) {
    res.MTV = -res.MTV;
  }
  return res;
}

CollisionResult CheckCollision(const LineBox& lhs, CircleBox& rhs) {
  vec2 nearest_point = GetNearestPointToPoint(lhs.Begin, lhs.End, rhs.Origin);
  // 最近点和原点不重合
  if (nearest_point != rhs.Origin) {
    rhs.SetExtraAxis(normalize(nearest_point - rhs.Origin));
  }
  return CheckCollision(lhs, dynamic_cast<HitBox&>(rhs));
}

CollisionResult CheckCollision(CircleBox& lhs, const LineBox& rhs) {
  auto res = CheckCollision(rhs, lhs);
  if (res.IsIntersect) {
    res.MTV = -res.MTV;
  }
  return res;
}

CollisionResult CheckCollision(const PolygonBox& lhs, CircleBox& rhs) {
  size_t vert_len = lhs.Vertices.size();
  // 找到距离圆心最近直线的端点
  int   mini(0), seci(0);
  float minl(float(INT32_MAX));
  for (int i = 0; i < vert_len; ++i) {
    float len = length(rhs.Origin - lhs.Vertices[i]);
    if (minl > len) {
      minl = len;
      mini = i;
    }
  }
  vec2 prev(lhs.Vertices[(mini - 1 + vert_len) % vert_len]);
  vec2 next(lhs.Vertices[(mini + 1 + vert_len) % vert_len]);
  if (length(rhs.Origin - prev) < length(rhs.Origin - next)) {
    seci = (mini - 1 + vert_len) % vert_len;
  } else {
    seci = (mini + 1 + vert_len) % vert_len;
  }
  vec2 nearest_point(GetNearestPointToPoint(lhs.Vertices[mini],
                                            lhs.Vertices[seci], rhs.Origin));
  // 最近点和原点不重合
  if (nearest_point != rhs.Origin) {
    rhs.SetExtraAxis(normalize(rhs.Origin - nearest_point));
  }
  return CheckCollision(lhs, dynamic_cast<HitBox&>(rhs));
}

CollisionResult CheckCollision(CircleBox& lhs, const PolygonBox& rhs) {
  auto res = CheckCollision(rhs, lhs);
  if (res.IsIntersect) {
    res.MTV = -res.MTV;
  }
  return res;
}

}  // namespace physics
}  // namespace easy_engine
