#include "StdAfx.h"
#include "ServerMonitor.h"
#include "minidump.h"
#define MAXBUFFLEN					4096
#define SERVERMONITORPORT		7896
#define COMMANDHEAD				"\xFC\xF0\xF0\xFC"
#define COMMAND_GETSERVERSTATUS	1
#define COMMAND_RESTARTSERVICE	2
#define COMMAND_STARTSERVICE	3
//#define CONFIGFILENAME		

#define SERVICESTATUS_STOP		"s"
#define SERVICESTATUS_RUNNING	"r"
#define SERVICESTATUS_UNKNOW	"u"
#ifdef _DEBUG
#define RECVTIMEOUT		WSA_INFINITE
#else
#define	RECVTIMEOUT		2000
#endif // _DEBUG
 
using namespace std;
string g_strconfigfile;
string g_strserverini;
string g_appPath;


BOOL GetInnoSetupAppPath(const char *regkey,OUT char *path);

int main(int argc, char* argv[])
{	
	::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	g_appPath = Linkwork::Win32::GetAppPath();
	g_strconfigfile= g_appPath+ "ServerMonitor.ini";
	g_strserverini = g_appPath + "Server.ini";
//	g_strAlertServerini = g_appPath.substr(0,g_appPath.rfind('\\')) + "";
	

	SetCurrentDirectory(g_appPath.c_str());
	gLogger.init(g_strconfigfile.c_str(), "ServerMonitor");

#ifndef _DEBUG
	SetAutoMinidump();
#endif
	ServerMonitor sm;

	if ( (argc > 1) &&
		((*argv[1] == '-') || (*argv[1] == '/')) )
	{
		if ( _stricmp( "debug", argv[1]+1 ) == 0 )
		{
			//��������
			sm.debugservice();
			return TRUE;
		}	
	}

	if(!sm.startservice("ESServerMonitor"))
	{
		gLogger.debug("��������ʧ��");
	}
	
	return 0;
	
}

BOOL ServerMonitor::OnStop()
{

	SetEvent(_hstop);
	gLogger.info("����ֹͣ");
	return TRUE;
}

BOOL ServerMonitor::OnStart()
{
	//�������߳�
	gLogger.info("��������");
	_hstop = CreateEvent(NULL, TRUE, FALSE, NULL);
	_hmainListenThread = ::CreateThread(NULL, 0, ServerMonitor::ServerMonitorListenThread, this, 0, NULL);
	_hmainTimerThread = ::CreateThread(NULL, 0, ServerMonitor::ServerTimerThread, this, 0, NULL);
	return TRUE;
}


DWORD WINAPI ServerMonitor::ServerMonitorListenThread(LPVOID lpParam)
{
	ServerMonitor *sm=(ServerMonitor *)lpParam;
	unsigned short port=SERVERMONITORPORT;
	int retval;
	struct sockaddr_in local;
	WSADATA wsaData;
	SOCKET msgsocket=INVALID_SOCKET;
	SOCKET s_listen = 0;
	WSAEVENT hListenEvent=0;
	
	try
	{
		if ((retval = WSAStartup(0x202,&wsaData)) != 0)
		{
			gLogger.error("[ServerMonitorListenThread] WSAStartup failed : %d",WSAGetLastError());
			throw "WSAStartup failed";
		}
		
		local.sin_family = AF_INET;
		local.sin_addr.s_addr = INADDR_ANY; 
		local.sin_port = htons(port);

		s_listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		
		
		if(s_listen == INVALID_SOCKET)
		{
			gLogger.error("[ServerMonitorListenThread] socket failed : %d",WSAGetLastError());
			throw "socket failed";
		}
		
		hListenEvent = WSACreateEvent();
		
		if( SOCKET_ERROR == WSAEventSelect(s_listen, hListenEvent, FD_ACCEPT) )
		{
			gLogger.error("[ServerMonitorListenThread] WSAEventSelect failed : %d",WSAGetLastError());
			throw "WSAEventSelect failed";
		}
		
		if(bind(s_listen,(SOCKADDR *)&local,sizeof(local))==SOCKET_ERROR)
		{
			gLogger.error("[ServerMonitorListenThread] bind failed : %d",WSAGetLastError());
			throw "bind failed";
		}
		
		if( SOCKET_ERROR == listen(s_listen, 5) )
		{
			gLogger.error("[ServerMonitorListenThread] listen failed : %d",WSAGetLastError());
			throw "listen failed";
		}
	}
	catch (char *e)
	{
		if(s_listen != 0 )
			closesocket(s_listen);
		if(hListenEvent !=NULL)
			WSACloseEvent(hListenEvent);
		WSACleanup();
		printf(e);
		sm->stopservice();
		return 10;
	}
	


	DWORD dwRet;
	WSANETWORKEVENTS events;
	WSAEVENT handles[]={sm->_hstop,hListenEvent};
	gLogger.info("[ServerMonitorListenThread] listen begin");
	while(TRUE)
	{
		dwRet = WSAWaitForMultipleEvents(2, handles, FALSE, WSA_INFINITE, FALSE);
		if(WSA_WAIT_EVENT_0 == dwRet)//�˳���������
			break;
		if( WSA_WAIT_TIMEOUT == dwRet )
			continue;
		int nRet = WSAEnumNetworkEvents(s_listen, hListenEvent, &events);
		if( SOCKET_ERROR == nRet )
			break;
		WSAResetEvent(hListenEvent);
		if( events.lNetworkEvents & FD_ACCEPT )
		{
			if( events.iErrorCode[FD_ACCEPT_BIT] != 0 )
				break;
		}
		else
			continue;

		SOCKADDR_IN remoteaddr;
		int remoteaddrlen;
		SOCKET s_Accept;
		
		remoteaddrlen = sizeof(remoteaddr);
		s_Accept = WSAAccept( s_listen, (sockaddr*)&remoteaddr, &remoteaddrlen, NULL, NULL );
		if ( INVALID_SOCKET == s_Accept )
		{
			gLogger.error("[ServerMonitorListenThread] WSAAccept failed : %d",WSAGetLastError());
			continue;
		}
		gLogger.debug("[ServerMonitorListenThread] WSAAccept  %u",ntohl(remoteaddr.sin_addr.S_un.S_addr));
		
		HANDLE ht=CreateThread(NULL, 0, TcpDataThread, (LPVOID)s_Accept, 0, NULL);
		if(ht)
			CloseHandle(ht);
	}

	closesocket(s_listen);
	WSACloseEvent(hListenEvent);
	WSACleanup();

	return 0;

}

