#include "felock.h"
#include <process.h>
//#include <stdio.h>
#include <iostream>
int i;
LockCriticalSection g_cslocker;
LockEvent g_eventLocker;

class tempchar
{
public:
	tempchar()
	{
		data_=NULL;
	}
	~tempchar()
	{
		if(data_)
			delete data_;
	}
private:
	char *data_;
};
DWORD WINAPI thread1(void *p)
{
	while(1)
	{
		TLocker<LockEvent> lock(g_eventLocker);
		printf("%d\n",i++);
	}
}
// int main()
// {
// 	CreateThread(0,0,thread1,0,0,0);
//  	CreateThread(0,0,thread1,0,0,0);
// 	Sleep(2000);
// };



volatile bool threadStarted = false;

void leaker()
{
	char abc[1024];
	std::cout << atoi( "0" ) << std::endl;
	std::cout << abs(2.0f) << std::endl;
	std::cout << rand() << std::endl;
	sprintf(abc,"%s,%d,%f","abc",6,6.0f);
	printf(abc);
}

DWORD __stdcall CreateThreadFunc( LPVOID )
{
	leaker();
	threadStarted = false;
	return 0;
}

unsigned int __stdcall beginThreadExFunc( LPVOID )
{
	leaker();
	threadStarted = false;
	return 0;
}

void __cdecl beginThreadFunc( LPVOID )
{
	leaker();
	threadStarted = false;
}

int main()
{
	int i=2000;
	for(;;)
	{
		while( threadStarted )
			Sleep( 5 );
		threadStarted = true;
		//_beginthread( beginThreadFunc, 0, 0 );//1
		//CreateThread( NULL, 0, CreateThreadFunc, 0, 0, 0 );//2
		_beginthreadex(0,0,beginThreadExFunc,0,0,0);
		// 		i--;
		// 		if(i<0)
		// 			break;
	}
	system("pause");
	return 0;
}