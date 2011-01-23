#ifndef __fe_lock__
#define __fe_lock__
#include <windows.h>
template <class LockType = LockCriticalSection>
class TLocker
{
public:
	TLocker(LockType& lock) : lock_(lock)
	{
		lock_.Lock();
	}
	~TLocker()
	{
		lock_.Unlock();
	}
private:
	LockType& lock_;

};

class LockCriticalSection
{
public:
	LockCriticalSection()
	{
		::InitializeCriticalSection(&cs_);
	}
	virtual ~LockCriticalSection()
	{
		::DeleteCriticalSection(&cs_);
	}

	void Lock()
	{
		::EnterCriticalSection(&cs_);
	}

	void Unlock()
	{
		::LeaveCriticalSection(&cs_);
	}

#if _WIN32_WINNT >= 0x0400
	bool TestLock()
	{
		return TryEnterCriticalSection(&cs_);
	}
#endif // _DEBUG

private:
	CRITICAL_SECTION cs_;
};

class LockSemaphore
{
public:
	LockSemaphore(char *name = 0,unsigned int maxnumcoun=1)
	{
		hSemaphore = CreateSemaphore(NULL,maxnumcoun,maxnumcoun,name);
		if(hSemaphore == NULL)
			throw;
	}

	virtual ~LockSemaphore()
	{
		CloseHandle(hSemaphore);
	}

	void Lock()
	{
		WaitForSingleObject(hSemaphore,INFINITE);
	}

	void Unlock()
	{
		ReleaseSemaphore(hSemaphore,1,NULL);
	}

	bool TestLock()
	{
		int ret  =  WaitForSingleObject(hSemaphore,0);
		if(WAIT_TIMEOUT == ret)
			return false;
		else if(WAIT_OBJECT_0 == ret)
			return true;
		throw;
	}
private:
	HANDLE hSemaphore;
};



class LockEvent
{
public:
	LockEvent(const char *name=0)
	{
		hEvent_ = CreateEvent(0,FALSE,TRUE,name);
	}
	virtual ~LockEvent()
	{
		CloseHandle(hEvent_);
	}

	void Lock()
	{
		WaitForSingleObject(hEvent_,INFINITE);
	}

	void Unlock()
	{
		SetEvent(hEvent_);
	}

	bool TestLock()
	{
		int ret  =  WaitForSingleObject(hEvent_,0);
		if(WAIT_TIMEOUT == ret)
			return false;
		else if(WAIT_OBJECT_0 == ret)
			return true;
		throw;
	}

private:
	HANDLE hEvent_;
};

typedef TLocker<LockCriticalSection> CSLocker;
typedef TLocker<LockSemaphore> SemaphoreLocker;
typedef TLocker<LockEvent> EventLocker;
#endif //__fe_lock__