DWORD WINAPI ServerMonitor::TcpDataThread(LPVOID lParam)
{
	ServerMonitor *sm = (ServerMonitor *)WinService::GetService();
	SOCKET s_accept=(SOCKET)lParam;
	
	DWORD dwBytes;
	DWORD flags=0;
	
	WSAOVERLAPPED overlapped = {0};
	WSAEVENT hEvent = WSACreateEvent();
	overlapped.hEvent=hEvent;
	
	WSABUF wsabuf;
	char buff[MAXBUFFLEN];
	wsabuf.buf=buff;
	wsabuf.len=MAXBUFFLEN;
	WSAEVENT handles[]={sm->_hstop,hEvent};
	int retval=0;
	DWORD dwTotalBytes=strlen(COMMANDHEAD)+4+4; //plus windows number
	DWORD recvlen=0;
	DWORD exlen= -1;
	while(TRUE)
	{
		flags = 0;
		memset(&overlapped, 0, sizeof(overlapped));
		overlapped.hEvent = hEvent;
		if(WSARecv(s_accept,&wsabuf,1,&dwBytes,&flags,&overlapped,NULL)==SOCKET_ERROR)
		{
			int error=WSAGetLastError();
			if(error!=WSA_IO_PENDING)
			{
				gLogger.error("[TcpDataThread] WSARecv failed: %u", error);
				break;
			}
		}

		DWORD dwRet=WSAWaitForMultipleEvents(2, handles, FALSE, RECVTIMEOUT, FALSE);
		if(WSA_WAIT_EVENT_0 + 1 != dwRet)
		{
			if(WSA_WAIT_TIMEOUT == dwRet)
			{
				gLogger.debug("[TcpDataThread] WSARecv timeout");
				printf("[TcpDataThread] WSARecv timeout\n");
			}
			break;
		}
		if(dwRet==WSA_WAIT_FAILED)
		{
			int error=GetLastError();
			gLogger.error("[TcpDataThread] WaitForMultipleObjects failed: %u", error);
			break;
		}
		
		dwBytes=0;
		retval=WSAGetOverlappedResult(s_accept,&overlapped,&dwBytes,FALSE,&flags);
		
		if(retval==FALSE)
		{
			int error=WSAGetLastError();
			if(WSA_IO_INCOMPLETE == error)
			{
				continue;
			}
			else
			{
				gLogger.error("[TcpDataThread] WSAGetOverlappedResult failed: %u", error);
				break;
			}
		}
		
		if(dwBytes==0)
		{
			break;
		}

		
		recvlen+=dwBytes;
		wsabuf.buf=buff+recvlen;
		wsabuf.len=MAXBUFFLEN -recvlen;
		WSAResetEvent(hEvent);
		
		if(exlen==-1 && recvlen >= dwTotalBytes)
		{
			exlen = *(DWORD *)(buff+strlen(COMMANDHEAD)+4);
			dwTotalBytes+=exlen;
		}

		if(recvlen >= dwTotalBytes)
			break;
	}

	if(recvlen >= dwTotalBytes)//������ɣ�
	{
		printf("recv ok!\n");
		DWORD sendLen=dwTotalBytes;
		gLogger.debug("[TcpDataThread] TotalRecvBytes %u",dwTotalBytes);
		sm->HandleCommand(buff,&sendLen);//������Ϻ󣬰�buff���ظ��Է���
		gLogger.debug("[TcpDataThread] TotalSendBytes %u",sendLen);
		if(sendLen != 0)//���ͷ�������
		{
			while(TRUE)
			{
				wsabuf.buf = buff;
				wsabuf.len = sendLen;
				memset(&overlapped, 0, sizeof(overlapped));
				overlapped.hEvent = hEvent;
				if(WSASend(s_accept,&wsabuf,1,&sendLen,0,&overlapped,NULL)==SOCKET_ERROR)
				{
					int error=WSAGetLastError();
					if(error!=WSA_IO_PENDING)
					{
						gLogger.error("[TcpDataThread] WSASend failed: %u", error);
						break;
					}
				}
				
				
				DWORD dwRet=WSAWaitForMultipleEvents(2, handles, FALSE, RECVTIMEOUT, FALSE);
				if(WSA_WAIT_EVENT_0 + 1 != dwRet)
				{
					if(WSA_WAIT_TIMEOUT == dwRet)
					{
						gLogger.info("[TcpDataThread] WSASend timeout");
						printf("[TcpDataThread] WSASend timeout");
					}
					break;
				}	
				if(dwRet==WSA_WAIT_FAILED)
				{
					int error=GetLastError();
					gLogger.error("[TcpDataThread] WaitForMultipleObjects failed: %u", error);
					break;
				}

				retval=WSAGetOverlappedResult(s_accept,&overlapped,&dwBytes,FALSE,&flags);
				if(retval==FALSE)
				{
					int error=WSAGetLastError();
					if(WSA_IO_INCOMPLETE == error)
					{
						RaiseException(0, 0, 0, NULL);
						continue;
					}
				}
				break;
			}
		}
	}
	else
	{
		printf("recv error\n");
		gLogger.info("[TcpDataThread] recv error");
	}
	
	WSACloseEvent(hEvent);
	gLogger.debug("[TcpDataThread] closesocket");
	//WSACloseEvent(hEvent);
	shutdown(s_accept,SD_BOTH);
	closesocket(s_accept);
	return 0;
}


