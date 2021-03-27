#ifndef __MOCK_LOGGER_H__
#define __MOCK_LOGGER_H__ 

#include <string>

#include "logger.h"


namespace simulator
{

// initialize static variables to default values
level logger::mLogLevel = INFO;
std::ofstream logger::mStream("");
std::mutex logger::mMutex;

// mock for get_log_level()
level logger::get_log_level()
{
   return INFO;
}

// mock for set_log_level()
void logger::set_log_level(level)
{
}

// mock for level_to_string()
std::string logger::level_to_string(level)
{
    return std::string {"[INFO]   "};
}

// mock for now()
std::string logger::now()
{
   return std::string{"2020-02-03 12:05:05"};
} 

} // namespace simulator

#endif