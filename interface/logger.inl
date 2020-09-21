#ifndef __LOGGER_INL__
#define __LOGGER_INL__

namespace simulator
{

template <typename Arg, typename... Args>
void logger::log(level level, const char *file, const uint32_t line,
                 const char *function, Arg &&arg, Args &&... args)
{
   const std::lock_guard<std::mutex> lock(mMutex);
   assert(mStream.is_open());
   mStream << now() << " " << level_to_string(level) << ": ";
   mStream << file << "(" << line << "," << function << "()): ";
   mStream << std::forward<Arg>(arg);
   ((mStream << std::forward<Args>(args)), ...);
   mStream << std::endl;
}

} // namespace simulator

#endif
