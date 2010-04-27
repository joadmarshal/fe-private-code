#include <time.h>
int GetMaxDayfromMonth(int year,int month);//指定年月，得出该月天数

void TimetToSystemTime( time_t t, LPSYSTEMTIME pst )
{
	FILETIME ft;

	LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
	ft.dwLowDateTime = (DWORD) ll;
	ft.dwHighDateTime = (DWORD)(ll >> 32);

	FileTimeToSystemTime( &ft, pst );
}

void SystemTimeToTimet( SYSTEMTIME st, time_t *pt )
{
	FILETIME ft;
	SystemTimeToFileTime( &st, &ft );

	LONGLONG ll;

	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;

	ll = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;

	*pt = (DWORD)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
}


void FileTimeToTime_t(  FILETIME  ft,  time_t  *t  )  
{  
	LONGLONG  ll;  

	ULARGE_INTEGER            ui;  
	ui.LowPart            =  ft.dwLowDateTime;  
	ui.HighPart            =  ft.dwHighDateTime;  

	ll            =  ft.dwHighDateTime  <<  32  +  ft.dwLowDateTime;  

	*t            =  ((LONGLONG)(ui.QuadPart  -  116444736000000000)  /  10000000);  
}
