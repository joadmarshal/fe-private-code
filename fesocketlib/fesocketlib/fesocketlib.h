#ifndef _FE_SOCKETLIB__
#define _FE_SOCKETLIB__
#include <winsock2.h>
#include <exception>
#pragma comment(lib,"Ws2_32.lib")
#include "fesl_addr.h"
#include "fe_reactor.h"

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

	inline int febind(SOCKET sk,sockaddr_in *addr)
	{
		int ret = bind(sk,(sockaddr *)addr,sizeof(sockaddr_in));
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

	inline SOCKET feListenAddr(sockaddr_in *addr)
	{
		SOCKET sk = feWSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		febind(sk,addr);
		felisten(sk, 20);
		return sk;
	}

	inline SOCKET feWSAAccept(SOCKET s,sockaddr_in * addr)
	{
		int len = sizeof(sockaddr_in);
		SOCKET ret=WSAAccept( s, (sockaddr *)addr,&len, 0,0 );
		if(ret == INVALID_SOCKET)
			throw std::exception("WSAAccept error");

		return ret;
	}

	inline void feConnect(SOCKET sk,sockaddr_in *addr)
	{
		if (connect(sk,(struct sockaddr*)addr,sizeof(sockaddr_in))== SOCKET_ERROR) 
			throw std::exception("WSAAccept connect error");
	}

	inline SOCKET feConnectAddr(sockaddr_in *addr)
	{
		SOCKET sk = feWSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		feConnect(sk,addr);
		return sk;
	}

	inline SOCKET feConnectAddr(int port,const char *ip=0)
	{
		fesl::Inet_addr addr(port,ip);
		return feConnectAddr((sockaddr_in *)&addr);
	}
}




#endif // _DEBUG


