#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <utility>

#include "logger.h"

namespace simulator {

// initialize static variables to default values
level logger::mLogLevel = INFO;
std::ofstream logger::mStream("");
std::mutex logger::mMutex;

void logger::set_log_file(const std::string &logFilename) {
  const std::lock_guard<std::mutex> lock(mMutex);
  close();
  mStream.open(logFilename, std::fstream::out | std::fstream::app);
}

void logger::close() {
  if (mStream.is_open()) {
    mStream.flush();
    mStream.close();
  }
}

level logger::string_to_level(const std::string &string) {
  if ("DEBUG" == string) {
    return DEBUG;
  }
  if ("INFO" == string) {
    return INFO;
  }
  if ("NOTICE" == string) {
    return NOTICE;
  }
  if ("WARNING" == string) {
    return WARNING;
  }
  if ("ERROR" == string) {
    return ERR;
  }
  if ("CRIT" == string) {
    return CRIT;
  }
  if ("ALERT" == string) {
    return ALERT;
  }
  if ("EMERG" == string) {
    return ALERT;
  }

  // unknown log level
  assert(false);
  return INVALID;
}

void logger::set_log_level(level level) {
  const std::lock_guard<std::mutex> lock(mMutex);
  mLogLevel = level;
}

level logger::get_log_level() { return mLogLevel; }

std::string logger::now() {
  const auto now = std::chrono::system_clock::now();
  const auto now_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %X");
  return ss.str();
}

std::string logger::level_to_string(level level) {
  switch (level) {
  case level::EMERG:
    return std::string("[EMERG]  ");
  case level::ALERT:
    return std::string("[ALERT]  ");
  case level::CRIT:
    return std::string("[CRIT]   ");
  case level::ERR:
    return std::string("[ERROR]  ");
  case level::WARNING:
    return std::string("[WARNING]");
  case level::NOTICE:
    return std::string("[NOTICE] ");
  case level::INFO:
    return std::string("[INFO]   ");
  case level::DEBUG:
    return std::string("[DEBUG]  ");
  default:
    return std::string();
  }
}
} // namespace simulator
