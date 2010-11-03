// MapFile.h : header file
#ifndef __FE__MEMMAPFILE__
#define __FE__MEMMAPFILE__

#include <Windows.h>
#include <string>
class CMapFile
{
public:
	CMapFile();           // protected constructor used by dynamic creation
	virtual ~CMapFile();
	// Attributes
private:
	PVOID m_pvFile;
	HANDLE m_hFileMap;
	DWORD m_dwlowFileSize;
	DWORD m_dwHighFileSize;
	HANDLE m_hFile;
	// Operations
public:
	BOOL OpenFile(std::string sFile);
private:
	PVOID MapView(DWORD offsethigh,DWORD offsetlow,DWORD dwLenth);//映射多大的文件1,高位偏移，2低位偏移，3长度
	void CMapFile::CloseFile();
	void CloseMapFile();
};


#endif // __FE__MEMMAPFILE__