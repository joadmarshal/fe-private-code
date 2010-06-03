// TClientOverlappedSocket.h: interface for the TClientOverlappedSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCLIENTOVERLAPPEDSOCKET_H__2D6FA452_DFE1_4E79_A904_EC76B02EA9A7__INCLUDED_)
#define AFX_TCLIENTOVERLAPPEDSOCKET_H__2D6FA452_DFE1_4E79_A904_EC76B02EA9A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int constTransferTimeOut = -1;
const int constConnectionReset = -2;
const int constWaitEventError  = -3;

class TClientOverlappedSocket  
{
public:
	TClientOverlappedSocket(HANDLE hStopEvt, DWORD dwIPAddr, UINT nPort);
	virtual ~TClientOverlappedSocket();

	// 创建Socket和事件
	int Create();
	// 建立连接， nRetry 为连接失败重试次数，当连接失败时调用OnConnectError
	int Connect(DWORD nRetry = 0);
	int SendData(char* pBuffer, int nLen, DWORD dwTimeOut);
	int ReceiveData(char* pBuffer, int nLen, DWORD dwTimeOut);
protected:
	// OnConnectError Return TRUE : Continue, Return FALSE: Stop Connect 
	virtual BOOL  OnConnectError(int iErr) = 0;
	virtual void  OnSendError(int iErr) = 0;
	virtual void  OnReceiveError(int iErr) = 0;
	// 传输超时时回调，由子类确定是否退出 TRUE 结束传输， FALSE 继续
	virtual BOOL  OnTimeOut(DWORD dwTimeOut)
	{
		return TRUE;
	}
protected:
	// Raw Data
	DWORD m_dwIPAddr;
	UINT  m_nPort;
	HANDLE m_hStopEvt;
	// Socket Data
	SOCKET	 m_sock;
	WSAEVENT m_wsaEvt;

	WSAOVERLAPPED overlapped;
};

#endif // !defined(AFX_TCLIENTOVERLAPPEDSOCKET_H__2D6FA452_DFE1_4E79_A904_EC76B02EA9A7__INCLUDED_)
