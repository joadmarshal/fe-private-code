#ifndef __windowsEx_fe__
#define __windowsEx_fe__
#include <Windows.h>
#include <string>
#include <shlobj.h>
#include <time.h>
#pragma comment(lib,"shell32.lib")
DWORD SetFileNoReadOnlyA(LPCSTR filename);
BOOL CopyFileNoReadOnlyA(LPCSTR lpExistingFileName,
						LPCTSTR lpNewFileName,
						BOOL bFailIfExists
						);
void ClearDirectory(const char* szPath);
void DeleteDirectoryWithFile(const char* szPath);
void Time_tToFiletime(time_t t,FILETIME *ft);//
BOOL SetFileTime_T(HANDLE hf,time_t createtime,time_t lastaccesstime,time_t lastmodifytime);// 
std::string GetFileDirA(LPCSTR filePath);

BOOL ReadReg(TCHAR	*sKey, BYTE	*pBuffer,	DWORD	dwBufSize,HKEY	hkey, TCHAR	*sSubKey, DWORD	ulType);
BOOL RegistryGetValue(HKEY hk, const TCHAR * pszKey, const TCHAR * pszValue, DWORD dwType, LPBYTE data, DWORD dwSize);
DWORD SystemTimeToTime_t(const  SYSTEMTIME *st, time_t *pt );
#endif