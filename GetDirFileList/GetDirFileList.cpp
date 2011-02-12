// GetDirFileList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "md5/md5.h"
#include <Windows.h>

class CFilelist
{
public:
	void ProcessFile(const char * lpdir,WIN32_FIND_DATA FindFileData)
	{
		char md5buff[40];
		char filepath[MAX_PATH];
		strcpy(filepath,lpdir);
		strcat(filepath,"\\");
		strcat(filepath,FindFileData.cFileName);
		MD5::FileToMd5(filepath,md5buff);
		printf("%s|%s\n",filepath+rootdirlen+1,md5buff);
	}
	void setrootdir(const char *lpdir)
	{
		strcpy(rootdir,lpdir);
		rootdirlen = strlen(lpdir);
	}
private:
	char rootdir[MAX_PATH];
	int rootdirlen;

};

template <
typename T
>
void emundir(const char *lpdir,T &processfun)
{
	char lpfstr[MAX_PATH+2];
	strncpy(lpfstr,lpdir,MAX_PATH);
	strcat(lpfstr,"\\*");
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = NULL;
	hFind = FindFirstFile(lpfstr, &FindFileData);
	do 
	{
		if (hFind == INVALID_HANDLE_VALUE) 
			continue;
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)//隐藏文件忽略
			continue;
		if (strcmp(".",FindFileData.cFileName) !=0 && strcmp("..",FindFileData.cFileName)!=0)
		{
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char subdir[MAX_PATH];
				strcpy(subdir,lpdir);
				strcat(subdir,"\\");
				strcat(subdir,FindFileData.cFileName);
				emundir(subdir,processfun);
			}
			else
				processfun.ProcessFile(lpdir,FindFileData);
		}
	} while (FindNextFile(hFind, &FindFileData));
	if(GetLastError()==ERROR_NO_MORE_FILES)
		FindClose(hFind);
}

int main(int argc, char* argv[])
{
	char lpdir[MAX_PATH]={0};
	if(argc > 1)
	{
		strncpy(lpdir,argv[1],MAX_PATH);
	}
	else
	{
		GetModuleFileName(NULL,lpdir,MAX_PATH);
		char *t=strrchr(lpdir,'\\');
		t[0]=0;
	}

	if(argc > 2)
	{
		//重定向到文件
		FILE *fp=NULL;
		do 
		{
			fp=fopen(argv[2],"w");
			int ret = GetLastError();
			if(ret==123)
			{
				printf("open file error %d\n",ret);
				return -1;
			}
		} while (!fp);
		*stdout = *fp;
	}
	
	CFilelist fl;
	fl.setrootdir(lpdir);
	emundir(lpdir,fl);
	return 0;
}



