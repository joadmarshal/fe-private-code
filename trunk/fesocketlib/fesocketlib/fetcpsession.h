#ifndef _FE_FESL__TCPSESSION
#define _FE_FESL__TCPSESSION
#include <winsock2.h>
#include "windows.h"

class Cpio_Reactor;
class TcpSession
{
public:
	TcpSession();
	
	void AddRef();
	
	void Release();
	
	void SynSend(char *buff,size_t sendsize,unsigned int uiTimeout=0);
	
	void SynRecv(char *buff,size_t recvsize,unsigned int uiTimeout=0);
	
	void CloseSocket();

public:
	virtual void  OnAccept(char *pdata){};
	virtual void  OnConnect(int iErr){};
	virtual void  OnSend(char *pdata,size_t sendsize,int iErr){};
	virtual void  OnReceive(char *pdata,size_t recvsize,int iError){};
protected:
	virtual ~TcpSession();
protected:
	SOCKET m_sk;
	CRITICAL_SECTION m_cs;
	Cpio_Reactor *m_preactor;
	int m_ref;
};
#endif