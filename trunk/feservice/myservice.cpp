#include "myservice.h"
BOOL MyService::OnStop()
{
	return TRUE;
}

BOOL MyService::OnStart()
{
	//创建主线程
// 	gLogger.info("服务启动");
// 	_hstop = CreateEvent(NULL, TRUE, FALSE, NULL);
// 	_hmainListenThread = ::CreateThread(NULL, 0, ServerMonitor::ServerMonitorListenThread, this, 0, NULL);
	return TRUE;
}