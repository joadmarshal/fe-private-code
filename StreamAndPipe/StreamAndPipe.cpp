// StreamAndPipe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <io.h>
int main(int argc, char* argv[])
{
	//C中stdout,stderror,stdin 作为文件流对象与文件操作函数通用,
	//且显然printf 用的就是stdout...,若stdout改变了。printf就不正常了
	fprintf(stdout,"Hello World!\n");
	fwrite("fwrite\n",1,7,stdout);
	char a[2];
	a[0]=11;
	a[1]='\n';
	fwrite(a,1,2,stdout);
	printf("printf\n");

	//C中的流与windows HANDLE的关系及转换
	//file 转 handle 
	//这里可以看出在windows 中系统句柄 > c流句柄 有系统句柄才能有流句柄.有流必定有其句柄，
	//_open_osfhandle用于打开系统文件句柄的一个c流句柄的引用
	//而_get_osfhandle是返回C流文件句柄的实际对应系统句柄;
	//也可以发现，一个系统句柄可以对应n个c流文件句柄.
	//fdopen 为一个c文件句柄创建一个流对象
	HANDLE ss = GetStdHandle(STD_OUTPUT_HANDLE);
	int nc = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 0x4000);
	int nc2 = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 0x4000);
	HANDLE ns = (HANDLE)_get_osfhandle(nc);
	HANDLE ns2 = (HANDLE)_get_osfhandle(nc2);
    HANDLE h = (HANDLE)_get_osfhandle (_fileno (stdout));
    WriteFile( h,"123456\n",7,0,0);
	fwrite("654321\n",1,7,stdout);
	//handle 转 file
//	FILE *f = _fdopen ( (int)h,"w");//h已是stdout的文件句柄，估计不允许再给另一个流
//	fwrite("654321\n",1,7,f);
	

	//pipe 测试 //管道得到的handle 实际上也可当文件句柄使用
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;
	
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	CreatePipe(&hRead,&hWrite,&sa,0);
	//将stdout 指向 hwrite;改变printf等语句，让其不输出到窗口
	int nWc = _open_osfhandle((long)hWrite, 0x4000);
	
	FILE *pf = _fdopen(nWc,"w");
	*stdout = *pf;
	setvbuf(stdout, NULL, _IONBF, 0);
	printf("不在黑窗出现");


	//要由hRead里再读出来.
	char rbuff[1024]={0};
	DWORD dwRead;
	ReadFile(hRead,rbuff,1024,&dwRead,0);
	//虽然stdout已改变，但STD_OUTPUT_HANDLE可还存在
	strcat(rbuff,",结果还是被抽出来了\n");
	WriteFile( GetStdHandle(STD_OUTPUT_HANDLE),rbuff,strlen(rbuff),0,0);
	//于是用这个方法就可以把一个程序的输出读到你的程序中
	STARTUPINFO si;
	PROCESS_INFORMATION pi; 
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si); 
	si.dwFlags=STARTF_USESTDHANDLES;//关键
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_SHOW;
	//在这里不可以，父进程为控制台程序ms不行
	CreateProcess(NULL,"cmd.exe /c dir /?"
        ,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi);
	CloseHandle(hWrite);
	ReadFile(hRead,rbuff,10,&dwRead,0);
	
	WriteFile( GetStdHandle(STD_OUTPUT_HANDLE),rbuff,dwRead,0,0);

	return 0;
}

