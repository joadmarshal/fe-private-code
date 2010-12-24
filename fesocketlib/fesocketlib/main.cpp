#include "fesocketlib.h"

#define SUPERIORREPORT_HEAD "\xE0\xE0\xE0\xE0"  //级联上报使用的通讯包头
#define PACK_HEAD_LEN	sizeof(SUPERIORREPORT_HEAD)+4
#define GETDATALEN(packbuff) ((DWORD)(packbuff+sizeof(SUPERIORREPORT_HEAD)))
#define MAX_RP_BUFF_SIZE 4096

//fe_Cpio_Reactor *cpioRt;
struct MyOVERLAPPED
{
	MyOVERLAPPED()
	{
		ZeroMemory(&overlapped,sizeof(OVERLAPPED));
		DataBuf.buf=recvBuff;
		DataBuf.len=1024;
	}
	OVERLAPPED overlapped;
	int iotype;
	WSABUF DataBuf;
	char recvBuff[1024];
};
bool handle_recv(LPOVERLAPPED lpOverlapped)
{
	MyOVERLAPPED *myovlp=(MyOVERLAPPED *)lpOverlapped;
//	delete cpioRt;
	return true;
}

int main()
{
	//cpioRt=new fe_Cpio_Reactor();
	fesl::initsocketlib();
	fesl::Inet_addr addr(7892);

	SOCKET m_sockListen = fesl::feListenAddr((sockaddr *)&addr);
	
	fesl::Inet_addr rmaddr(0,0);

	while(TRUE)
	{
		SOCKET saccept = fesl::feWSAAccept( m_sockListen, (sockaddr*)&rmaddr);
		const char chOpt = 1;

 		DWORD Flags=0;
 		DWORD RecvBytes;
 
 		MyOVERLAPPED *myol = new MyOVERLAPPED;
 		if (WSARecv(saccept,&(myol->DataBuf),1, &RecvBytes, &Flags,0, NULL)== SOCKET_ERROR)
 		{
 			if (WSAGetLastError() != ERROR_IO_PENDING)
 			{
 				printf("WSARecv() failed with error %d\n", WSAGetLastError());
 				return 0;
 			}
 		}
		
		char *buff=myol->DataBuf.buf + PACK_HEAD_LEN;
		printf(buff);
		if(send(saccept,myol->DataBuf.buf,RecvBytes,0)==SOCKET_ERROR)
		{
			
		}
		int i=0;
// 		if (WSARecv(saccept,&(myol->DataBuf),1, &RecvBytes, &Flags,0, NULL)== SOCKET_ERROR)
// 		{
// 			if (WSAGetLastError() != ERROR_IO_PENDING)
// 			{
// 				printf("WSARecv() failed with error %d\n", WSAGetLastError());
// 				return 0;
// 			}
// 		}

	}
	

	
	fesl::cleanupWinsock();
};