void ServerMonitor::HandleCommand(char *msgbuff,DWORD *len)
{	
	*len=0;
	if(strnicmp(msgbuff,COMMANDHEAD,strlen(COMMANDHEAD))!=0)//�Ǳ����������
		return;
	//�����
	int command = *(int *)(msgbuff + strlen(COMMANDHEAD));
	int paramlen = *(int *)(msgbuff + strlen(COMMANDHEAD)+4);
	char *param = msgbuff + strlen(COMMANDHEAD) + 8;//��������;
	param[paramlen]=0;
	if(paramlen>80)
	{
		gLogger.error("[HandleCommand] ����̫�����ܾ�����");
		return;
	}
	char cmd[100];
	switch (command)
	{
	case COMMAND_GETSERVERSTATUS://��ȡ������״̬�б�
		GetServerStatus(msgbuff,len);
		break;
	case COMMAND_RESTARTSERVICE://����������
		strcpy(cmd, "net stop ");
		strncat(cmd, param, sizeof(cmd));
		system(cmd);

		sprintf(cmd,"taskkill /f /im %s.exe",param);
		system(cmd);

		strcpy(cmd, "net start ");
		strncat(cmd, param, sizeof(cmd));
		//sprintf(cmd,"net start %s",param);
		system(cmd);
		system(cmd);//�ٴ�ȷ���ѱ�ɱ��;
		Sleep(2000);
		gLogger.info("restart service %s",param);
		GetServerStatus(msgbuff,len);//Ҳ�ط���ȥ
		//*len=0;//���ػط�
		break;
	case COMMAND_STARTSERVICE://ֹͣ������
		strcpy(cmd, "net stop ");
		strncat(cmd, param, sizeof(cmd));
		//sprintf(cmd,"net stop %s",param);
		system(cmd);
		Sleep(2000);
		gLogger.info("stop service %s",param);
		GetServerStatus(msgbuff,len);//Ҳ�ط���ȥ
		//*len=0;//���ػط�
		break;
	default:
		printf("undefined command :%d\n",command);
		gLogger.info("undefined command :%d\n",command);
		break;
	}
}

