// log.cc - 日志记录器实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 11
// License: GPL.v3

#include "log.h"

#include <boost/thread/lock_types.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <iostream>

namespace {
const std::string kGlobalLoggerName("@global@");

using ReadLock  = boost::shared_lock<boost::shared_mutex>;
using WriteLock = boost::unique_lock<boost::shared_mutex>;
boost::shared_mutex Mutex;
}  // namespace

namespace easy_engine {
namespace tools {

std::map<std::string, std::shared_ptr<Logging>> Logging::_sm_loggers;
std::shared_ptr<Logging>                        Logging::_sp_global_logger;

std::shared_ptr<Logging> Logging::GetLogger(const std::string &module_name) {
  if (module_name.empty()) {
    if (!_sp_global_logger.get()) {
      _sp_global_logger.reset(new Logging(kGlobalLoggerName));
    }
    return _sp_global_logger;
  } else {
    if (!_sm_loggers.count(module_name)) {
      _sm_loggers[module_name] =
          std::shared_ptr<Logging>(new Logging(module_name));
    }
    return _sm_loggers[module_name];
  }
}

Logging::Logging(const std::string &module_name)
    : _level(LoggingLevel::DEBUG),
      _enable_time(false),
      _module_name(module_name),
      _time_fmt("[%s %s]"),
      _output_fmt("%s") {}

void Logging::Config(LoggingLevel level, bool enable_time,
                     const std::string &file_path, const std::string &fmt) {
  WriteLock write(Mutex);
  _enable_time = enable_time;
  _level       = level;
  _output_fmt  = boost::format(fmt);
  if (file_path.empty()) {
    _output_file.close();
  } else {
    if (_output_file.is_open()) {
      _output_file.close();
    }
    _output_file.open(file_path, std::ios::app | std::ios::out);
  }
}

void Logging::Clear() {
  WriteLock write(Mutex);
  _sp_global_logger = nullptr;
  _sm_loggers.clear();
}

void Logging::Debug(const std::string &msg) {
  if (static_cast<int>(_level) <= static_cast<int>(LoggingLevel::DEBUG)) {
    Output(msg);
  }
}

void Logging::Info(const std::string &msg) {
  if (static_cast<int>(_level) <= static_cast<int>(LoggingLevel::INFO)) {
    Output(msg);
  }
}

void Logging::Warn(const std::string &msg) {
  if (static_cast<int>(_level) <= static_cast<int>(LoggingLevel::WARN)) {
    Output(msg);
  }
}

void Logging::Error(const std::string &msg) {
  if (static_cast<int>(_level) <= static_cast<int>(LoggingLevel::ERROR)) {
    Output(msg);
  }
}

void Logging::Critical(const std::string &msg) {
  if (static_cast<int>(_level) <= static_cast<int>(LoggingLevel::CRITICAL)) {
    Output(msg);
  }
}

void Logging::Output(const std::string &msg) {
  ReadLock read(Mutex);

  std::string time_prefix;
  if (_enable_time) {
    boost::posix_time::ptime now =
        boost::posix_time::second_clock::local_time();
    _time_fmt % now.date() % now.time_of_day();
    time_prefix = _time_fmt.str();
  }
  std::string output("[" + _module_name + "]");
  _output_fmt % msg;
  output = time_prefix + output + _output_fmt.str();
  if (_output_file && _output_file.is_open()) {
    _output_file << output << std::endl;
  } else {
    std::cout << output << std::endl;
  }
}

}  // namespace tools
}  // namespace easy_engine
