// TClientOverlappedSocket.cpp: implementation of the TClientOverlappedSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TClientOverlappedSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TClientOverlappedSocket::TClientOverlappedSocket(HANDLE hStopEvt, DWORD dwIPAddr, UINT nPort) : m_hStopEvt(hStopEvt), m_dwIPAddr(dwIPAddr), m_nPort(nPort)
{
	m_wsaEvt = WSA_INVALID_EVENT;
	m_sock = INVALID_SOCKET;
}

TClientOverlappedSocket::~TClientOverlappedSocket()
{
	if( m_sock != INVALID_SOCKET )
	{
		closesocket(m_sock);
	}
	if( m_wsaEvt != WSA_INVALID_EVENT )
	{
		WSACloseEvent(m_wsaEvt);
	}
	
}

int TClientOverlappedSocket::Create()
{
	m_sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED); 
	if( INVALID_SOCKET == m_sock )
	{
		return WSAGetLastError();
	}
	m_wsaEvt = WSACreateEvent();
	if( WSA_INVALID_EVENT == m_wsaEvt )
	{
		return WSAGetLastError();
	}
	return 0;
}

int TClientOverlappedSocket::Connect(DWORD nRetry)
{
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port = htons(m_nPort);
	server.sin_addr.S_un.S_addr=htonl(m_dwIPAddr);
	DWORD nCount = 0;
	BOOL bSuccess = FALSE;
	DWORD dwErr;
	while( nCount <= nRetry )
	{
		if( SOCKET_ERROR == connect(m_sock, (sockaddr*)&server, sizeof(sockaddr_in)) )
		{
			dwErr = WSAGetLastError();
			if( FALSE == OnConnectError(dwErr) )
			{
				break;
			}
			nCount++;
		}
		else
		{
			bSuccess = TRUE;
			break;
		}
	}
	if( bSuccess )
		return 0;
	else
		return dwErr;
}

int TClientOverlappedSocket::SendData(char* pBuffer, int nLen, DWORD dwTimeOut)
{
	HANDLE lpHandles[2] = {m_hStopEvt, m_wsaEvt};
	WSABUF wsabuf;
	
	WSAResetEvent(m_wsaEvt);
	memset(&overlapped, 0, sizeof(WSAOVERLAPPED));
	overlapped.hEvent = m_wsaEvt;

	int nLeft = nLen;
	int nTotalSend = 0;
	DWORD nSend = 0;
	int iRet;
	while( nLeft > 0 )
	{
		wsabuf.buf = pBuffer+nTotalSend;
		wsabuf.len = nLeft;
		
		iRet = WSASend(m_sock, &wsabuf, 1, &nSend, 0, &overlapped, NULL);
		if( iRet == SOCKET_ERROR )
		{
			iRet = WSAGetLastError();
			if( iRet != WSA_IO_PENDING )
			{
				OnSendError(iRet);
				break;
			}
		}
		
		DWORD dwRet = WaitForMultipleObjects(2, lpHandles, FALSE, dwTimeOut);
		if( dwRet == WAIT_OBJECT_0 )
		{
			return -1;
		}
		else if( dwRet == WAIT_OBJECT_0+1 )
		{
			DWORD dwFlags;
			if( FALSE == WSAGetOverlappedResult(m_sock, &overlapped, &nSend, FALSE, &dwFlags) )
			{
				OnSendError(WSAGetLastError());
				break;
			}
			if( 0 == nSend )
			{
				// Peer Reset
				OnSendError(constConnectionReset);
				break;
			}
			nTotalSend += nSend;
			nLeft -= nSend;
			WSAResetEvent(m_wsaEvt);
		}
		else if( dwRet == WAIT_TIMEOUT )
		{
			if( OnTimeOut(dwTimeOut) )
				OnSendError(constTransferTimeOut);
			break;
		}
		else
		{
			OnSendError(constWaitEventError);
			break;
		}

	}
	return nTotalSend;
}

int TClientOverlappedSocket::ReceiveData(char *pBuffer, int nLen, DWORD dwTimeOut)
{
	HANDLE lpHandles[2] = {m_hStopEvt, m_wsaEvt};
	WSABUF wsabuf;
	

	WSAResetEvent(m_wsaEvt);
	memset(&overlapped, 0, sizeof(WSAOVERLAPPED));
	overlapped.hEvent = m_wsaEvt;

	int nLeft = nLen;
	int nTotalRecv = 0;
	DWORD nRecv = 0;
	int iRet;
	while( nLeft > 0 )
	{
		wsabuf.buf = pBuffer+nTotalRecv;
		wsabuf.len = nLeft;
		DWORD dwFlags = 0;
		iRet = WSARecv(m_sock, &wsabuf, 1, &nRecv, &dwFlags, &overlapped, NULL);
		if( iRet == SOCKET_ERROR )
		{
			iRet = WSAGetLastError();
			if( iRet != WSA_IO_PENDING )
			{
				OnReceiveError(iRet);
				break;
			}
		}
		
		DWORD dwRet = WaitForMultipleObjects(2, lpHandles, FALSE, dwTimeOut);
		if( dwRet == WAIT_OBJECT_0 )
		{
			return -1;
		}
		else if( dwRet == WAIT_OBJECT_0+1 )
		{
			DWORD dwFlags;
			if( FALSE == WSAGetOverlappedResult(m_sock, &overlapped, &nRecv, FALSE, &dwFlags) )
			{
				OnReceiveError(WSAGetLastError());
				break;
			}
			if( 0 == nRecv )
			{
				// Peer Reset
				OnReceiveError(constConnectionReset);
				break;
			}
			nTotalRecv += nRecv;
			nLeft -= nRecv;
			WSAResetEvent(m_wsaEvt);
		}
		else if( dwRet == WAIT_TIMEOUT )
		{
			if( OnTimeOut(dwTimeOut) )
				OnReceiveError(constTransferTimeOut);
			break;
		}
		else
		{
			OnReceiveError(constWaitEventError);
			break;
		}

	}
	return nTotalRecv;
}