#include "fesl.h"
#include "fe_buff.h"
#include <stdio.h>
#define SUPERIORREPORT_HEAD "\xE0\xE0\xE0\xE0"  //级联上报使用的通讯包头
#define PACK_HEAD_LEN	sizeof(SUPERIORREPORT_HEAD)+4
#define GETDATALEN(packbuff) (*(DWORD *)(packbuff+sizeof(SUPERIORREPORT_HEAD)))
#define MAX_RP_BUFF_SIZE 4096

int main()
{
	
};

void testbuff()
{
	fesl::CBuff cb;
	cb.Add("abcds",5);
	cb.Add("dbcg",5);
	printf("cb now : %s , addr %x\n",cb.GetBuff(),cb.GetBuff());
	fesl::CBuff cb2=cb;
	printf("cb2 now : %s, addr %x\n",cb2.GetBuff(),cb2.GetBuff());
	cb.Clear();
	printf("cb2 now : %s, addr %x,and the cb addr %x\n",cb2.GetBuff(),cb2.GetBuff()[0],cb.GetBuff());
}
int testSocket()
{
	//cpioRt=new fe_Cpio_Reactor();
	fesl::initsocketlib();
	fesl::Inet_addr addr(7890);

	SOCKET m_sockListen = fesl::feListenAddr((sockaddr_in *)&addr);

	fesl::Inet_addr rmaddr(0,0);

	while(TRUE)
	{
		SOCKET saccept = fesl::feWSAAccept( m_sockListen, (sockaddr*)&rmaddr);
		const char chOpt = 1;

		DWORD Flags=0;
		DWORD RecvBytes;




		int i=0;

	}



	fesl::cleanupWinsock();
}

