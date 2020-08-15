// config.h - 配置器
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 11
// License: GPL.v3

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <map>
#include <string>

#ifndef __EASY_ENGINE_INCLUDE_UTILITY_CONFIG_H__
#define __EASY_ENGINE_INCLUDE_UTILITY_CONFIG_H__

namespace easy_engine {
namespace utility {

using boost::property_tree::ptree;

// 配置器, 用来存储读取配置文件
class Configurator {
 public:
  Configurator() {}
  virtual ~Configurator() {}

  // 为指定的ptree设定一个简单值
  // @param key: 配置项名称
  // @param value: 配置项的值
  // @param tree: ptree的指针
  template <typename T>
  static void Set(const std::string& key, const T& value, ptree* tree) {
    // put, 有则改之, 无则加勉. add, 添加
    tree->put(key, value);
  }

  // 为指定的ptree设定一个子向量
  // @param key: 配置项名称
  // @param vec: 值向量
  // @param tree: ptree的指针
  template <typename T>
  static void SetVector(const std::string& key, const std::vector<T>& vec,
                        ptree* tree) {
    ptree vector_obj;
    for (const auto& val : vec) {
      ptree unit;
      unit.put_value(val);
      vector_obj.push_back(std::make_pair("", unit));
    }
    tree->put_child(key, vector_obj);
  }

  // 为指定的ptree设定一个子对象
  // @param key: 配置项名称
  // @param obj: 子对象
  // @param tree: ptree的指针
  static void SetObject(const std::string& key, const ptree& obj, ptree* tree) {
    tree->put_child(key, obj);
  }

  // 从指定的ptree获得一个简单值
  // @param tree: ptree对象
  // @param key: 配置项名称
  // @return:    返回项目项的值
  template <typename T>
  static T Get(const ptree& tree, const std::string& key) {
    return tree.get<T>(key);
  }

  // 从指定的ptree获得一个子向量
  // @param tree: ptree对象
  // @param key: 配置项名称
  // @param vec: 子向量的指针
  template <typename T>
  static void GetVector(const ptree& tree, const std::string& key,
                        std::vector<T>* vec) {
    for (const auto& pt : tree.get_child(key)) {
      vec->push_back(boost::lexical_cast<T>(pt.second.data()));
    }
  }

  // 从指定的ptree获得一个子对象
  // @param tree: ptree对象
  // @param key: 配置项名称
  // @return: ptree对象
  static ptree GetObject(const ptree& tree, const std::string& key) {
    return tree.get_child(key);
  }

  // 将配置信息读取到内存中
  // @param file_path: 文件的路径
  void LoadConfig(const std::string& file_path);

  // 将配置信息存储到文件中
  // @param file_path: 文件的路径
  void SaveConfig(const std::string& file_path);

 protected:
  // 存储所有键值对的容器
  ptree _elements;
};

// 扁平配置器, 所有的配置项都会存储在同一层级, 互为兄弟
class FlatConfigurator : public Configurator {
 public:
  // 设定某一个配置项具体的值, 不存在则插入
  // @param key: 配置项名称
  // @param value: 配置项的值
  template <typename T>
  void SetValue(const std::string& key, const T& value) {
    Set(key, value, &_elements);
  }

  // 获得某一个项的具体值
  // @param key: 配置项名称
  // @return: 配置项的值
  template <typename T>
  T GetValue(const std::string& key) {
    return Get<T>(_elements, key);
  }
};

}  // namespace utility
}  // namespace easy_engine

#endif // __EASY_ENGINE_INCLUDE_UTILITY_CONFIG_H__
