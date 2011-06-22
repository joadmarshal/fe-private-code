#ifndef _FE_FESL__CPIO_REACTOR
#define _FE_FESL__CPIO_REACTOR
#include <windows.h>
#include <process.h>
#include <map>
#include <exception>
#include <winsock2.h>
#include <list>
class TcpSession;
class Cpio_Reactor
{
public:
	Cpio_Reactor(DWORD dwWorkThreadNum=0,DWORD dwWaitMilliseconds=INFINITE);//填0自动根据cpu数量选择线程数//
	~Cpio_Reactor();

	void BindingSocket(SOCKET sk);
	void SendBuff(SOCKET sk,char *buff,size_t buffsize,TcpSession *pHandle,unsigned int TimeOutMilliseconds=0);

	void RecvBuff(SOCKET sk,char *buff,size_t recvSize,TcpSession *pHandle,unsigned int TimeOutMilliseconds=0);

private:
	enum iotype
	{
		iosend=0,
		iorecv,
		ioaccept,
		ioconnect,
		ioclose
	};
	struct IocpReactorOVERLAPPED
	{
		IocpReactorOVERLAPPED()
		{
			ZeroMemory(&overlapped,sizeof(OVERLAPPED));
			buff=NULL;
			buffsize=0;
		}
		~IocpReactorOVERLAPPED()
		{
		}
		OVERLAPPED overlapped;
		TcpSession *pHandle;
		SOCKET sk;
		long ioid;
		iotype type;
		char *buff;
		size_t buffsize;
		WSABUF DataBuf;
	};

	struct TimeOutStruct
	{
		TimeOutStruct(){sk=0;timeOut=0;ioTick=GetTickCount();};
		TimeOutStruct(SOCKET _sk,DWORD _to){sk=_sk;timeOut=_to;ioTick=GetTickCount();}
		SOCKET sk;
		DWORD ioTick;
		DWORD timeOut;
	};

	void startrun(DWORD numberOfThread);

	void stoprun();

	static void WorkThread(void *lParam);
	static void TimerThread(void *lParam);

//	DWORD m_dwWaitMilliseconds;
	HANDLE m_hCompletionPort;//完成端口//
	DWORD m_dwNumberOfThread;
	CRITICAL_SECTION m_cs;

	DWORD m_TimerTimeOut;
	long m_ioid;

	HANDLE m_stopEvent;
	HANDLE m_timeEvent;
	std::map<long,TimeOutStruct> m_MapTimeOutList;
};
#endif
