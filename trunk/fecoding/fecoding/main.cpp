#include "fecoding.h"
#include <stdio.h>
int main()
{
	char *ff="��";
	wchar_t *wc=L"abc��";
	std::string ob;
	std::string aa;

	WStrToUtf8(wc,6,ob);

	utf8ToConsole(ob,aa);
	return 0;
}