#include "felock.h"
#include <process.h>
//#include <stdio.h>
#include <iostream>
#include "feRWlock.h"
int i;
// LockCriticalSection g_cslocker;
// LockEvent g_eventLocker;
RwLockEvent<LockCriticalSection> rwlock;

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
	int count = 0;
	while(1)
	{
		Sleep(1);
		TRLocker<RwLockEvent<LockCriticalSection>> rlocker(rwlock);
		printf("%d Rthread %d count,%d,%d,%d,%d\n",i,count++,testval.a,testval.b,testval.c,testval.d);
		
	}
}

void Writehread(void *p)
{
	int i = (int)p;
	while(1)
	{
		Sleep(1);
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
	_beginthread( ReadThread, 0, (void *)3 );
	_beginthread( ReadThread, 0, (void *)4 );
	_beginthread( ReadThread, 0, (void *)5 );
	_beginthread( ReadThread, 0, (void *)6 );
	_beginthread( ReadThread, 0, (void *)7 );
	_beginthread( Writehread, 0, (void *)1 );
	_beginthread( Writehread, 0, (void *)2 );
	for(;;)
	{

	}
	return 0;
}