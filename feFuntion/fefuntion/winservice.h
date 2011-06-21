#ifndef		__FE_WinService__
#define		__FE_WinService__
#include <windows.h>
#include <list>
typedef BOOL (*pOnStart)();
class WinService
{
public:
	static WinService & GetService() 
	{
		if(!_service)
		{
			_service = new WinService(); 
			atexit(ReleaseService);
		}
		return *_service;
	};
	void setDisplayname(const char *szdisplayname){strncpy(_serverdisplayname,szdisplayname,sizeof(_serviceDescription)-1);}
	void setDescription(const char *szdescription){strncpy(_serviceDescription,szdescription,sizeof(_serviceDescription)-1);}
	BOOL startservice(char *servicename);//������������
	BOOL debugservice();//����ʱ�����ڷ�ʽ����
	BOOL stopservice();

	void regeditOnStartFun(pOnStart pfun); 
protected:

	BOOL OnStop() {return TRUE;}
	BOOL OnStart() {return TRUE;}

protected:
	static void ReleaseService();
	WinService(){}

	~WinService(){}

	void LogEvent(LPCTSTR pFormat, ...);
private:

	void run();//���̳߳����ȴ�
	BOOL Install();
	BOOL IsInstalled();
	BOOL Uninstall();

	static void WINAPI ServiceMain(
								DWORD dwArgc,     // number of arguments
								LPTSTR *lpszArgv  // array of arguments
								);	
	static void WINAPI ServiceStrl(DWORD dwOpcode);
	
private:
	std::list<pOnStart> _onstartfunlist;
	HANDLE _hStopEvent_Winservice;
	char _servername[50];
	char _serverdisplayname[50];
	char _serviceDescription[100];
	SERVICE_STATUS _status;
	SERVICE_STATUS_HANDLE _hServiceStatus;
	BOOL _bInstall;
	BOOL _isdebug;
	DWORD _dwThreadID;
//Ψһһ��ʵ��
	static WinService *_service;
};
#endif