int ServerMonitor::GetCpuStatus()
{
	//ԭ��:,��NtQuerySystemInformation����idle������ʱ�䣬����ϵͳ����ʱ�䣬�����Եõ�idle��ռ�ðٷֱȣ������аٷֱȣ�,
	//��������cpuʹ���ʡ�
	//�����N�˴�����,���õ���idleʱ�佫��N��������Ҫ����N.
	//��һ����ȡ�� ϵͳʱ�估idle����ʱ�䣬��¼����������N�����ȡ��ϵͳʱ�估idle����ʱ�䣬���������������յõ�
	//��N���ڵ�ƽ��cpuʱ�䡣
	gLogger.debug("GetCpuStatus");
	DWORD cpuPercent=0;

	SYSTEM_PERFORMANCE_INFORMATION   SysPerfInfo;   
	SYSTEM_TIME_INFORMATION   SysTimeInfo;   
	SYSTEM_BASIC_INFORMATION   SysBaseInfo;   
	double   dbIdleTime;   
	double   dbSystemTime;   
	LONG   status;   
	LARGE_INTEGER   liOldIdleTime   =   {0,0};   
	LARGE_INTEGER   liOldSystemTime   =   {0,0};   
    
	NtQuerySystemInformation   =   (PROCNTQSI)GetProcAddress(GetModuleHandle("ntdll"),"NtQuerySystemInformation");   
    
	if   (!NtQuerySystemInformation)   
		return cpuPercent;   
    
	//   get   number   of   processors   in   the   system   
	status   =   NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL);   
	if   (status   !=   NO_ERROR)   
		return cpuPercent;   
    
	while(TRUE)   
	{   
		//   ��ȡϵͳʱ��
		status   =   NtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),0);   
		if   (status!=NO_ERROR)   
			return cpuPercent;   
		
		//��ȡidle������ʱ�䡣
		status   =NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL);   
		if   (status   !=   NO_ERROR)   
			return cpuPercent;   
		  
		if   (liOldIdleTime.QuadPart   !=   0)   //��һ��Ϊ�ȽϵĻ��㣬�����㡣
		{   
			dbIdleTime   =   Li2Double(SysPerfInfo.liIdleTime)   -   Li2Double(liOldIdleTime);   
			dbSystemTime   =   Li2Double(SysTimeInfo.liKeSystemTime)   - Li2Double(liOldSystemTime);   
			
			dbIdleTime   =   dbIdleTime   /   dbSystemTime;   
			
			cpuPercent   =   100.0   -   dbIdleTime   *   100.0   /     
				(double)SysBaseInfo.bKeNumberProcessors   +   0.5;   

			break; 
		}   
		   
		liOldIdleTime   =   SysPerfInfo.liIdleTime;   
		liOldSystemTime   =   SysTimeInfo.liKeSystemTime;   
		  
		Sleep(200);//����0.2���ƽ��ֵ   
	}    
	return cpuPercent;
}

void ServerMonitor::GetMemStatus(DWORD *pdwFreeMem,DWORD *pdwTotalMem)//�Ե�λmb�ϱ�
{
	MEMORYSTATUSEX MemStatex;
	MemStatex.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&MemStatex);//��mbΪ��λ
	*pdwFreeMem  = (DWORD)(MemStatex.ullAvailPhys/1024/1024);
	*pdwTotalMem = (DWORD)(MemStatex.ullTotalPhys/1024/1024);
	gLogger.info("GetMemStatus %d/%d",*pdwFreeMem,*pdwTotalMem);
}

typedef BOOL (WINAPI *PGETDISKFREESPACEEX)(LPCSTR,
										   PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);
