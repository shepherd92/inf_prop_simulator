#include <algorithm>
#include <cassert>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

#include "config.h"

namespace simulator
{

config::config() : mSections {}
{
}

void config::parse(const std::string &configFileName)
{

   static const std::regex comment_regex {R"x(\s*[;#])x"};
   static const std::regex section_regex {R"x(\s*\[([^\]]+)\])x"};
   static const std::regex value_regex {
       R"x(\s*(\S[^ \t=]*)\s*=\s*((\s?\S+)+)\s*$)x"};
   std::smatch pieces;

   std::ifstream inputStream(configFileName);
   assert(inputStream.good());

   std::string currentSection;

   for(std::string line; std::getline(inputStream, line);)
   {
      // skip empty lines
      if(line.empty() || std::regex_match(line, pieces, comment_regex))
      {
      }
      else if(std::regex_match(line, pieces, section_regex))
      {
         if(pieces.size() == 2)
         { // exactly one match
            currentSection = pieces[1].str();
         }
      }
      else if(std::regex_match(line, pieces, value_regex))
      {
         if(pieces.size() == 4)
         { // exactly enough matches
            mSections[currentSection][pieces[1].str()] = pieces[2].str();
         }
      }
   }
}

std::string config::get_value(const std::string &sectionName,
                              const std::string &keyName) const
{
   const auto foundSection = get_section(sectionName);
   const auto foundIterator = foundSection.find(keyName);

   assert(foundIterator != foundSection.end());
   return foundIterator->second;
}

const config::section &
config::get_section(const std::string &sectionName) const
{
   const auto foundIterator = mSections.find(sectionName);
   assert(foundIterator != mSections.end());
   return foundIterator->second;
}
} // namespace simulator
