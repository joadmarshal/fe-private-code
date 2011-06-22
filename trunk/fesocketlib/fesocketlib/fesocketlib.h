#ifndef _FE_SOCKETLIB__
#define _FE_SOCKETLIB__
#include <winsock2.h>
#include <exception>
#pragma comment(lib,"Ws2_32.lib")
namespace fesl
{
	void cleanupWinsock();
	void initsocketlib();
	void cleanupWinsock();
	SOCKET feWSASocket(int af,int type,int protocol,LPWSAPROTOCOL_INFOA lpProtocolInfo,GROUP g,DWORD dwFlags);

	SOCKET feWSAUdpSocket();
	int febind(SOCKET sk,sockaddr_in *addr);

	int felisten(SOCKET sk,int backlog);


	SOCKET feWSAAccept(SOCKET s,struct sockaddr FAR * addr);

	SOCKET feListenAddr(sockaddr_in *addr);

	SOCKET feWSAAccept(SOCKET s,sockaddr_in * addr);

	void feConnect(SOCKET sk,sockaddr_in *addr);

	SOCKET feConnectAddr(sockaddr_in *addr);

	SOCKET feConnectAddr(int port,const char *ip=0);
}


#endif // _DEBUG


