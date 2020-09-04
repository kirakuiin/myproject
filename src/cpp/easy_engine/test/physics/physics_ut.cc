// physics_ut.cc - 物理ut
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 25
// License: GPL.v3

#include <gtest/gtest.h>

#include <ctime>
#include <iostream>
#include <memory>
#include <string>

#include "common/quadtree.h"

#define __EASY_ENGINE_PHYSICS__
#define __EASY_ENGINE_UTILITY__
#define __EASY_ENGINE_COMMON__
#include "easy_engine.h"

using namespace easy_engine;
using namespace easy_engine::utility;
using namespace easy_engine::physics;
using std::cout;
using std::endl;

namespace {}  // namespace

class PHYUT : public testing::Test {
 public:
  static void SetUpTestCase() { config.LoadConfig("phyut.json"); }
  static void TearDownTestCase() {}
  void        SetUp() override {}
  void        TearDown() override {}

  static FlatConfigurator config;  // 配置读取
};
FlatConfigurator PHYUT::config;

TEST_F(PHYUT, CollisionTest) {
  DotBox  d0(vec2(0, 0));
  LineBox l0(vec2(3, 0), vec2(-3, 0.1));
  LineBox l1(l0.Add(vec2(3, 0)));
  cout << Format("line separate axis = %", l0.GetSeparateAxis()[0]) << endl;
  Projection pl0(l0.GetProjectionOnAxis(l0.GetSeparateAxis()[0]));
  cout << Format("proj = (% -> %)", pl0.Min, pl0.Max) << endl;
  PolygonBox p0{vec2(0, 1), vec2(1, 0), vec2(2, 1), vec2(1, 2)};
  PolygonBox p1{vec2(2, 3), vec2(3, 0), vec2(5, 0), vec2(6, 3), vec2(4, 6)};
  cout << Format("p1's AABB = Rect(%, %)", p1.GetAABB().Pos, p1.GetAABB().Size)
       << endl;
  for (const auto& iter : p0.GetSeparateAxis()) {
    cout << Format("polygon separate axis = %", iter) << endl;
  }
  Projection pp0(p0.GetProjectionOnAxis(l0.GetSeparateAxis()[0]));
  cout << Format("polygon proj on axis=% is (%->%)", l0.GetSeparateAxis()[0],
                 pp0.Min, pp0.Max)
       << endl;
  CircleBox c0(vec2(0), 1);
  CircleBox c1(vec2(4.1, 5.9), 1);
  auto      res = CheckCollision(c0, l0);
  cout << Format("collision=%, mtv=%", res.IsIntersect, res.MTV) << endl;
}

TEST_F(PHYUT, QuadTreeTest) {
  QuadTree<int> tree(1, Rect(vec2(0, 0), vec2(100, 100)));
  tree.Insert(1, Rect(vec2(0), vec2(10)));
  tree.Insert(2, Rect(vec2(30), vec2(10)));
  tree.Insert(3, Rect(vec2(60), vec2(10)));
  tree.Insert(4, Rect(vec2(90), vec2(10)));
  tree.Insert(5, Rect(vec2(0, 90), vec2(10)));
  tree.Insert(6, Rect(vec2(30, 60), vec2(10)));
  tree.Insert(7, Rect(vec2(60, 30), vec2(10)));
  tree.Insert(8, Rect(vec2(90, 0), vec2(10)));
  tree.Insert(9, Rect(vec2(25), vec2(50)));
  tree.Insert(10, Rect(vec2(50), vec2(50)));
  cout << tree.TreeStruct() << endl;
  auto res = tree.Query(Rect(vec2(11), vec2(90)));
  cout << Format("intersect=%", res) << endl;
  tree.Remove(1);
  tree.Remove(2);
  tree.Remove(9);
  cout << tree.TreeStruct() << endl;
  res = tree.Query(Rect(vec2(11), vec2(90)));
  cout << Format("intersect=%", res) << endl;
}
