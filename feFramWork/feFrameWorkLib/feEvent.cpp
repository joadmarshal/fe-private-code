#include "feEvent.h"
CEvent::CEvent(BOOL bManual)
{
	m_event = CreateEvent(0,bManual,TRUE,0);
}

CEvent::~CEvent()
{
	CloseHandle(m_event);
}