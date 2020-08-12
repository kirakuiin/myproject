// log.h - 日志记录器
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 11
// License: GPL.v3

#ifndef __CPP_EASY_ENGINE_INCLUDE_LOG_H__
#define __CPP_EASY_ENGINE_INCLUDE_LOG_H__

#include <boost/date_time.hpp>
#include <boost/format.hpp>
#include <fstream>
#include <map>
#include <memory>
#include <string>

namespace easy_engine {
namespace tools {

// 日志级别. 级别由低到高为DEBUG->CRITICAL
enum class LoggingLevel {
  DEBUG    = 0,  // 调试
  INFO     = 1,  // 信息
  WARN     = 2,  // 警告
  ERROR    = 3,  // 错误
  CRITICAL = 4,  // 致命
};

// 简单日志记录
// 只有等级大于日志等级的函数方能输出, 默认为DEBUG
class Logging {
 public:
  ~Logging(){};

  // 根据传入模块名返回对应模块的日志记录
  // @param module_name: 模块名称, 默认为空(对应全局日志记录)
  static std::shared_ptr<Logging> GetLogger(
      const std::string& module_name = "");

  // 配置日志的记录位置和日志等级. 如果不调用此函数, 默认输出日志到标准输出.
  // 仅有等级大于等于当前日志等级的函数才会输出.
  // @param level:       日志等级
  // @param enable_time: 日志之中是否包含当前时间, 默认为否
  // @param file_path:   日志记录的保存位置, 默认为空
  // @param fmt:         日志的输出格式化字符串, 只接受单%s格式, 默认为%s
  void Config(LoggingLevel level, bool enable_time = false,
              const std::string& file_path = "", const std::string& fmt = "%s");

  // 清除所有日志模块
  void Clear();

  // 输出调试级别的信息
  // @param msg: 信息内容
  void Debug(const std::string& msg);

  // 输出信息级别的信息
  // @param msg: 信息内容
  void Info(const std::string& msg);

  // 输出警告级别的信息
  // @param msg: 信息内容
  void Warn(const std::string& msg);

  // 输出错误级别的信息
  // @param msg: 信息内容
  void Error(const std::string& msg);

  // 输出致命级别的信息
  // @param msg: 信息内容
  void Critical(const std::string& msg);

 private:
  explicit Logging(const std::string& module_name);

  // 所有输出的底层实现
  void Output(const std::string& msg);

  // 存储全部模块的实例
  static std::map<std::string, std::shared_ptr<Logging>> _sm_loggers;
  static std::shared_ptr<Logging>                        _sp_global_logger;

  LoggingLevel _level;        // 日志级别
  bool         _enable_time;  // 是否记录时间
  std::string  _module_name;  // 模块名称

  boost::format _time_fmt;     // 时间格式
  boost::format _output_fmt;   // 输出格式
  std::ofstream _output_file;  // 输出文件
};

}  // namespace tools
}  // namespace easy_engine

#endif  // __CPP_EASY_ENGINE_INCLUDE_LOG_H__
