#include "stringEx.h"


void string_replace(std::string& str, const char* szPre, const char* szPost)
{
	std::string::size_type pos;
	pos = str.find(szPre);
	size_t length = strlen(szPre);
	size_t postlen = strlen(szPost);
	while( pos != std::string::npos )
	{
		str.replace(pos, length, szPost);
		pos = str.find(szPre, pos+postlen);
	}
}


std::vector<std::string> string_split(const std::string& src, std::string delimit,std::string null_subst)   
{   
	if( src.empty() || delimit.empty() ) throw "split:empty string\0";   
	std::vector<std::string> v;   
	std::basic_string<char>::size_type deli_len = delimit.size();
	long index = -1, last_search_position = 0;
	while( (index=src.find(delimit,last_search_position))!=-1 )
	{   
		if(index==last_search_position)   
			v.push_back(null_subst);   
		else  
			v.push_back( src.substr(last_search_position, index-last_search_position) );   
		last_search_position = index + deli_len;   
	}   
	std::string last_one = src.substr(last_search_position);   
	v.push_back( last_one.empty()? null_subst:last_one );   
	return v;   
}

std::string IPCoverNumberToString(unsigned long uhostformat)
{
	char result[16];
	sprintf(result,"%u.%u.%u.%u",uhostformat>>24,uhostformat>>16&0x000000FF,uhostformat>>8&0x000000FF,uhostformat&0x000000FF);
	std::string s_r=result;
	return s_r;
}