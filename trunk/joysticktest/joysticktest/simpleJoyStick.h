#ifndef simpleJoyStick_h__
#define simpleJoyStick_h__
#include <windows.h>
#define JOYNUM 2
#define BUTTONNUM 4
class SimpleJoyStickMgr;
class JoyStickState;
class IJoyStickHandle
{
public:
	virtual void onButtonDown(JoyStickState* sjs, int buttonNum) = 0;
	virtual void onButtonUp(JoyStickState* sjs, int buttonNum) = 0;
	virtual void onMoveStateChange(JoyStickState* sjs, int movestateX, 
		int movestateY, int preMovestateX, int preMovestateY) = 0;
};

class JoyStickState
{
public:
	JoyStickState();
	bool isButtonDown(int keyNum){return _buttonState[keyNum];};
	int getMoveStateX(){return _moveState[0];};
	int getMoveStateY(){return _moveState[1];};
private:
	bool  _buttonState[BUTTONNUM]; //按钮状态
	int _moveState[2];	//摇杆状态 0是X方向， 1 Y方向
	friend class SimpleJoyStickMgr;
};

class SimpleJoyStickMgr
{
public:

	static SimpleJoyStickMgr *getSimpleJoyStickMgr();;

	bool setJoyStickHandle(int joyNum, IJoyStickHandle *handleObj);
private:
	SimpleJoyStickMgr();
	~SimpleJoyStickMgr();
	bool initSimpleJoyStick();
	void uninitSimpleJoyStick();
	JoyStickState _jsstate[JOYNUM];
	IJoyStickHandle *_handleObjs[JOYNUM];
	void checkDevice();

	static SimpleJoyStickMgr *_simpleJoyStick;
	static LRESULT CALLBACK __wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT __joyMessageHandle(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif // simpleJoyStick_h__