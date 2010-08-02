#include "felock.h"
#include <process.h>
//#include <stdio.h>
#include <iostream>
#include "feRWlock.h"
int i;
// LockCriticalSection g_cslocker;
// LockEvent g_eventLocker;
RwLockEvent<LockCriticalSection> rwlock(2);

struct tests 
{
	int a;
	int b;
	int c;
	int d;
};
tests testval= {1,1,1,1};
void ReadThread(void *p)
{
	int i = (int)p;
	while(1)
	{
		TRLocker<RwLockEvent<LockCriticalSection>> rlocker(rwlock);
		printf("%d Rthread ,%d,%d,%d,%d\n",i,testval.a,testval.b,testval.c,testval.d);
	}
}

void Writehread(void *p)
{
	int i = (int)p;
	while(1)
	{
		TWLocker<RwLockEvent<LockCriticalSection>> wlocker(rwlock);
		printf("%d Wthread\n",i);
		testval.a++;
		testval.b++;
		Sleep(1);
		testval.c++;
		testval.d++;
	}
}


int main()
{
	
	_beginthread( ReadThread, 0, (void *)1 );
	_beginthread( ReadThread, 0, (void *)2 );
	_beginthread( Writehread, 0, (void *)1 );
	_beginthread( Writehread, 0, (void *)2 );
	for(;;)
	{

	}
	return 0;
}