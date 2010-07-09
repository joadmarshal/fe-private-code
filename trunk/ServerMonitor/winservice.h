#ifndef		__FE_WinService__
#define		__FE_WinService__
#include <windows.h>
class WinService
{
public:
	void setDisplayname(const char *szdisplayname){strncpy(_serverdisplayname,szdisplayname,sizeof(_serviceDescription)-1);}
	void setDescription(const char *szdescription){strncpy(_serviceDescription,szdescription,sizeof(_serviceDescription)-1);}
	BOOL startservice(char *servicename);//正常启动服务
	BOOL debugservice();//调试时，窗口方式启动
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
protected:
	static WinService *GetService() 
	{
		return _service;
	};
protected:
	WinService()
	{
		_isdebug=FALSE;
		if(_service==NULL)
		{
			_service=this;
		}
		else
		{
			//ASSERT(FALSE);
		}
	}
	void LogEvent(LPCTSTR pFormat, ...);
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
	BOOL _isdebug;
	DWORD _dwThreadID;
//唯一一个实例
	static WinService *_service;
};
#endif