void ServerMonitor::GetUseDisk(OUT DWORD* pdwAllsize,OUT DWORD *pdwFreesize)
{
//	g_strserverini
	gLogger.debug("GetUseDisk");
	char driver[5];
	const char *inipath=g_strserverini.c_str();
	_splitpath(inipath,driver,NULL,NULL,NULL);


	PGETDISKFREESPACEEX pGetDiskFreeSpaceEx;
	__int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;
	
	DWORD dwSectPerClust, 
		dwBytesPerSect, 
		dwFreeClusters, 
		dwTotalClusters;
	
	BOOL fResult;
	
	pGetDiskFreeSpaceEx = (PGETDISKFREESPACEEX) GetProcAddress( 
		GetModuleHandle("kernel32.dll"),
		"GetDiskFreeSpaceExA");
	
	if (pGetDiskFreeSpaceEx)
	{
		fResult = pGetDiskFreeSpaceEx (driver,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);
		
		// Process GetDiskFreeSpaceEx results.
		if(fResult) 
		{
			printf("Total bytes = %I64d\n", i64TotalBytes);
			printf("Total free bytes = %I64d\n", i64FreeBytes);
			*pdwAllsize=(DWORD)(i64TotalBytes/1024/1024);
			*pdwFreesize=(DWORD)(i64FreeBytes/1024/1024);
		}
		return ;
	}
	
	else 
	{
		fResult = GetDiskFreeSpaceA (driver, 
			&dwSectPerClust, 
			&dwBytesPerSect,
			&dwFreeClusters, 
			&dwTotalClusters);
		
		// Process GetDiskFreeSpace results.
		if(fResult) 
		{
			printf("Total bytes = I64d\n", 
				dwTotalClusters*dwSectPerClust*dwBytesPerSect);
			printf("Total free bytes = %I64d\n",
				dwFreeClusters*dwSectPerClust*dwBytesPerSect);
			*pdwAllsize=(DWORD)(dwTotalClusters*dwSectPerClust*dwBytesPerSect/1024/1024);
			*pdwFreesize=(DWORD)(dwFreeClusters*dwSectPerClust*dwBytesPerSect/1024/1024);

		}
	}
}
void ServerMonitor::GetDBUseDisk(OUT DWORD* pdbhostip,OUT DWORD *freesize,OUT DWORD *dbstate)
{
	gLogger.debug("GetDBUseDisk");
	
	DWORD ret=0;
	*dbstate=0;
	//wmic���� wmic LOGICALDISK get name,Description,filesystem,size,freespace   //������㡣��SAûȨ�ް�������
	//dir����	dir g:\* /a/s | findstr /i "���ļ� ��Ŀ¼ File(s) Dir(s) total	//���������ʹ�������Ϳ�����������Ч��ѡ������
	char path[MAX_PATH];
	//��ȡ�澯������iniλ��
	string strAlertServerini;
	if(GetInnoSetupAppPath("�쫑�澯������_is1",path))
	{
		strAlertServerini = path;
		strAlertServerini += "\\AlertServer.ini";
	}
	
	char constr[1024];
	//��ȡ���ݿ����Ӵ�
	//Server.ini
	char host[100];
	char Database[100];
	char UserID[100];
	char Password[100];
	GetPrivateProfileString("SQLServer", "Host", "", host, sizeof(host), strAlertServerini.c_str());
	GetPrivateProfileString("SQLServer", "Database", "", Database, sizeof(Database), strAlertServerini.c_str());
	GetPrivateProfileString("SQLServer", "UserID", "", UserID, sizeof(UserID), strAlertServerini.c_str());
	GetPrivateProfileString("SQLServer", "Password", "", Password, sizeof(Password), strAlertServerini.c_str());
	if(host[0]==0||Database[0]==0||UserID[0]==0||Password[0]==0)
	{
		gLogger.error("[GetUseDisk] get db setting error!,%s,%s,%s,%s,%s,%s",host,Database,UserID,Password,strAlertServerini.c_str());
		*dbstate=2;//���ݿ����ò���ȷ
		return;
	}
	
	sprintf(constr,"Server=%s;uid=%s;pwd=%s;DATABASE=%s;Provider=SQLOLEDB;",host,UserID,Password,Database);
	//
	char* ch = strrchr(host,',');
	if(ch)
		*ch=0;
	*pdbhostip = ntohl(inet_addr(host));

	_ConnectionPtr Conn;
	_RecordsetPtr rs;
	_variant_t temp;
	_variant_t freeSpaceStr;
	char dbDriver[5];
	char sql[1024] = "";
	try                
    {
		Conn.CreateInstance(__uuidof(Connection));
		Conn->ConnectionTimeout=2;//���ӳ��ԣ�ֻ�ȴ�2��
		Conn->Open(constr,"","",-1);
	}
	catch(_com_error e)
	{
			printf("���ݿ�����ʧ��!,%s,%u",(char *)_bstr_t(e.Description()),e.Error());
			gLogger.error("[GetDBUseDisk] ���ݿ�����ʧ��!,%s,%u",(char *)_bstr_t(e.Description()),e.Error());
			*dbstate=1;//���ݿ�����ʧ��
    }

	if(*dbstate!=0)
	{
		*freesize=ret;
		return;
	}
	try
	{
		rs = Conn->Execute("SELECT physical_name FROM sys.database_files",NULL,adCmdText);
		if(!rs->adoEOF)
		{
			_variant_t v1(0L);
			temp = rs->GetCollect(v1);
			if(temp.vt!=VT_NULL&&temp.vt!=VT_EMPTY)
			{
				_bstr_t dbpath = temp;
				if(dbpath.length()!=0)
				{
					_splitpath((char *)dbpath,dbDriver,NULL,NULL,NULL);
					gLogger.debug("[GetDBUseDisk] Driver %s",dbDriver);
					sprintf(sql,"exec master..xp_cmdshell 'dir %s\\ | findstr /i \"Dir(s) ��Ŀ¼\"'",dbDriver);
					rs->Close();
					rs=NULL;
				}
				else
				{
					rs->Close();
					rs=NULL;
					return;
				}
				
			}
			else
			{
				rs->Close();
				rs=NULL;
				printf("��ȡ���ݿ���̿��пռ�ʧ��!database_files Ϊ��");
				gLogger.error("[GetDBUseDisk] ��ȡ���ݿ���̿��пռ�ʧ��!database_files Ϊ��");
				*dbstate=3;//��ȡ���пռ�ʧ��.
				return;
			}
			
		}
		else
		{
			printf("��ȡ���ݿ���̿��пռ�ʧ��!database_files Ϊ��");
			gLogger.error("[GetDBUseDisk] ��ȡ���ݿ���̿��пռ�ʧ��!database_files Ϊ��");
			*dbstate=3;//��ȡ���пռ�ʧ��.
			return;
		}
		rs = Conn->Execute(sql,NULL,adCmdText);
		
		if(!rs->adoEOF)
		{
			while(!rs->adoEOF)
			{
				_variant_t v1(0L);
				temp = rs->GetCollect(v1);
				if(temp.vt!=VT_NULL)
					freeSpaceStr = rs->GetCollect(v1);
				rs->MoveNext();
			}
			char cfree[1024];
			_bstr_t bstrfs = freeSpaceStr;
			sscanf((char *)bstrfs,"%*s%*s%s",cfree);
			gLogger.debug("[GetDBUseDisk] free %s",cfree);
			ret = GetNUMFromSeparateStr(cfree)/1024/1024;
			rs->Close();
			rs=NULL;
		}
	}
	catch(_com_error e)
	{
        printf("��ȡ���ݿ���̿��пռ�ʧ��!,%s,%u",(char *)_bstr_t(e.Description()),e.Error());
		gLogger.error("[GetDBUseDisk] ��ȡ���ݿ���̿��пռ�ʧ��!,%s,%u",(char *)_bstr_t(e.Description()),e.Error());
		*dbstate=3;//��ȡ���пռ�ʧ��.
    }
	*freesize = ret;
}

