#ifndef _FE_SOCKETLIB__
#define _FE_SOCKETLIB__
#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib")

namespace fesl
{
	WSADATA __wsaData;
	void initsocketlib()
	{
		int retval;
		if ((retval = WSAStartup(0x202,&__wsaData)) != 0)
			throw "";
		atexit(WSACleanup);
	}
}
fesl::initsocketlib();
#endif // _DEBUG


