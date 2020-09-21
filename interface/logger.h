#ifndef __I_LOGGER_H__
#define __I_LOGGER_H__

#include <cassert>
#include <cstdint>
#include <fstream>
#include <mutex>
#include <string>

#include "types.h"

namespace simulator
{

#ifdef LOGGING

#define LOG(level, ...)                                                        \
  if (level <= logger::get_log_level())                                        \
  logger::log(level, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#else

#define LOG(level, ...) do {} while(0)

#endif

class logger
{
public:
   template <typename Arg, typename... Args>
   static void log(level level, const char *file, const uint32_t line,
                   const char *function, Arg &&arg, Args &&... args);

   static void set_log_level(const level level);
   static level get_log_level();

   static void set_log_file(const std::string &logFile);

   static void close();

   static std::string level_to_string(level level);
   static level string_to_level(const std::string &string);

   // prevent copying
   logger(const logger &) = delete;
   logger &operator=(const logger &) = delete;

private:
   static std::string now();

   static level mLogLevel;
   static std::ofstream mStream;
   static std::mutex mMutex;
};
} // namespace simulator

#endif

#include "logger.inl"
