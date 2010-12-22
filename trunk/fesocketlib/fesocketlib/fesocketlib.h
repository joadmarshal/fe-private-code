#ifndef _FE_SOCKETLIB__
#define _FE_SOCKETLIB__
#include <winsock2.h>
#include <exception>
#pragma comment(lib,"Ws2_32.lib")

namespace fesl
{

	WSADATA __wsaData;
	void cleanupWinsock();
	void initsocketlib()
	{
		int retval;
		if ((retval = WSAStartup(0x202,&__wsaData)) != 0)
			throw std::exception("WSAStartup error");
		atexit(cleanupWinsock);
	}

	void cleanupWinsock()
	{
		WSACleanup();
	}
	
	inline SOCKET feWSASocket(int af,int type,int protocol,LPWSAPROTOCOL_INFOA lpProtocolInfo,GROUP g,DWORD dwFlags)
	{
		SOCKET ret  =  WSASocket(af, type, protocol, lpProtocolInfo, g, dwFlags);
		if(ret == INVALID_SOCKET)
			throw std::exception("WSASocket error");
		return ret;
	}

	inline int febind(SOCKET sk,sockaddr *addr)
	{
		int ret = bind(sk,addr,sizeof(sockaddr_in));
		if(ret == SOCKET_ERROR)
			throw std::exception("bind error");
		return ret;
	}

	inline int felisten(SOCKET sk,int backlog)
	{
		int ret = listen(sk, backlog);
		if(ret == SOCKET_ERROR)
			throw std::exception("listen error");
		return ret;
	}

	inline SOCKET feListenAddr(sockaddr *addr)
	{
		SOCKET sk = feWSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		//SOCKET sk = socket(AF_INET, SOCK_STREAM, 0);
		febind(sk,addr);
		felisten(sk, 20);
		return sk;
	}

	inline SOCKET feWSAAccept(SOCKET s,struct sockaddr FAR * addr)
	{
		int len = sizeof(sockaddr_in);
		SOCKET ret=WSAAccept( s, addr,&len, 0,0 );
		if(ret == INVALID_SOCKET)
			throw std::exception("WSAAccept error");

		return ret;
	}
	
}



#include "fesl_addr.h"
#include "fe_reactor.h"
#endif // _DEBUG


