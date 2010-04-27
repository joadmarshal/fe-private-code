#include "winservice.h"
class MyService : public WinService
{
public:
	MyService(char *servicename)
			:WinService(servicename){}
	virtual BOOL OnStop();
	virtual BOOL OnStart() ;
};