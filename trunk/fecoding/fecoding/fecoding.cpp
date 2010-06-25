#include "fecoding.h"
#if PLATFORM == PLATFORM_WINDOWS
#include <Windows.h>
#endif

typedef signed char		int8;
typedef unsigned char	uint8;
typedef short			int16;
typedef unsigned short	uint16;
typedef int				int32;
typedef unsigned int	uint32;

size_t utf8length(std::string& utf8str)
{
	try
	{
		return utf8::distance(utf8str.c_str(),utf8str.c_str()+utf8str.size());
	}
	catch(std::exception)
	{
		utf8str = "";
		return 0;
	}
}

void utf8truncate(std::string& utf8str,size_t len)
{
	try
	{
		size_t wlen = utf8::distance(utf8str.c_str(),utf8str.c_str()+utf8str.size());
		if(wlen <= len)
			return;

		std::wstring wstr;
		wstr.resize(wlen);
		utf8::utf8to16(utf8str.c_str(),utf8str.c_str()+utf8str.size(),&wstr[0]);
		wstr.resize(len);
		char* oend = utf8::utf16to8(wstr.c_str(),wstr.c_str()+wstr.size(),&utf8str[0]);
		utf8str.resize(oend-(&utf8str[0]));                 // remove unused tail
	}
	catch(std::exception)
	{
		utf8str = "";
	}
}

bool Utf8toWStr(char const* utf8str, size_t csize, wchar_t* wstr, size_t& wsize)
{
	try
	{
		size_t len = utf8::distance(utf8str,utf8str+csize);
		if(len > wsize)
		{
			if(wsize > 0)
				wstr[0] = L'\0';
			wsize = 0;
			return false;
		}

		wsize = len;
		utf8::utf8to16(utf8str,utf8str+csize,wstr);
		wstr[len] = L'\0';
	}
	catch(std::exception)
	{
		if(wsize > 0)
			wstr[0] = L'\0';
		wsize = 0;
		return false;
	}

	return true;
}

bool Utf8toWStr(const std::string& utf8str, std::wstring& wstr)
{
	try
	{
		size_t len = utf8::distance(utf8str.c_str(),utf8str.c_str()+utf8str.size());
		wstr.resize(len);

		utf8::utf8to16(utf8str.c_str(),utf8str.c_str()+utf8str.size(),&wstr[0]);
	}
	catch(std::exception)
	{
		wstr = L"";
		return false;
	}

	return true;
}

bool WStrToUtf8(wchar_t* wstr, size_t size, std::string& utf8str)
{
	try
	{
		std::string utf8str2;
		utf8str2.resize(size*4);                            // allocate for most long case

		char* oend = utf8::utf16to8(wstr,wstr+size,&utf8str2[0]);
		utf8str2.resize(oend-(&utf8str2[0]));               // remove unused tail
		utf8str = utf8str2;
	}
	catch(std::exception)
	{
		utf8str = "";
		return false;
	}

	return true;
}

bool WStrToUtf8(std::wstring wstr, std::string& utf8str)
{
	try
	{
		std::string utf8str2;
		utf8str2.resize(wstr.size()*4);                     // allocate for most long case

		char* oend = utf8::utf16to8(wstr.c_str(),wstr.c_str()+wstr.size(),&utf8str2[0]);
		utf8str2.resize(oend-(&utf8str2[0]));                // remove unused tail
		utf8str = utf8str2;
	}
	catch(std::exception)
	{
		utf8str = "";
		return false;
	}

	return true;
}

bool utf8ToConsole(const std::string& utf8str, std::string& conStr)
{
#if PLATFORM == PLATFORM_WINDOWS
	std::wstring wstr;
	if(!Utf8toWStr(utf8str,wstr))
		return false;

	conStr.resize(wstr.size());
	CharToOemBuffW(&wstr[0],&conStr[0],wstr.size());
#else
	// not implemented yet
	conStr = utf8str;
#endif

	return true;
}

bool consoleToUtf8(const std::string& conStr,std::string& utf8str)
{
#if PLATFORM == PLATFORM_WINDOWS
	std::wstring wstr;
	wstr.resize(conStr.size());
	OemToCharBuffW(&conStr[0],&wstr[0],conStr.size());

	return WStrToUtf8(wstr,utf8str);
#else
	// not implemented yet
	utf8str = conStr;
	return true;
#endif
}

