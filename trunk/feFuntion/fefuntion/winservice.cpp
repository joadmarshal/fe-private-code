#include "WinService.h"
#include <stdio.h>
#include <stdarg.h>
BOOL WinService::startservice(int argc, char* argv[])
{
	if ( (argc > 1) &&
		((*argv[1] == '-') || (*argv[1] == '/')) )
	{
		
		if ( _stricmp( "install", argv[1]+1 ) == 0 )
		{
			return Install();
		}
		else if ( _stricmp( "remove", argv[1]+1 ) == 0 )
		{
			return Uninstall();
		}
		else if ( _stricmp( "debug", argv[1]+1 ) == 0 )
		{
			//调试运行
			_service->run();
			return TRUE;
		}
		else
		{
			
		}
		
	}
	//注册服务
	_st[0].lpServiceName=_servername;
	_st[0].lpServiceProc=(LPSERVICE_MAIN_FUNCTION)_ServiceMain;
	
	
	_hServiceStatus = NULL;
	_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	_status.dwCurrentState = SERVICE_STOPPED;
	_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	_status.dwWin32ExitCode = 0;
	_status.dwServiceSpecificExitCode = 0;
	_status.dwCheckPoint = 0;
	_status.dwWaitHint = 0;
	
	WinService::_service=this;
	if (!::StartServiceCtrlDispatcher(_st))//servermian开始
	{
		return FALSE;
	}
	return TRUE;
}

BOOL WinService::stopservice()
{
	if(this->OnStop())
	{
		return SetEvent(_hStopEvent_Winservice);
	}
	return FALSE;
}


void WinService::run()//主线程持续等待
{
	if(!OnStart())
		return;
	_hStopEvent_Winservice = CreateEvent(NULL, TRUE, FALSE, NULL);
	LogEvent("%s start",_servername);
	DWORD dwRet;
	dwRet = WaitForSingleObject(_hStopEvent_Winservice, INFINITE);
	switch( dwRet )
	{
	case WAIT_OBJECT_0:
		CloseHandle(_hStopEvent_Winservice);
		_hStopEvent_Winservice=NULL;
		break;
	default:
		break;
	}
	
}
BOOL WinService::Install()
{
	if (IsInstalled())
		return TRUE;
	
	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		return FALSE;
	}
	
	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	
	//创建服务
	SC_HANDLE hService = ::CreateService(
		hSCM, _servername, _serverdisplayname,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, "", NULL, NULL);
	
	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		return FALSE;
	}
	SERVICE_DESCRIPTION sdBuf;
    sdBuf.lpDescription = _serviceDescription;

	ChangeServiceConfig2(
		hService,                // handle to service
		SERVICE_CONFIG_DESCRIPTION, // change: description
		&sdBuf) ;                  // value: new description
	
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

BOOL WinService::IsInstalled()
{
	BOOL bResult = FALSE;
	
	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	
	if (hSCM != NULL)
	{
		//打开服务
		SC_HANDLE hService = ::OpenService(hSCM, _servername, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

BOOL WinService::Uninstall()
{
	if (!IsInstalled())
		return TRUE;
	
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	
	if (hSCM == NULL)
	{
		return FALSE;
	}
	
	SC_HANDLE hService = ::OpenService(hSCM, _servername, SERVICE_STOP | DELETE);
	
	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);
	
	//删除服务
	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	
	if (bDelete)
		return TRUE;
	
	LogEvent("Service could not be deleted");
	return FALSE;
}

void WINAPI WinService::_ServiceMain(
								DWORD dwArgc,     // number of arguments
								LPTSTR *lpszArgv  // array of arguments
								)
{
	_status.dwCurrentState = SERVICE_START_PENDING;
	_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	
	_hServiceStatus = RegisterServiceCtrlHandler(_servername, _ServiceStrl);
	
	SetServiceStatus(_hServiceStatus, &_status);
	
	_status.dwWin32ExitCode = S_OK;
	_status.dwCheckPoint = 0;
	_status.dwWaitHint = 0;
	_status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(_hServiceStatus, &_status);
	
	_service->run();
	
	_status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(_hServiceStatus, &_status);
}

void WinService::LogEvent(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;
	
	va_start(pArg, pFormat);
	vsprintf(chMsg, pFormat, pArg);
	va_end(pArg);
	
	lpszStrings[0] = chMsg;
	
	hEventSource = RegisterEventSource(NULL, _servername);
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}

void WINAPI WinService::_ServiceStrl(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		if(_service->stopservice())
		{
			_status.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus(_hServiceStatus, &_status);
		}
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		LogEvent("Bad service request");
	}
}

WinService *WinService::_service=NULL;
char WinService::_servername[1024]={"fuckservice"};
char WinService::_serverdisplayname[1024]={"fuckservice"};
char WinService::_serviceDescription[1024]={"fuckservice"};
SERVICE_TABLE_ENTRY WinService::_st[2]={WinService::_servername,WinService::_ServiceMain,NULL,NULL};
SERVICE_STATUS WinService::_status={0,0,0,0,0,0,0};
SERVICE_STATUS_HANDLE WinService::_hServiceStatus=0;
BOOL WinService::_bInstall=FALSE;
DWORD WinService::_dwThreadID=0;