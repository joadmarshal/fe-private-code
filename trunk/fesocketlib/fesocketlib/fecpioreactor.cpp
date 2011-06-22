#include "fetcpsession.h"
#include "fecpioreactor.h"
Cpio_Reactor::Cpio_Reactor(DWORD dwWorkThreadNum,DWORD dwWaitMilliseconds)//填0自动根据cpu数量选择线程数//
{
	//创建一个完成端口//
	m_TimerTimeOut=INFINITE;
	m_timeEvent = CreateEvent(0,FALSE,FALSE,0);
	m_ioid=0;
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	InitializeCriticalSection(&m_cs);
	if(NULL == m_hCompletionPort)
		throw std::exception("Cpio_Reactor CreateIoCompletionPort error");
	startrun(dwWorkThreadNum);
}

Cpio_Reactor::~Cpio_Reactor()
{
	::DeleteCriticalSection(&m_cs);
	stoprun();
	CloseHandle(m_timeEvent);
	CloseHandle(m_hCompletionPort);
}

void Cpio_Reactor::BindingSocket(SOCKET sk)
{
	CreateIoCompletionPort((HANDLE)sk, m_hCompletionPort, sk, 0);
}

void Cpio_Reactor::SendBuff(SOCKET sk,char *buff,size_t buffsize,TcpSession *pHandle,unsigned int TimeOutMilliseconds)
{
	IocpReactorOVERLAPPED *piol=new IocpReactorOVERLAPPED;
	piol->sk=sk;
	piol->pHandle=pHandle;
	pHandle->AddRef();
	piol->type=iosend;

	piol->buff=buff;
	piol->buffsize=buffsize;

	piol->DataBuf.buf=buff;
	piol->DataBuf.len=buffsize;

	piol->ioid = InterlockedIncrement(&m_ioid);
	
	DWORD dwsize=0;
	int iRet = WSASend(sk,&piol->DataBuf,1,&dwsize,0,(LPOVERLAPPED)piol,0);
	if(iRet==SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
	{
		pHandle->OnSend(buff,buffsize,WSAGetLastError());
		pHandle->Release();
		delete piol;
	}
	else
	{
		if(TimeOutMilliseconds)
		{
			EnterCriticalSection(&m_cs);
			m_MapTimeOutList[piol->ioid]=TimeOutStruct(piol->sk,TimeOutMilliseconds);
			LeaveCriticalSection(&m_cs);
			SetEvent(m_timeEvent);
		}
	}
	
}

void Cpio_Reactor::RecvBuff(SOCKET sk,char *buff,size_t recvSize,TcpSession *pHandle,unsigned int TimeOutMilliseconds)
{
	IocpReactorOVERLAPPED *piol=new IocpReactorOVERLAPPED;
	piol->sk=sk;
	piol->pHandle=pHandle;
	pHandle->AddRef();
	piol->type=iorecv;
	
	piol->buff=buff;
	piol->buffsize=recvSize;

	piol->DataBuf.buf=(char *)buff;
	piol->DataBuf.len=recvSize;

	piol->ioid = InterlockedIncrement(&m_ioid);

	DWORD dwsize=0;
	DWORD flag=0;
	int iRet = WSARecv(sk,&piol->DataBuf,1,&dwsize,&flag,(LPOVERLAPPED)piol,0);
	if(iRet==SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
	{
		pHandle->OnReceive(buff,0,WSAGetLastError());
		pHandle->Release();
		delete piol;
	}
	else
	{
		if(TimeOutMilliseconds)
		{
			EnterCriticalSection(&m_cs);
			m_MapTimeOutList[piol->ioid]=TimeOutStruct(piol->sk,TimeOutMilliseconds);
			LeaveCriticalSection(&m_cs);
			SetEvent(m_timeEvent);
		}
	}
	
}

void Cpio_Reactor::startrun(DWORD numberOfThread)
{
	m_stopEvent=CreateEvent(0,TRUE,FALSE,0);
	if(numberOfThread==0)
	{
		SYSTEM_INFO systeminfo;
		GetSystemInfo(&systeminfo);
		m_dwNumberOfThread=systeminfo.dwNumberOfProcessors;
	}
	else
		m_dwNumberOfThread = numberOfThread;
	if(m_dwNumberOfThread == 0)
		throw std::exception("Cpio_Reactor 0 NumberOfThread");
	for( int i = 0; i < m_dwNumberOfThread; ++i )
	{
		HANDLE hThread = (HANDLE)_beginthread(WorkThread, 0, this);
		if( !hThread )
			throw std::exception("Cpio_Reactor _beginthread error");
	}
	HANDLE hThread = (HANDLE)_beginthread(TimerThread, 0, this);
}

void Cpio_Reactor::stoprun()
{
	SetEvent(m_stopEvent);
	for (int i=0;i<m_dwNumberOfThread;++i)
		PostQueuedCompletionStatus(m_hCompletionPort,0,0,0);
	CloseHandle(m_hCompletionPort);
}
void Cpio_Reactor::TimerThread(void *lParam)
{
	Cpio_Reactor *rt = (Cpio_Reactor *)lParam;
	HANDLE hevent[2]={rt->m_stopEvent,rt->m_timeEvent};
	while(true)
	{
		DWORD dwret=WaitForMultipleObjects(2,hevent,FALSE,rt->m_TimerTimeOut);
		if(dwret != WAIT_TIMEOUT && dwret != WAIT_OBJECT_0 + 1)
			break;
		EnterCriticalSection(&rt->m_cs);
		DWORD s = rt->m_MapTimeOutList.size();
		if(rt->m_MapTimeOutList.size()>0)
		{
			std::map<long,TimeOutStruct>::iterator ite = rt->m_MapTimeOutList.begin();
			for (;ite != rt->m_MapTimeOutList.end();)
			{
				DWORD ntick = GetTickCount();
				DWORD iotick = ite->second.ioTick;
				DWORD td = ntick - ite->second.ioTick;
				if( td >= ite->second.timeOut)
				{
					DWORD ioid = ite->first;
					closesocket(ite->second.sk);//结束IO;100
					std::map<long,TimeOutStruct>::iterator eraseite=ite;
					++ite;
					rt->m_MapTimeOutList.erase(eraseite);
				}
				else
				{
					DWORD nwt=ite->second.timeOut - td;
					rt->m_TimerTimeOut = nwt > rt->m_TimerTimeOut ? rt->m_TimerTimeOut : nwt;
					++ite;
				}
			}
		}
		else
		{
			rt->m_TimerTimeOut = INFINITE;
		}
		LeaveCriticalSection(&rt->m_cs);
	}
}

void Cpio_Reactor::WorkThread(void *lParam)
{
	Cpio_Reactor *rt = (Cpio_Reactor *)lParam;
	DWORD dwBytesTransferred;

	IocpReactorOVERLAPPED *lpOverlapped;
	BOOL bRet;
	ULONG_PTR pulCompletionKey;

	while (TRUE)
	{
		bRet = GetQueuedCompletionStatus( rt->m_hCompletionPort, &dwBytesTransferred, &pulCompletionKey, (LPOVERLAPPED*)&lpOverlapped, INFINITE);
		if(pulCompletionKey == 0 && lpOverlapped ==0)//退出标志//
			return;
		if(lpOverlapped==NULL)//检查超时
			continue;
#define RELEASE_OVLD(lpOverlapped) EnterCriticalSection(&rt->m_cs);\
									rt->m_MapTimeOutList.erase(lpOverlapped->ioid);\
									LeaveCriticalSection(&rt->m_cs);\
									lpOverlapped->pHandle->Release();\
									delete lpOverlapped;

// 		if(dwBytesTransferred==0 && WSAGetLastError()==ERROR_IO_PENDING)
// 			continue;
		switch (lpOverlapped->type)
		{
		case iosend:
			if(dwBytesTransferred==0)
			{
				bRet=FALSE;
				WSASetLastError(64);//网络不可用;
			}
			if(bRet)
				lpOverlapped->DataBuf.len-=dwBytesTransferred;
			if(bRet == FALSE || lpOverlapped->DataBuf.len==0)
			{
				lpOverlapped->pHandle->OnSend(lpOverlapped->DataBuf.buf,lpOverlapped->buffsize,bRet?0:WSAGetLastError());
				RELEASE_OVLD(lpOverlapped);
				
			}
			else
			{
				lpOverlapped->DataBuf.buf=
					lpOverlapped->buff + lpOverlapped->buffsize - lpOverlapped->DataBuf.len;
				ZeroMemory(lpOverlapped,sizeof(OVERLAPPED));
				DWORD dwsize=0;
				int iRet =  WSASend(lpOverlapped->sk,&lpOverlapped->DataBuf,1,&dwsize,0,(LPOVERLAPPED)lpOverlapped,0);
				if(iRet==SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
				{
					lpOverlapped->pHandle->OnSend(lpOverlapped->buff,0,WSAGetLastError());
					RELEASE_OVLD(lpOverlapped);
				}
			}
			break;
		case iorecv:
			if(dwBytesTransferred==0)
			{
				bRet=FALSE;
				WSASetLastError(64);//网络不可用;
			}
			if(bRet)
				lpOverlapped->DataBuf.len-=dwBytesTransferred;
			if(bRet == FALSE || lpOverlapped->DataBuf.len==0)
			{
				lpOverlapped->pHandle->OnReceive(lpOverlapped->DataBuf.buf,lpOverlapped->buffsize,bRet?0:WSAGetLastError());
				RELEASE_OVLD(lpOverlapped);
			}
			else
			{
				lpOverlapped->DataBuf.buf=
					lpOverlapped->buff + lpOverlapped->buffsize - lpOverlapped->DataBuf.len;
				ZeroMemory(lpOverlapped,sizeof(OVERLAPPED));
				DWORD dwsize=0;
				DWORD flag=0;
				int iRet = WSARecv(lpOverlapped->sk,&lpOverlapped->DataBuf,1,&dwsize,&flag,(LPOVERLAPPED)lpOverlapped,0);
				if(iRet==SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
				{
					lpOverlapped->pHandle->OnReceive(lpOverlapped->buff,0,WSAGetLastError());
					RELEASE_OVLD(lpOverlapped);
				}
			}
			break;
		case ioaccept:
			//lpOverlapped->pHandle->OnAccept(lpOverlapped->DataBuf.buf,dwBytesTransferred,bRet?bRet:WSAGetLastError());
			RELEASE_OVLD(lpOverlapped);
			break;
		case ioconnect:
			RELEASE_OVLD(lpOverlapped);
			break;
		}
		
		
		
	}
}

