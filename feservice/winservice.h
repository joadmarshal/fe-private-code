#ifndef		__WinService__
#define		__WinService__
#include <windows.h>
class WinService
{
public:
	WinService(char *servicename)
	{
		ZeroMemory(_servername,sizeof(_servername));
		ZeroMemory(_serverdisplayname,sizeof(_serverdisplayname));
		ZeroMemory(_serviceDescription,sizeof(_serviceDescription));
		if(_service==NULL)
		{
			_service=this;
			strncpy(_servername,servicename,sizeof(_servername)-1);
		}
		else
		{
			//ASSERT(FALSE);
		}
	}
	void setDisplayname(const char *szdisplayname){strncpy(_serverdisplayname,szdisplayname,sizeof(_serviceDescription)-1);}
	void setDescription(const char *szdescription){strncpy(_serviceDescription,szdescription,sizeof(_serviceDescription)-1);}

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

	static void WINAPI ServiceMain(
								DWORD dwArgc,     // number of arguments
								LPTSTR *lpszArgv  // array of arguments
								);	
	static void WINAPI ServiceStrl(DWORD dwOpcode);
	
private:
	HANDLE _hStopEvent_Winservice;
	char _servername[50];
	char _serverdisplayname[50];
	char _serviceDescription[100];
	SERVICE_STATUS _status;
	SERVICE_STATUS_HANDLE _hServiceStatus;
	BOOL _bInstall;
	DWORD _dwThreadID;
//唯一一个实例
	static WinService *_service;
};
#endif