BOOL ServerMonitor::GetServerStatus(OUT char *buff,OUT DWORD *len)
{
	//			cpu	mem	sqlserver	
	//״̬buff	20	50	60			servicelistlen servicelist
	gLogger.debug("get service status list");
	*len =0 ;
	string sl=GetServiceList();
	DWORD sls = sl.size();
	if(sls > (MAXBUFFLEN-36))
		return FALSE;
	DWORD MemFree=0,MemTotal=0;
	DWORD DiskFree=0,DiskTotal=0;
	DWORD DbDiskFree=0,DbIp=0,Dbstate=0;
	int cpus = GetCpuStatus();
	GetMemStatus(&MemFree,&MemTotal);
	GetUseDisk(&DiskTotal,&DiskFree);
	GetDBUseDisk(&DbIp,&DbDiskFree,&Dbstate);
	
	memcpy(buff,&cpus,4);
	memcpy(buff+4,&MemFree,4);
	memcpy(buff+8,&MemTotal,4);
	memcpy(buff+12,&DiskFree,4);
	memcpy(buff+16,&DiskTotal,4);
	memcpy(buff+20,&DbDiskFree,4);
	memcpy(buff+24,&DbIp,4);
	memcpy(buff+28,&Dbstate,4);

	memcpy(buff+32,&sls,4);
	memcpy(buff+36,sl.c_str(),sls);

	*len = 36 +  sls;

	return TRUE;
}

