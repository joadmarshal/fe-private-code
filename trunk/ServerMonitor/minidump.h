#ifndef ___MINIDUMP___
#define ___MINIDUMP___



#include <Dbghelp.h>
#include <time.h>

typedef BOOL (*MiniDumpWriteDump_PTR)(
									  HANDLE hProcess,
									  DWORD ProcessId,
									  HANDLE hFile,
									  MINIDUMP_TYPE DumpType,
									  PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
									  PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
									  PMINIDUMP_CALLBACK_INFORMATION CallbackParam
									  );


void CreateMiniDump(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	HMODULE hModule = ::LoadLibrary(".\\dbghelp.dll");
	if( NULL == hModule )
		return;
	MiniDumpWriteDump_PTR pfnMiniDumpWriteDump = (MiniDumpWriteDump_PTR)GetProcAddress(hModule, "MiniDumpWriteDump");
	if( NULL == pfnMiniDumpWriteDump )
	{
		FreeLibrary(hModule);
		return;
	}
	char szFile[MAX_PATH+1] = {0};
	_snprintf(szFile, MAX_PATH, ".\\%u.dmp", time(NULL));
	HANDLE hFile = CreateFile(szFile, GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if( INVALID_HANDLE_VALUE == hFile )
	{
		FreeLibrary(hModule);
		return;
	}
	MINIDUMP_EXCEPTION_INFORMATION mei;
	mei.ThreadId = GetCurrentThreadId();
	mei.ClientPointers = TRUE;
	mei.ExceptionPointers = ExceptionInfo;
	
	(*pfnMiniDumpWriteDump)(GetCurrentProcess(), GetCurrentProcessId(), hFile,
		MiniDumpWithFullMemory,
		&mei, NULL, NULL);
	CloseHandle(hFile);
	FreeLibrary(hModule);
}

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	CreateMiniDump(ExceptionInfo);
	::SetErrorMode(SEM_NOGPFAULTERRORBOX);
	gLogger.fatal("ServerMonitor Meet UnhandledException");
	::ExitProcess(1);
	return EXCEPTION_EXECUTE_HANDLER;
}

void SetAutoMinidump()
{
	::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
}

#endif