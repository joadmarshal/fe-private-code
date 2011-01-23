#include "feFrameWork.h";
#include <process.h>
namespace feFrameWork
{
	CFrameWorkApp *CFrameWorkApp::_pApp=0;
	CFrameWorkApp::CFrameWorkApp()
	{
		uintptr_t hThread = _beginthread(framework_main_thread,0,0);
	};
	bool CFrameWorkApp::RegeditComponent(IFComponent *)
	{
		return true;
	}
	bool CFrameWorkApp::UnRegeditComponent(const char *componentName)
	{
		return true;
	}

	void CFrameWorkApp::framework_main_thread(void *param) //主循环线程.
	{

	}
}