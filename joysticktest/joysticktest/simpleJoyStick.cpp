#include "simpleJoyStick.h"
//todo 必须预先插入。。没处理中途插入的情况。
SimpleJoyStickMgr::SimpleJoyStickMgr()
{

}

LRESULT SimpleJoyStickMgr::__joyMessageHandle( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HDC hdc; 
	PAINTSTRUCT ps;
	switch (message) 
	{ 
	case WM_DESTROY: 
		PostQuitMessage (0); 
		return (0);
	case MM_JOY1MOVE :                     // changed position 
	case MM_JOY2MOVE :
		//printf("MM_JOY1MOVE %u %u %u\n", (UINT) wParam, LOWORD(lParam), HIWORD(lParam));
		{
			int joyNUm = message - MM_JOY1MOVE;
			JoyStickState *jss = &_jsstate[joyNUm];
			int preX = jss->_moveState[0];
			int preY = jss->_moveState[1];
			int origin = LOWORD(DWORD(-1)) / 2;
			jss->_moveState[0] = (int)LOWORD(lParam) - origin;
			jss->_moveState[1] = origin - (int)HIWORD(lParam);
			if(preX != jss->_moveState[0] ||  preY != jss->_moveState[1])
			{
				if(_handleObjs[joyNUm])
					_handleObjs[joyNUm]->onMoveStateChange(jss, jss->_moveState[0], jss->_moveState[1], preX, preY);
			}
		}
		break; 
	case MM_JOY1BUTTONDOWN :               // button is down 
	case MM_JOY2BUTTONDOWN :               // button is down 
		{
			int joyNUm = message - MM_JOY1BUTTONDOWN;
			UINT fwButtons = wParam; 
			JoyStickState *jss = &_jsstate[joyNUm];
			for (int i = 0; i < BUTTONNUM; ++i)
			{
				if(!jss->_buttonState[i] && (UINT) wParam & (JOY_BUTTON1 << i)) 
				{
					jss->_buttonState[i] = true;
					if(jss->_buttonState[i] && _handleObjs[joyNUm])
						_handleObjs[joyNUm]->onButtonDown(jss, i);

				}
			}
		}
		break; 
	case MM_JOY1BUTTONUP :                 // button is up 
	case MM_JOY2BUTTONUP :                 // button is up 
		{
			int joyNUm = message - MM_JOY1BUTTONUP;
			JoyStickState *jss = &_jsstate[joyNUm];
			for (int i = 0; i < BUTTONNUM; ++i)
			{
				if(jss->_buttonState[i] && !((UINT) wParam & (JOY_BUTTON1 << i))) 
				{
					jss->_buttonState[i] = false;
					if(_handleObjs[joyNUm])
						_handleObjs[joyNUm]->onButtonUp(jss, i);
				}
			}
		}
		
		//printf("MM_JOY1BUTTONUP\n");
		//sndPlaySound(NULL, 0);             // stops the sound 
		break; 
	} 
	return DefWindowProc (hwnd, message, wParam, lParam); 
}

LRESULT CALLBACK SimpleJoyStickMgr::__wndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SimpleJoyStickMgr *simpleJoyStick = SimpleJoyStickMgr::getSimpleJoyStickMgr();
	return simpleJoyStick->__joyMessageHandle(hwnd, message, wParam, lParam);
}

bool SimpleJoyStickMgr::initSimpleJoyStick()
{
	WNDCLASS wc; 
	wc.style = 0; 
	wc.lpfnWndProc = SimpleJoyStickMgr::__wndProc; 
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hInstance = GetModuleHandle(NULL); 
	wc.hIcon = NULL; 
	wc.hCursor = NULL; 
	wc.hbrBackground = NULL; 
	wc.lpszMenuName =  ""; 
	wc.lpszClassName = "simplejoystick-memssage-only"; 

	if (!RegisterClass(&wc)) 
		return FALSE; 

	HWND hwnd = CreateWindow(wc.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);

	MMRESULT Result = joySetCapture(hwnd, JOYSTICKID1, 0, FALSE);
	if(Result != MMSYSERR_NOERROR)
		return 1;
}

void SimpleJoyStickMgr::uninitSimpleJoyStick()
{
	delete _simpleJoyStick;
}

bool SimpleJoyStickMgr::setJoyStickHandle( int joyNum, IJoyStickHandle *handleObj )
{
	if(joyNum > JOYNUM)
		return false;
	this->_handleObjs[joyNum] = handleObj;
	return true;

}

SimpleJoyStickMgr * SimpleJoyStickMgr::getSimpleJoyStickMgr()
{
	if(_simpleJoyStick == NULL)
	{
		_simpleJoyStick = new SimpleJoyStickMgr;
		_simpleJoyStick->initSimpleJoyStick();
	}
	return _simpleJoyStick;
}

SimpleJoyStickMgr::~SimpleJoyStickMgr()
{
	_simpleJoyStick->uninitSimpleJoyStick();
	delete _simpleJoyStick;
	_simpleJoyStick = NULL;
}

void SimpleJoyStickMgr::checkDevice()
{
	JOYINFO joyinfo; 
	UINT wNumDevs, wDeviceID; 
	BOOL bDev1Attached, bDev2Attached; 

	if((wNumDevs = joyGetNumDevs()) == 0) 
		return; 
	bDev1Attached = joyGetPos(JOYSTICKID1,&joyinfo) != JOYERR_UNPLUGGED; 
	bDev2Attached = wNumDevs == 2 && joyGetPos(JOYSTICKID2,&joyinfo) != 
		JOYERR_UNPLUGGED; 
	if(bDev1Attached || bDev2Attached)   // decide which joystick to use 
		wDeviceID = bDev1Attached ? JOYSTICKID1 : JOYSTICKID2; 
	else 
		return;
}




SimpleJoyStickMgr * SimpleJoyStickMgr::_simpleJoyStick = NULL;

JoyStickState::JoyStickState()
{
	ZeroMemory(_buttonState, sizeof(_buttonState));
	ZeroMemory(_moveState, sizeof(_moveState));
}
