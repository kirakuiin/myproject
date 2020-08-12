// config.cc - 配置器实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 11
// License: GPL.v3

#include "tools/config.h"

#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <sstream>

namespace easy_engine {
namespace tools {

void Configurator::LoadConfig(const std::string &file_path) {
  std::ifstream     config_file(file_path);
  std::stringstream file_streamer;
  if (config_file) {
    file_streamer << config_file.rdbuf();
    boost::property_tree::read_json(file_streamer, _elements);
  }
  config_file.close();
}

void Configurator::SaveConfig(const std::string &file_path) {
  std::stringstream elem_streamer;
  boost::property_tree::write_json(elem_streamer, _elements);
  std::ofstream config_file(file_path);
  if (config_file) {
    config_file << elem_streamer.str();
  }
  config_file.close();
}

}  // namespace tools
}  // namespace easy_engine
