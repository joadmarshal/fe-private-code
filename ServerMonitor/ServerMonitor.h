#include "winservice.h"
#include <time.h>
#define   SystemBasicInformation   0   
#define   SystemPerformanceInformation   2   
#define   SystemTimeInformation   3   
enum dbstate
{
	dbstate_success = 0,
	dbstate_dbConnectError = 1,
	dbstate_dbsettingerror = 2,
	dbstate_dbGetFreeDiskError = 3
};
#define  DAYSECOND  (24*60*60)
enum timertasktype
{
	type_none = -1,
	type_once = 0,
	type_everyday,
	type_weekday, 
	type_everymonth
};

enum weekday
{
	weekday_Sunday = 0,
	weekday_Monday,
	weekday_Tuesday,
	weekday_Wednesday,
	weekday_Thursday,
	weekday_Friday, 
	weekday_Saturday
};

class timertask
{
public:
	timertask():tasktype(0),weekdayMask(0),monthday(0){}
	std::list<std::string> commandlist;
	typedef std::list<std::string>::iterator commandite;
	int tasktype;
	int weekdayMask;
	int monthday;
	tm tmsetting;
	time_t nextTime;//下次任务时间
	std::string taskname;
	bool isInvalidTask(){return tasktype == type_none;}
	bool operator  < (timertask& rhd) 
	{
        return  nextTime  <  rhd.nextTime;
	}
	void runcommand()
	{
		//do command;
		for (commandite ite=commandlist.begin();ite!=commandlist.end();ite++)
		{
			gLogger.debug("[ServerTimerThread] run command %s",ite->c_str());
			system(ite->c_str());
			Sleep(1000);
		}
	}
	void calculateNextTime()
	{
		time_t _t ;
		tm _t_tm ;
		int _addDay=0;
		_t = time(0);
		_t_tm = *localtime(&_t);

		switch (tasktype)
		{
		case type_once:
			nextTime = mktime(&tmsetting);
			if(nextTime == -1)
			{
				gLogger.error("[calculateNextTime] task : %s  error timesetting",taskname.c_str());
				tasktype = type_none;
			}
			else if(nextTime < _t)
			{
				gLogger.info("[calculateNextTime] task : %s  is Invalid",taskname.c_str());
				tasktype = type_none;
			}

			break;
		case type_everyday:
			_t_tm.tm_hour = tmsetting.tm_hour;
			_t_tm.tm_min = tmsetting.tm_min;
			_t_tm.tm_sec = tmsetting.tm_sec;
			nextTime = mktime(&_t_tm);
			nextTime = nextTime >= _t ? nextTime : nextTime+DAYSECOND;
			break;
		case type_weekday:
			if(weekdayMask<0)
			{
				gLogger.error("[calculateNextTime] task : %s  error weekdayMask",taskname.c_str());
				tasktype = type_none;
			}
			else
			{
				_t_tm.tm_hour = tmsetting.tm_hour;
				_t_tm.tm_min = tmsetting.tm_min;
				_t_tm.tm_sec = tmsetting.tm_sec;
				while (true)
				{
					int testday = (_addDay + _t_tm.tm_wday)%7;
					bool bhit=(weekdayMask>>testday) &0x1;
					if(bhit)
					{
						nextTime = _addDay * DAYSECOND + mktime(&_t_tm);
						if(nextTime>=_t)//找到下次运行时间了.
							break;
					}
					++_addDay;
				}
			}
			
			break;
		case type_everymonth:
			if(monthday <=0)
			{
				gLogger.error("[calculateNextTime] task : %s  error monthday",taskname.c_str());
				tasktype = type_none;
			}
			else
			{
				int __monthday=monthday> 31?31:monthday;
				tm __temp = _t_tm;
				__temp.tm_mday = monthday;
				__temp.tm_hour = tmsetting.tm_hour;
				__temp.tm_min = tmsetting.tm_min;
				__temp.tm_sec = tmsetting.tm_sec;
				nextTime = mktime(&__temp);
				while(nextTime < _t)
				{
					if(__temp.tm_mday<=_t_tm.tm_mday)
					{
						if(__temp.tm_mon>=11)
						{
							__temp.tm_mon = 0;
							++__temp.tm_year; 
						}
						else
							++__temp.tm_mon;
					}
					else
						--__temp.tm_mday;//可能monthday爆了

					nextTime = mktime(&__temp);
				}
			}
			break;
		}
	}
};

class ServerMonitor : public WinService
{
public:
	ServerMonitor(){};
private:
	//服务触发事件
	virtual BOOL OnStop() ;
	virtual BOOL OnStart() ;

	void HandleCommand(IN OUT char *msgbuff,IN OUT DWORD *len);
	int GetCpuStatus();
	void GetMemStatus(DWORD *pdwFreeMem,DWORD *pdwTotalMem);
	void GetUseDisk(OUT DWORD* pdwAllsize,OUT DWORD *pdwFreesize);
	void GetDBUseDisk(OUT DWORD* pdbhostip,OUT DWORD *freesize,OUT DWORD *dbstate);
	BOOL GetServerStatus(OUT char *buff,OUT DWORD *len);
	BOOL init();
	std::string GetServiceList();//
	__int64 GetNUMFromSeparateStr(const char *srcstr);
	HANDLE _hstop;
	HANDLE _hmainListenThread;
	HANDLE _hCompletionPort;
	HANDLE _hmainTimerThread;

	std::list<timertask> timertasklist;
	typedef std::list<timertask>::iterator taskiterator;
	void initTimerTaskList();
	static DWORD WINAPI ServerMonitorListenThread(LPVOID lpParam);	
	static DWORD WINAPI TcpDataThread(LPVOID lParam);
	static DWORD WINAPI ServerTimerThread(LPVOID lpParam);
};




#define   Li2Double(x)   ((double)((x).HighPart)   *   4.294967296E9   +   (double)((x).LowPart))   

typedef   struct   
{   
	DWORD   dwUnknown1;   
	ULONG   uKeMaximumIncrement;   
	ULONG   uPageSize;   
	ULONG   uMmNumberOfPhysicalPages;   
	ULONG   uMmLowestPhysicalPage;   
	ULONG   uMmHighestPhysicalPage;   
	ULONG   uAllocationGranularity;   
	PVOID   pLowestUserAddress;   
	PVOID   pMmHighestUserAddress;   
	ULONG   uKeActiveProcessors;   
	BYTE   bKeNumberProcessors;   
	BYTE   bUnknown2;   
	WORD   wUnknown3;   
}   SYSTEM_BASIC_INFORMATION;   

typedef   struct   
{   
	LARGE_INTEGER   liIdleTime;   
	DWORD   dwSpare[76];   
}   SYSTEM_PERFORMANCE_INFORMATION;   

typedef   struct   
{   
	LARGE_INTEGER   liKeBootTime;   
	LARGE_INTEGER   liKeSystemTime;   
	LARGE_INTEGER   liExpTimeZoneBias;   
	ULONG   uCurrentTimeZoneId;   
	DWORD   dwReserved;   
}   SYSTEM_TIME_INFORMATION;   

typedef   LONG   (WINAPI   *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);   
PROCNTQSI   NtQuerySystemInformation;   


typedef struct
{
	OVERLAPPED Overlapped;
	WSABUF DataBuf;
	char Buffer[1024];
	DWORD totalBytes;
	DWORD BytesRECV;
	SOCKET socket;
	char *data;
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;




