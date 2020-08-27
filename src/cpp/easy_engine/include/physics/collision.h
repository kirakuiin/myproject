// collision.h - 碰撞系统
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 25
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_PHYSICS_COLLISION_H__
#define __EASY_ENGINE_INCLUDE_PHYSICS_COLLISION_H__

#include <initializer_list>
#include <vector>

#include "../common/math.h"

namespace easy_engine {
namespace physics {

// 碰撞结果
struct CollisionResult {
  CollisionResult() : IsIntersect(false) {}
  explicit CollisionResult(vec2 mtv) : IsIntersect(true), MTV(mtv) {}
  bool IsIntersect;  // 是否碰撞
  vec2 MTV;          // 最小移动向量
};

// 投影
struct Projection {
  Projection(double lhs, double rhs) : Min(min(lhs, rhs)), Max(max(lhs, rhs)) {}

  // 判断两个投影是否相互覆盖
  // @param rhs: 另一个投影
  // @return bool: 是否相交
  bool IsOverlap(const Projection& rhs) {
    return Min <= rhs.Max && rhs.Min <= Max;
  }

  // 得到投影的相交长度
  // @param rhs: 另一个投影
  // @return double: 相交长度
  double GetOverlap(const Projection& rhs) {
    return min(Max, rhs.Max) - max(Min, rhs.Min);
  }

  // 此投影是否包含另一个投影
  // @param rhs: 另一个投影
  // @return boo: 是否包含
  bool IsContain(const Projection& rhs) {
    return Min <= rhs.Min && Max >= rhs.Max;
  }

  double Min;  // 投影最小值
  double Max;  // 投影最大值
};

struct HitBox {
  virtual ~HitBox(){};

  // 得到碰撞盒的所有分离轴, 轴已经归一化
  //
  // @return vector<vec2>: 分离轴数组
  virtual std::vector<vec2> GetSeparateAxis() const = 0;

  // 得到碰撞盒在指定分离轴上的投影
  //
  // @param axis: 被投影分离轴(要求必须归一化)
  // @return Projection: 分离轴上的投影
  virtual Projection GetProjectionOnAxis(const vec2& axis) const = 0;

  // 平移碰撞盒
  //
  // @param offset: 平移距离
  // @return HitBox&: 自身的引用
  virtual HitBox& Translate(const vec2& offset) = 0;

  // 得到中心
  //
  // @return vec2: 中心坐标
  virtual vec2 GetCenter() const = 0;

  // 针对被检测的图形添加轴(圆)
  //
  // @param axis: 额外的轴
  virtual void SetExtraAxis(const vec2& axis) = 0;
};

// 点碰撞盒
struct DotBox : public HitBox {
  explicit DotBox(const vec2& pos) : Pos(pos) {}
  ~DotBox() {}

  std::vector<vec2> GetSeparateAxis() const override;
  Projection        GetProjectionOnAxis(const vec2& axis) const override;
  DotBox&           Translate(const vec2& offset) override;
  vec2              GetCenter() const override { return Pos; }
  void              SetExtraAxis(const vec2& axis) override {}

  // 以当前点为基础生成一个新的点
  //
  // @param offset: 偏移量
  // @return DotBox: 新的点盒
  DotBox Add(const vec2& offset) const;

  vec2 Pos;  // 点的位置
};

// 线碰撞盒
struct LineBox : public HitBox {
  LineBox(const vec2& begin, const vec2& end) : Begin(begin), End(end) {}
  ~LineBox() {}

  std::vector<vec2> GetSeparateAxis() const override;
  Projection        GetProjectionOnAxis(const vec2& axis) const override;
  LineBox&          Translate(const vec2& offset) override;
  vec2              GetCenter() const override { return (Begin + End) * 0.5f; }
  void              SetExtraAxis(const vec2& axis) override {}

  // 以当前点为基础生成一个新的线
  //
  // @param offset: 偏移量
  // @return LineBox: 新的线盒
  LineBox Add(const vec2& offset) const;

  vec2 Begin;  // 起点
  vec2 End;    // 终点
};

// 凸多边形碰撞盒
struct PolygonBox : public HitBox {
  explicit PolygonBox(std::initializer_list<vec2> list) : Vertices(list) {}
  explicit PolygonBox(const std::vector<vec2>& vec) : Vertices(vec) {}
  ~PolygonBox() {}

  std::vector<vec2> GetSeparateAxis() const override;
  Projection        GetProjectionOnAxis(const vec2& axis) const override;
  PolygonBox&       Translate(const vec2& offset) override;
  vec2              GetCenter() const override;
  void              SetExtraAxis(const vec2& axis) override {}

  // 以当前点为基础生成一个新的多边形
  //
  // @param offset: 偏移量
  // @return PolygonBox: 新的多边形盒
  PolygonBox Add(const vec2& offset) const;

  std::vector<vec2> Vertices;  // 顶点集合
};

// 圆碰撞盒
struct CircleBox : public HitBox {
  CircleBox(const vec2& origin, float radius)
      : Origin(origin), Radius(radius) {}
  ~CircleBox() {}

  std::vector<vec2> GetSeparateAxis() const override;
  Projection        GetProjectionOnAxis(const vec2& axis) const override;
  CircleBox&        Translate(const vec2& offset) override;
  vec2              GetCenter() const override { return Origin; }
  void              SetExtraAxis(const vec2& axis) override { _axis = axis; }

  // 以当前点为基础生成一个新的圆
  //
  // @param offset: 偏移量
  // @return PolygonBox: 新的圆盒
  CircleBox Add(const vec2& offset) const;

  vec2  Origin;  // 圆心位置
  float Radius;  // 半径
 private:
  vec2 _axis;  // 额外轴
};

// 碰撞检测算法, 返回的MTV代表第一个物体的移动向量
//
// @param lhs: 第一个物体
// @param rhs: 第二个物体
CollisionResult CheckCollision(const HitBox& lhs, const HitBox& rhs);

// 点点检测(速度更快)
CollisionResult CheckCollision(const DotBox& lhs, const DotBox& rhs);

// 圆圆检测
CollisionResult CheckCollision(CircleBox& lhs, CircleBox& rhs);

// 点圆检测
CollisionResult CheckCollision(const DotBox& lhs, CircleBox& rhs);
CollisionResult CheckCollision(CircleBox& lhs, const DotBox& rhs);

// 线圆检测
CollisionResult CheckCollision(const LineBox& lhs, CircleBox& rhs);
CollisionResult CheckCollision(CircleBox& lhs, const LineBox& rhs);

// 多边形圆检测
CollisionResult CheckCollision(const PolygonBox& lhs, CircleBox& rhs);
CollisionResult CheckCollision(CircleBox& lhs, const PolygonBox& rhs);

}  // namespace physics
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_PHYSICS_COLLISION_H__