string ServerMonitor::GetServiceList()
{
	string rpstr;
	//������
	//��ini�ж�ȡ����
	gLogger.debug("GetServiceList");
	char serverlistset[1024];
	GetPrivateProfileString("servicelist", "servicelist", "", serverlistset, sizeof(serverlistset), g_strconfigfile.c_str());
	
	vector<string> vecserverlist;
	
	string strsl=serverlistset;
	Linkwork::String::SplitString(strsl,',',vecserverlist);
	//�򿪷��������;
	SC_HANDLE schSCManager = OpenSCManager( 
		NULL,                    // local machine 
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if(schSCManager == NULL)
		return rpstr;

	//��Ҫ���񹹳��ַ��� ��:esserver|r|esalertservice|s|	
	
	for (vector<string>::iterator ite=vecserverlist.begin();ite !=vecserverlist.end();ite++)
	{
		//�������Ƿ���ڡ������������Ƿ���������
		SC_HANDLE hSvc = ::OpenService( schSCManager, ite->c_str(),SERVICE_QUERY_STATUS);
		if( hSvc == NULL)
		{
			//û��װ
		}
		else
		{
			rpstr+=ite->c_str();
			rpstr+="|";
			SERVICE_STATUS status;
			if(::QueryServiceStatus( hSvc, &status)==FALSE)
			{
				rpstr+="u|";
			}
			else if(status.dwCurrentState==SERVICE_RUNNING)
			{
				rpstr+="r|";
			}
			else if(status.dwCurrentState==SERVICE_STOPPED)
			{
				rpstr+="s|";
			}
			else
			{
				rpstr+="s|";
			}
			::CloseServiceHandle(hSvc);
		}

	}


	::CloseServiceHandle( schSCManager);	
	return rpstr;
}

__int64 ServerMonitor::GetNUMFromSeparateStr(const char *srcstr)
{
	__int64 ret=0;
	int len=strlen(srcstr);
	char *destemp=new char[len+1];
	int srcpos=0;
	int despos=0;
	while(srcstr[srcpos])
	{
		if(srcstr[srcpos]>='0'&&srcstr[srcpos]<='9')
			destemp[despos++]=srcstr[srcpos];
		++srcpos;
	}
	destemp[despos]=0;
	ret = _atoi64(destemp);
	delete [] destemp;
	return ret;
}


BOOL GetInnoSetupAppPath(const char *regkey,OUT char *path)
{
	int ret;
	char regpath[MAX_PATH];
	sprintf(regpath,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s",regkey);
	HKEY regKey=NULL;
	DWORD type=REG_SZ;
	DWORD dwAidSize=MAX_PATH;
	ret=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,regpath,0,KEY_READ,&regKey);
	if(regKey)
	{
		ret=::RegQueryValueEx(regKey,"Inno Setup: App Path",0,&type,(LPBYTE)path,&dwAidSize);
		RegCloseKey(regKey);
		if(ret == ERROR_SUCCESS)
			return TRUE;
	}
	return FALSE;
}

DWORD WINAPI ServerMonitor::ServerTimerThread(LPVOID lpParam)
{
	ServerMonitor *sm=(ServerMonitor *)lpParam;
	sm->initTimerTaskList();
	DWORD compensateTime=0;//����ʱ��
	while(sm->timertasklist.size())//ֻҪ�������񶼼�������
	{
		//ȡ���´�����ʱ��
		timertask &nexttask = *sm->timertasklist.begin();
		time_t nowt =time(0);
		int waittime = nexttask.nextTime - nowt;
		waittime = waittime>0?waittime:0;
		DWORD dwRet = WaitForSingleObject(sm->_hstop,waittime*1000);
		switch( dwRet )
		{
		case WAIT_OBJECT_0://�˳���,ɶҲ����
			gLogger.info("[ServerTimerThread] stop");
			return 0;
			break;
		case WAIT_TIMEOUT:
			nowt = time(0);
			if(nowt>=nexttask.nextTime)
			{
				gLogger.info("[ServerTimerThread] run task %s",nexttask.taskname.c_str());
				nexttask.runcommand();
				nexttask.calculateNextTime();
				if(nexttask.isInvalidTask())
					sm->timertasklist.erase(sm->timertasklist.begin());
			}

			break;
		default:
			break;
		}
		sm->timertasklist.sort();//����
	}
	//
	gLogger.info("[ServerTimerThread] empty task! ServerTimerThread stop");
	return 0;
}

void ServerMonitor::initTimerTaskList()
{
	//��ini�м����������
	gLogger.debug("[ServerTimerThread] load TimerTask setting!");
	char tasklistset[1024];
	GetPrivateProfileString("timerTask","tasklist","",tasklistset,1023,g_strconfigfile.c_str());
	
	vector<string> vecTaskList;
	 
	string strtl=tasklistset;
	Linkwork::String::SplitString(strtl,',',vecTaskList);
	
	for (vector<string>::iterator ite=vecTaskList.begin();ite != vecTaskList.end();ite++)
	{
		timertask task;
		task.taskname= *ite;
		char commondlist[1024];
		char timeSetting[1024];

		task.tasktype = GetPrivateProfileInt(ite->c_str(),"type",type_none,g_strconfigfile.c_str());
		task.weekdayMask = GetPrivateProfileInt(ite->c_str(),"weekday",0,g_strconfigfile.c_str());
		task.monthday = GetPrivateProfileInt(ite->c_str(),"monthday",0,g_strconfigfile.c_str());
		
		GetPrivateProfileString(ite->c_str(),"commondlist","",commondlist,1023,g_strconfigfile.c_str());
		GetPrivateProfileString(ite->c_str(),"time","",timeSetting,1023,g_strconfigfile.c_str());
		if(timeSetting[0]==0)
		{
			gLogger.error("[initTimerTaskList] load TimerTask : %s error timesetting,errorcode:%u",ite->c_str(),GetLastError());
			continue;
		}
		if(commondlist[0]==0)
		{
			gLogger.error("[initTimerTaskList] load commondlist : %s error commondlist or empty commondlist,errorcode:%u",ite->c_str(),GetLastError());
			continue;
		}
		else
		{
			string strcl = commondlist;
			Linkwork::String::SplitString(strcl,'|',task.commandlist);
			if(task.commandlist.size()==0)
			{
				gLogger.error("[initTimerTaskList] load commondlist : %s empty commondlist",ite->c_str());
				continue;
			}
		}	

/*		task.tmsetting = timeSetting;*/
		time_t t=-1;
		tm tms={0};
		int num;

		switch(task.tasktype)
		{
		case type_once:
			//ʱ���ʽΪ yyyy-MM-dd hh:mm:ss
			num=sscanf(timeSetting,"%d%*[-/]%d%*[-/]%d %d:%d:%d",
		&tms.tm_year,&tms.tm_mon,&tms.tm_mday,&tms.tm_hour,&tms.tm_min,&tms.tm_sec);
			if(num == 6)//��ȷ�ĸ�ʽ
			{
				tms.tm_year-=1900;
				task.tmsetting = tms;
			}
			else
			{
				gLogger.error("[initTimerTaskList] load TimerTask : %s error timesetting,errorstring:%s",ite->c_str(),timeSetting);
				continue;
			}
			break;
		case type_everyday:
			num=sscanf(timeSetting,"%d:%d:%d",
				&tms.tm_hour,&tms.tm_min,&tms.tm_sec);
			if(num == 3)//��ȷ�ĸ�ʽ
			{
				task.tmsetting = tms;
			}
			else
			{
				gLogger.error("[initTimerTaskList] load TimerTask : %s error timesetting,errorstring:%s",ite->c_str(),timeSetting);
				continue;
			}
			break;
		case type_weekday:
			num=sscanf(timeSetting,"%d:%d:%d",
				&tms.tm_hour,&tms.tm_min,&tms.tm_sec);
			if(num == 3)//��ȷ�ĸ�ʽ
			{
				task.tmsetting = tms;
			}
			else
			{
				gLogger.error("[initTimerTaskList] load TimerTask : %s error timesetting,errorstring:%s",ite->c_str(),timeSetting);
				continue;
			}

			if(task.weekdayMask == 0)
			{
				gLogger.error("[initTimerTaskList] load weekdayMask : %s ,empty weekday set",ite->c_str());
				continue;
			}
			break;
		case type_everymonth:
			num=sscanf(timeSetting,"%d:%d:%d",
				&tms.tm_hour,&tms.tm_min,&tms.tm_sec);
			if(num == 3)//��ȷ�ĸ�ʽ
			{
				task.tmsetting = tms;
			}
			else
			{
				gLogger.error("[initTimerTaskList] load TimerTask : %s error timesetting,errorstring:%s",ite->c_str(),timeSetting);
				continue;
			}
			break;
		default:
			gLogger.error("[initTimerTaskList] load TimerTask : %s error type,errorcode:%u",ite->c_str(),GetLastError());
			continue;
			break;
		}
		gLogger.debug("[initTimerTaskList] push task %s",task.taskname.c_str());
		timertasklist.push_back(task);
	}


	for (taskiterator iteTask=timertasklist.begin();iteTask != timertasklist.end();)
	{
		iteTask->calculateNextTime();
		if(iteTask->isInvalidTask())
		{
			gLogger.debug("[initTimerTaskList] erase task %s",iteTask->taskname.c_str());
			taskiterator __tempite =  iteTask++;
			timertasklist.erase(__tempite);
			
		}
		else
			iteTask++;
	}
	timertasklist.sort();
}
	
