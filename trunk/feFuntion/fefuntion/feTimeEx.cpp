#include "feTimeEx.h"
#include <memory.h>
int GetMaxDayfromMonth(int year,int month)
{
	//下一个月1号的时间差，和要求月的时间差，除以秒数，得到日子数
	time_t t_srcTime,t_desTime;
	tm tm_srctime,tm_desttime;

	memset(&tm_srctime,0,sizeof(tm_srctime));
	memset(&tm_desttime,0,sizeof(tm_desttime));
	tm_srctime.tm_year=year-1900;
	tm_srctime.tm_mon=month-1;
	tm_srctime.tm_mday=1;
	if(month==12)
	{
		tm_desttime.tm_year=tm_srctime.tm_year+1;
		tm_desttime.tm_mon=0;
	}
	else
	{
		tm_desttime.tm_year=tm_srctime.tm_year;
		tm_desttime.tm_mon=tm_srctime.tm_mon+1;
	}
	tm_desttime.tm_mday=1;
	t_srcTime=mktime(&tm_srctime);
	t_desTime=mktime(&tm_desttime);
	return (t_desTime-t_srcTime)/86400;
}