bool Utf8FitTo(const std::string& str, std::wstring search)
{
	std::wstring temp;

	if(!Utf8toWStr(str,temp))
		return false;

	// converting to lower case
	wstrToLower( temp );

	if(temp.find(search) == std::wstring::npos)
		return false;

	return true;
}

void utf8printf(FILE *out, const char *str, ...)
{
	va_list ap;
	va_start(ap, str);
	vutf8printf(stdout, str, &ap);
	va_end(ap);
}

void vutf8printf(FILE *out, const char *str, va_list* ap)
{
#if PLATFORM == PLATFORM_WINDOWS
	char temp_buf[32*1024];
	wchar_t wtemp_buf[32*1024];

	size_t temp_len = vsnprintf(temp_buf, 32*1024, str, *ap);

	size_t wtemp_len = 32*1024-1;
	Utf8toWStr(temp_buf, temp_len, wtemp_buf, wtemp_len);

	CharToOemBuffW(&wtemp_buf[0], &temp_buf[0], wtemp_len+1);
	fprintf(out, temp_buf);
#else
	vfprintf(out, str, *ap);
#endif
}






inline wchar_t wcharToUpper(wchar_t wchar)
{
	if(wchar >= L'a' && wchar <= L'z')                      // LATIN SMALL LETTER A - LATIN SMALL LETTER Z
		return wchar_t(uint16(wchar)-0x0020);
	if(wchar == 0x00DF)                                     // LATIN SMALL LETTER SHARP S
		return wchar_t(0x1E9E);
	if(wchar >= 0x00E0 && wchar <= 0x00F6)                  // LATIN SMALL LETTER A WITH GRAVE - LATIN SMALL LETTER O WITH DIAERESIS
		return wchar_t(uint16(wchar)-0x0020);
	if(wchar >= 0x00F8 && wchar <= 0x00FE)                  // LATIN SMALL LETTER O WITH STROKE - LATIN SMALL LETTER THORN
		return wchar_t(uint16(wchar)-0x0020);
	if(wchar >= 0x0101 && wchar <= 0x012F)                  // LATIN SMALL LETTER A WITH MACRON - LATIN SMALL LETTER I WITH OGONEK (only %2=1)
	{
		if(wchar % 2 == 1)
			return wchar_t(uint16(wchar)-0x0001);
	}
	if(wchar >= 0x0430 && wchar <= 0x044F)                  // CYRILLIC SMALL LETTER A - CYRILLIC SMALL LETTER YA
		return wchar_t(uint16(wchar)-0x0020);
	if(wchar == 0x0451)                                     // CYRILLIC SMALL LETTER IO
		return wchar_t(0x0401);

	return wchar;
}

inline wchar_t wcharToLower(wchar_t wchar)
{
	if(wchar >= L'A' && wchar <= L'Z')                      // LATIN CAPITAL LETTER A - LATIN CAPITAL LETTER Z
		return wchar_t(uint16(wchar)+0x0020);
	if(wchar >= 0x00C0 && wchar <= 0x00D6)                  // LATIN CAPITAL LETTER A WITH GRAVE - LATIN CAPITAL LETTER O WITH DIAERESIS
		return wchar_t(uint16(wchar)+0x0020);
	if(wchar >= 0x00D8 && wchar <= 0x00DE)                  // LATIN CAPITAL LETTER O WITH STROKE - LATIN CAPITAL LETTER THORN
		return wchar_t(uint16(wchar)+0x0020);
	if(wchar >= 0x0100 && wchar <= 0x012E)                  // LATIN CAPITAL LETTER A WITH MACRON - LATIN CAPITAL LETTER I WITH OGONEK (only %2=0)
	{
		if(wchar % 2 == 0)
			return wchar_t(uint16(wchar)+0x0001);
	}
	if(wchar == 0x1E9E)                                     // LATIN CAPITAL LETTER SHARP S
		return wchar_t(0x00DF);
	if(wchar == 0x0401)                                     // CYRILLIC CAPITAL LETTER IO
		return wchar_t(0x0451);
	if(wchar >= 0x0410 && wchar <= 0x042F)                  // CYRILLIC CAPITAL LETTER A - CYRILLIC CAPITAL LETTER YA
		return wchar_t(uint16(wchar)+0x0020);

	return wchar;
}