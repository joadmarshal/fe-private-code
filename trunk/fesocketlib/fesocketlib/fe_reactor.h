//
#include <windows.h>
#include <process.h>
#include <map>
#include <exception>
typedef bool (*Cpio_handle_fun)(LPOVERLAPPED lpOverlapped);

class fe_Cpio_Reactor
{
public:
	fe_Cpio_Reactor()
	{
		//创建一个完成端口
		m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if(NULL == m_hCompletionPort)
			throw std::exception("CreateIoCompletionPort error");
		startrun();
	}

	~fe_Cpio_Reactor()
	{
		stoprun();
		CloseHandle(m_hCompletionPort);
	}
	
	bool regedit_handler(HANDLE sk,Cpio_handle_fun fun)
	{
		CreateIoCompletionPort(sk, m_hCompletionPort, (DWORD)sk, 0);
		m_mHandleFun[sk] = fun;
		return true;
	}
private:
	typedef std::map<HANDLE,Cpio_handle_fun>::iterator iteHandleFun;
	void startrun()
	{
		for( int i = 0; i < 2; ++i )
		{
			HANDLE hThread = (HANDLE)_beginthread(WorkThread, 0, this);
			if( !hThread )
			{
				//CloseHandle(hThread);
				throw;
			}
		}
	}

	void stoprun()
	{
		PostQueuedCompletionStatus(m_hCompletionPort,0,0,0);
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
			}
			else
			{
				if(pulCompletionKey == 0 && lpOverlapped ==0)
					return;
				iteHandleFun handlef = rt->m_mHandleFun.find((HANDLE)pulCompletionKey);
				if(!handlef->second(lpOverlapped))
				{
					
				}

			}
		}
	}

	HANDLE m_hCompletionPort;//完成端口
	std::map<HANDLE,Cpio_handle_fun> m_mHandleFun;
	
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