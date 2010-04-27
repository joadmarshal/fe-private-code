#ifndef		__WinService__
#define		__WinService__
#include <windows.h>
class WinService
{
public:
	WinService(char *servicename,char *displayname,char *description)
	{
		if(_service==NULL)
		{
			_service=this;
			strncpy(_servername,servicename,1024);
			strncpy(_serverdisplayname,displayname,1024);
			strncpy(_serviceDescription,description,1024);
		}
	}
	BOOL startservice(int argc, char* argv[]);
	BOOL stopservice();
	~WinService()
	{
		if(_service==this)
		{
			_service=NULL;
		}
	}
	virtual BOOL OnStop() {return TRUE;}
	virtual BOOL OnStart() {return TRUE;}
	static WinService *GetService() {return _service;};
protected:
	static void LogEvent(LPCTSTR pFormat, ...);
private:

	void run();//主线程持续等待
	BOOL Install();
	BOOL IsInstalled();
	BOOL Uninstall();
	static void WINAPI _ServiceMain(
								DWORD dwArgc,     // number of arguments
								LPTSTR *lpszArgv  // array of arguments
								);	
	static void WINAPI _ServiceStrl(DWORD dwOpcode);
	

private:
	HANDLE _hStopEvent_Winservice;
	static char _servername[];
	static char _serverdisplayname[];
	static char _serviceDescription[];
	static SERVICE_TABLE_ENTRY _st[2];
	static WinService *_service;
	static SERVICE_STATUS _status;
	static SERVICE_STATUS_HANDLE _hServiceStatus;
	static BOOL _bInstall;
	static DWORD _dwThreadID;
};
#endif

