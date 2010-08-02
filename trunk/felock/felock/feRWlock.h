#ifndef __fe_RWlock__
#define __fe_RWlock__
#include "felock.h"
template <class RwLockType = RwLockEvent>
 class TRLocker
 {
 public:
	 TRLocker(RwLockType& Rwlock) : Rwlock_(Rwlock)
 	{
		Rwlock_.RLock();
 	}
	~TRLocker()
	{
		Rwlock_.RUnlock();
	}

 private:
	 RwLockType& Rwlock_;
 };

 template <class RwLockType = RwLockEvent>
 class TWLocker
 {
 public:
	 TWLocker(RwLockType& Rwlock) : Rwlock_(Rwlock)
	 {
		 Rwlock_.WLock();
	 }
	 ~TWLocker()
	 {
		 Rwlock_.WUnlock();
	 }

 private:
	 RwLockType& Rwlock_;
 };


template <class LockType = LockCriticalSection>
class RwLockEvent
{
public:
	RwLockEvent(int maxrnum)
	{
		hWEvent_ = CreateEvent(0,TRUE,TRUE,0);
		hREvent_ = CreateEvent(0,TRUE,TRUE,0);
		rcount_ = 0;
		maxrcount_ = maxrnum ;
	}
	virtual ~RwLockEvent()
	{
		CloseHandle(hWEvent_);
		CloseHandle(hREvent_);
	}

	void RLock()
	{
		TLocker<LockType> locker(lock_);
		WaitForSingleObject(hREvent_,INFINITE);
		if(++rcount_ >= maxrcount_)
			ResetEvent(hREvent_);
		if(rcount_ == 1)
			ResetEvent(hWEvent_);
	}

	void WLock()
	{
//		TLocker<LockType> locker(lock_);
		WaitForSingleObject(hWEvent_,INFINITE);
		ResetEvent(hREvent_);
	}

	void RUnlock()
	{
		TLocker<LockType> locker(lock_);
		if(rcount_ == maxrcount_)
			SetEvent(hREvent_);
		if(--rcount_==0)
			SetEvent(hWEvent_);
	}
	void WUnlock()
	{
//		TLocker<LockType> locker(lock_);
		SetEvent(hREvent_);
		SetEvent(hWEvent_);
	}
private:
	HANDLE hWEvent_;
	HANDLE hREvent_;
	LockType lock_;
	int maxrcount_;
	int rcount_;
};

#endif //__fe_RWlock__