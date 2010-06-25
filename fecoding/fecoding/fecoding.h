#ifndef  __FECODING__
#define __FECODING__


#include "utf8cpp/utf8.h"
#include <algorithm>

bool Utf8toWStr(const std::string& utf8str, std::wstring& wstr);
// in wsize==max size of buffer, out wsize==real string size
bool Utf8toWStr(char const* utf8str, size_t csize, wchar_t* wstr, size_t& wsize);
inline bool Utf8toWStr(const std::string& utf8str, wchar_t* wstr, size_t& wsize)
{
	return Utf8toWStr(utf8str.c_str(), utf8str.size(), wstr, wsize);
}

bool WStrToUtf8(std::wstring wstr, std::string& utf8str);
// size==real string size
bool WStrToUtf8(wchar_t* wstr, size_t size, std::string& utf8str);

size_t utf8length(std::string& utf8str);                    // set string to "" if invalid utf8 sequence
void utf8truncate(std::string& utf8str,size_t len);

bool utf8ToConsole(const std::string& utf8str, std::string& conStr);
bool consoleToUtf8(const std::string& conStr,std::string& utf8str);
bool Utf8FitTo(const std::string& str, std::wstring search);
void utf8printf(FILE *out, const char *str, ...);
void vutf8printf(FILE *out, const char *str, va_list* ap);

inline bool isNumeric(wchar_t wchar)
{
	return (wchar >= L'0' && wchar <=L'9');
}

inline bool isNumeric(char c)
{
	return (c >= '0' && c <='9');
}

inline bool isNumericOrSpace(wchar_t wchar)
{
	return isNumeric(wchar) || wchar == L' ';
}

inline bool isNumeric(char const* str)
{
	for(char const* c = str; *c; ++c)
		if (!isNumeric(*c))
			return false;

	return true;
}

inline bool isNumeric(std::string const& str)
{
	for(std::string::const_iterator itr = str.begin(); itr != str.end(); ++itr)
		if (!isNumeric(*itr))
			return false;

	return true;
}

inline bool isNumeric(std::wstring const& str)
{
	for(std::wstring::const_iterator itr = str.begin(); itr != str.end(); ++itr)
		if (!isNumeric(*itr))
			return false;

	return true;
}

inline wchar_t wcharToUpper(wchar_t wchar);

inline wchar_t wcharToLower(wchar_t wchar);

inline void wstrToUpper(std::wstring& str)
{
	std::transform( str.begin(), str.end(), str.begin(), wcharToUpper );
}

inline void wstrToLower(std::wstring& str)
{
	std::transform( str.begin(), str.end(), str.begin(), wcharToLower );
}

#endif // __FECODING__