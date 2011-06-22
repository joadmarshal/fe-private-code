#include "fetcpsession.h"
#include "fecpioreactor.h"


TcpSession::TcpSession()
{
	m_sk=INVALID_SOCKET;
	m_ref=1;
	InitializeCriticalSection(&m_cs);
}
	
void TcpSession::AddRef()
{ 
	EnterCriticalSection(&m_cs);
	++m_ref;
	LeaveCriticalSection(&m_cs);
}

void TcpSession::Release()
{
	int ref;
	EnterCriticalSection(&m_cs);
	ref = --m_ref;
	LeaveCriticalSection(&m_cs);
	if (ref == 0)
		delete this;
}
	
void TcpSession::SynSend(char *buff,size_t buffsize,unsigned int uiTimeout)
{
	m_preactor->SendBuff(m_sk,buff,buffsize,this,uiTimeout);
}
	
void TcpSession::SynRecv(char *buff,size_t recvsize,unsigned int uiTimeout)
{
	m_preactor->RecvBuff(m_sk,buff,recvsize,this,uiTimeout);
}
	
void TcpSession::CloseSocket()
{
	EnterCriticalSection(&m_cs);
	if(m_sk!=INVALID_SOCKET)
		closesocket(m_sk);
	m_sk = INVALID_SOCKET;
	LeaveCriticalSection(&m_cs);
}


TcpSession::~TcpSession()
{
	DeleteCriticalSection(&m_cs);
}
