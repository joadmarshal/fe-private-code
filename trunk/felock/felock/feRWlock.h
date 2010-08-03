#ifndef __fe_RWlock__
#define __fe_RWlock__
#include "felock.h"
#include <queue>
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
	RwLockEvent()
	{
		hWEvent_ = CreateEvent(0,TRUE,TRUE,0);
		rcount_ = 0;
	}
	virtual ~RwLockEvent()
	{
		CloseHandle(hWEvent_);
	}

	void RLock()
	{
		do
		{
			bool isCanRead=true;
			{
				TLocker<LockType> locker(lock_);
				if(rcount_ == -1)
				{
					isCanRead = false;
				}
				else
				{
					if(rcount_ == 0)
						ResetEvent(hWEvent_);
					++rcount_;
				}
			}
			if(isCanRead)
				break;
			else
			{
				WaitForSingleObject(hWEvent_,INFINITE);
			}
		}
		while(true);
	}

	void WLock()
	{
		do
		{
			bool isCanWrite=true;
			{
				TLocker<LockType> locker(lock_);
				if(rcount_ != 0)
				{
					isCanWrite = false;
				}
				else
				{
					ResetEvent(hWEvent_);
					--rcount_;
				}
			}
			if(isCanWrite)
				break;
			else
			{
				WaitForSingleObject(hWEvent_,INFINITE);
			}
		}
		while(true);
	}

	void RUnlock()
	{
		TLocker<LockType> locker(lock_);
		if(--rcount_==0)
			SetEvent(hWEvent_);
	}
	void WUnlock()
	{
		TLocker<LockType> locker(lock_);
		SetEvent(hWEvent_);
		++rcount_;
	}
private:
	HANDLE hWEvent_;
	HANDLE hREvent_;
	LockType lock_;
	int rcount_;
};


//template <class LockType = LockCriticalSection>
//class RwLockEvent
//{
//public:
// RwLockEvent(int maxrnum)
// {
//	 hWEvent_ = CreateEvent(0,TRUE,TRUE,0);
//	 hREvent_ = CreateEvent(0,TRUE,TRUE,0);
//	 rcount_ = 0;
//	 maxrcount_ = maxrnum ;
// }
// virtual ~RwLockEvent()
// {
//	 CloseHandle(hWEvent_);
//	 CloseHandle(hREvent_);
// }
//
// void RLock()
// {
//	 lock(read);
// }
//
// void WLock()
// {
//	 lock(write);
// }
//
// void RUnlock()
// {
//	 unlock(read);
// }
// void WUnlock()
// {
//	 unlock(write);
// }
//private:
// enum iotype
// {
//	read = 0,
//	write
// };
// HANDLE hWEvent_;
// HANDLE hREvent_;
// LockType accestlock_;
// LockType statelock_;
// int maxrcount_;
// int rcount_;
// bool lock(iotype type)
// {
//	 TLocker<LockType> locker(accestlock_);
//	 if(type == read)
//	 {
//		 if(rcount_==-1)
//		 {
//			WaitForSingleObject(hWEvent_,INFINITE);
//		 }
//		 else if(rcount_ == maxrcount_)
//		 {
//			WaitForSingleObject(hREvent_,INFINITE);
//		 }
//		 {
//			 TLocker<LockType> statelocker(statelock_);
//			 rcount_++;
//			 if(rcount_>0)
//				 ResetEvent(hWEvent_);
//			 if(rcount_ == maxrcount_)
//				 ResetEvent(hREvent_);
//		 }
//	 }
//	 else
//	 {
//		if(rcount_ > 0 || rcount_< 0)
//		{
//			WaitForSingleObject(hWEvent_,INFINITE);
//		}
//
//		{
//			TLocker<LockType> statelocker(statelock_);
//			rcount_--;
//			ResetEvent(hWEvent_);
//			ResetEvent(hREvent_);
//		}
//	 }
//	return true;
// }
//
// bool unlock(iotype type)
// {
//	if (type == read)
//	{
//		if (rcount_ == maxrcount_)
//		{
//			SetEvent(hREvent_);
//		}
//		else if(rcount_ == 1)
//		{
//			SetEvent(hWEvent_);
//		}
//		rcount_--;
//	}
//	else
//	{
//		rcount_++;
//		SetEvent(hREvent_);
//		SetEvent(hWEvent_);
//	}
//
//	return true;
// }
//};

#endif //__fe_RWlock__