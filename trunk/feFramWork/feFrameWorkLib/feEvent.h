#include "windows.h"
class CEvent
{
public:
	CEvent(BOOL bManual=FALSE);
	~CEvent();
	void wait(1000);
private:
	HANDLE m_event;
};