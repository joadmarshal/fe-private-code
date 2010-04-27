#ifndef __stringEx_fe__
#define __stringEx_fe__
#include <string>
#include <vector>
void string_replace(std::string& str, const char* szPre, const char* szPost);
std::vector<std::string> string_split(const std::string& src, std::string delimit,std::string null_subst="");
std::string IPCoverNumberToString(unsigned long uhostformat);
#endif