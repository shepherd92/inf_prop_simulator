#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <unordered_map>

namespace simulator
{

class config
{
public:
   typedef std::unordered_map<std::string, std::string> section;
   typedef std::unordered_map<std::string, section> sections;

   explicit config();
   void parse(const std::string &filename);
   std::string get_value(const std::string &sectionname,
                         const std::string &keyname) const;

private:
   const config::section &get_section(const std::string &sectionname) const;

   sections mSections;
};

} // namespace simulator

#endif

