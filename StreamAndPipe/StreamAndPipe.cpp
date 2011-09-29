// StreamAndPipe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <io.h>
int main(int argc, char* argv[])
{
	//C��stdout,stderror,stdin ��Ϊ�ļ����������ļ���������ͨ��,
	//����Ȼprintf �õľ���stdout...,��stdout�ı��ˡ�printf�Ͳ�������
	fprintf(stdout,"Hello World!\n");
	fwrite("fwrite\n",1,7,stdout);
	char a[2];
	a[0]=11;
	a[1]='\n';
	fwrite(a,1,2,stdout);
	printf("printf\n");

	//C�е�����windows HANDLE�Ĺ�ϵ��ת��
	//file ת handle 
	//������Կ�����windows ��ϵͳ��� > c����� ��ϵͳ��������������.�����ض���������
	//_open_osfhandle���ڴ�ϵͳ�ļ������һ��c�����������
	//��_get_osfhandle�Ƿ���C���ļ������ʵ�ʶ�Ӧϵͳ���;
	//Ҳ���Է��֣�һ��ϵͳ������Զ�Ӧn��c���ļ����.
	//fdopen Ϊһ��c�ļ��������һ��������
	HANDLE ss = GetStdHandle(STD_OUTPUT_HANDLE);
	int nc = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 0x4000);
	int nc2 = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 0x4000);
	HANDLE ns = (HANDLE)_get_osfhandle(nc);
	HANDLE ns2 = (HANDLE)_get_osfhandle(nc2);
    HANDLE h = (HANDLE)_get_osfhandle (_fileno (stdout));
    WriteFile( h,"123456\n",7,0,0);
	fwrite("654321\n",1,7,stdout);
	//handle ת file
//	FILE *f = _fdopen ( (int)h,"w");//h����stdout���ļ���������Ʋ������ٸ���һ����
//	fwrite("654321\n",1,7,f);
	

	//pipe ���� //�ܵ��õ���handle ʵ����Ҳ�ɵ��ļ����ʹ��
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;
	
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	CreatePipe(&hRead,&hWrite,&sa,0);
	//��stdout ָ�� hwrite;�ı�printf����䣬���䲻���������
	int nWc = _open_osfhandle((long)hWrite, 0x4000);
	
	FILE *pf = _fdopen(nWc,"w");
	*stdout = *pf;
	setvbuf(stdout, NULL, _IONBF, 0);
	printf("���ںڴ�����");


	//Ҫ��hRead���ٶ�����.
	char rbuff[1024]={0};
	DWORD dwRead;
	ReadFile(hRead,rbuff,1024,&dwRead,0);
	//��Ȼstdout�Ѹı䣬��STD_OUTPUT_HANDLE�ɻ�����
	strcat(rbuff,",������Ǳ��������\n");
	WriteFile( GetStdHandle(STD_OUTPUT_HANDLE),rbuff,strlen(rbuff),0,0);
	//��������������Ϳ��԰�һ����������������ĳ�����
	STARTUPINFO si;
	PROCESS_INFORMATION pi; 
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si); 
	si.dwFlags=STARTF_USESTDHANDLES;//�ؼ�
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_SHOW;
	//�����ﲻ���ԣ�������Ϊ����̨����ms����
	CreateProcess(NULL,"cmd.exe /c dir /?"
        ,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi);
	CloseHandle(hWrite);
	ReadFile(hRead,rbuff,10,&dwRead,0);
	
	WriteFile( GetStdHandle(STD_OUTPUT_HANDLE),rbuff,dwRead,0,0);

	return 0;
}

