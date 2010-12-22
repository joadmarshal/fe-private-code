//
#include <windows.h>
#include <process.h>
#include <map>
#include <exception>
typedef bool (*Cpio_handle_fun)(LPOVERLAPPED lpOverlapped);

template<typename T>
class fe_Cpio_Reactor
{
public:
	fe_Cpio_Reactor(DWORD dwWorkThreadNum=0)//填0自动根据cpu数量选择线程数//
	{
		//创建一个完成端口//
		m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if(NULL == m_hCompletionPort)
			throw std::exception("fe_Cpio_Reactor CreateIoCompletionPort error");
		startrun(dwWorkThreadNum);
	}

	~fe_Cpio_Reactor()
	{
		::DeleteCriticalSection(&__cs);
		stoprun();
		CloseHandle(m_hCompletionPort);
	}

	bool regedit_handler(HANDLE sk,T &obj)
	{
		CreateIoCompletionPort(sk, m_hCompletionPort, (DWORD)sk, 0);
		::EnterCriticalSection(&__cs);
		m_mHandleObj[sk] = obj;
		::LeaveCriticalSection(&__cs);
		return true;
	}

	bool unregedit_handler(HANDLE sk)//调用后，必须外部调用closesocket,并停止，关闭//
	{
		CancelIo(sk);
		::EnterCriticalSection(&__cs);
		m_mHandleObj.erase(sk);
		::LeaveCriticalSection(&__cs);
		return true;
	}
private:
	void startrun(DWORD numberOfThread)
	{
		if(numberOfThread==0)
		{
			SYSTEM_INFO systeminfo;
			GetSystemInfo(&systeminfo);
			m_dwNumberOfThread=systeminfo.dwNumberOfProcessors;
		}
		else
			m_dwNumberOfThread = numberOfThread;
		if(m_dwNumberOfThread == 0)
			throw std::exception("fe_Cpio_Reactor 0 NumberOfThread");
		for( int i = 0; i < m_dwNumberOfThread; ++i )
		{
			HANDLE hThread = (HANDLE)_beginthread(WorkThread, 0, this);
			if( !hThread )
				throw std::exception("fe_Cpio_Reactor _beginthread error");
		}
	}

	void stoprun()
	{
		for (int i=0;i<m_dwNumberOfThread;++i)
			PostQueuedCompletionStatus(m_hCompletionPort,0,0,0);
		CloseHandle(m_hCompletionPort);
	}

	static void WorkThread(void *lParam)
	{
		fe_Cpio_Reactor *rt = (fe_Cpio_Reactor *)lParam;
		DWORD dwBytesTransferred;
		LPOVERLAPPED lpOverlapped;
		BOOL bRet;
		ULONG_PTR pulCompletionKey;

		while (TRUE)
		{
			bRet = GetQueuedCompletionStatus( rt->m_hCompletionPort, &dwBytesTransferred, &pulCompletionKey, (LPOVERLAPPED*)&lpOverlapped, INFINITE );
			if( FALSE == bRet )
			{
				DWORD dwErr = GetLastError();
				if( WAIT_TIMEOUT == dwErr )
					continue;
				else
				{

					return;
				}
			}
			else
			{
				if(pulCompletionKey == 0 && lpOverlapped ==0)//退出标志//
					return;
				::EnterCriticalSection(&__cs);
				iteHandleObj handleObj = rt->m_mHandleObj.find((HANDLE)pulCompletionKey);
				if(handleObj != m_mHandleObj.end())
				{
					handleObj->HandleIocp();
				}
				::LeaveCriticalSection(&__cs);
			}
		}
	}

	HANDLE m_hCompletionPort;//完成端口//
	DWORD m_dwNumberOfThread;
	std::map<HANDLE,T> m_mHandleObj;
	typedef typename std::map<HANDLE,T>::iterator iteHandleObj;
	CRITICAL_SECTION __cs;
};



typedef bool (*EventSelect_handle_fun)();

class fe_EventSelect_Reactor
{
public:
	fe_EventSelect_Reactor()
	{
	}

	~fe_EventSelect_Reactor()
	{
		stoprun();
	}
	bool regedit_handler(HANDLE sk,EventSelect_handle_fun fun	)
	{
		
	}
private:

	void startrun()
	{
	}

	void stoprun()
	{
	}

};