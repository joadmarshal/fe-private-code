#include <windows.h>
#pragma comment(lib, "Winmm.lib")
#include <stdio.h>
#include "simpleJoyStick.h"
class Joyhandle : public IJoyStickHandle
{
public:
	virtual void onButtonDown(JoyStickState* sjs, int buttonNum)
	{
		printf("press button down %u\n", buttonNum);
	}

	virtual void onButtonUp(JoyStickState* sjs, int buttonNum)
	{
		printf("press button up %u\n", buttonNum);
	}

	virtual void onMoveStateChange(JoyStickState* sjs, int movestateX, 
		int movestateY, int preMovestateX, int preMovestateY)
	{
		printf("move x %d\n", movestateX);
		printf("move y %d\n", movestateY);
	}
};
int main()
{
	Joyhandle jh;
	SimpleJoyStickMgr::getSimpleJoyStickMgr()->setJoyStickHandle(0, &jh);
	MSG msg;
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		int i = 0;
		WPARAM wParam = msg.wParam;
		HWND hwnd = msg.hwnd;
	
	}
	return 0;
}