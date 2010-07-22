#include "fesocketlib.h"


fe_Cpio_Reactor *cpioRt;
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
	delete cpioRt;
	return true;
}

int main()
{
	cpioRt=new fe_Cpio_Reactor();
	fesl::initsocketlib();
	fesl::Inet_addr addr(0,1656);

	SOCKET m_sockListen = fesl::feListenAddr((sockaddr *)&addr);
	
	fesl::Inet_addr rmaddr(0,0);

	while(TRUE)
	{
		SOCKET saccept = fesl::feWSAAccept( m_sockListen, (sockaddr*)&rmaddr);
		const char chOpt = 1;

		

		cpioRt->regedit_handler((HANDLE)saccept,handle_recv);

		DWORD Flags=0;
		DWORD RecvBytes;

		MyOVERLAPPED *myol = new MyOVERLAPPED;
		if (WSARecv(saccept,&(myol->DataBuf),1, &RecvBytes, &Flags,&(myol->overlapped), NULL)== SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSARecv() failed with error %d\n", WSAGetLastError());
				return 0;
			}
		}
	}
	

	
	
};

