#include "windowsEx.h"
DWORD SetFileNoReadOnlyA(LPCSTR filename)
{
	DWORD ofa=GetFileAttributes(filename);
	if(ofa!=INVALID_FILE_ATTRIBUTES)
		SetFileAttributes(filename,ofa&~FILE_ATTRIBUTE_READONLY);
	return ofa;
}

BOOL CopyFileNoReadOnlyA(LPCSTR lpExistingFileName,
						LPCSTR lpNewFileName,
						BOOL bFailIfExists
						)
{
	std::string desdir=GetFileDirA(lpNewFileName);
	long cdret=SHCreateDirectoryExA(0,desdir.c_str(),0);//目录不存在时需要建立
	if(cdret!=ERROR_SUCCESS&&cdret!=ERROR_ALREADY_EXISTS&&cdret!=ERROR_FILE_EXISTS)
		return FALSE;
	DWORD ofa=SetFileNoReadOnlyA(lpNewFileName);
	return CopyFileA(lpExistingFileName,lpNewFileName,bFailIfExists);
}

void ClearDirectory(const char* szPath)
{

	if( szPath == NULL )
		return;
	std::string strPath = szPath;
	if( strPath.at(strPath.length()-1) != '\\' )
		strPath.append("\\");

	std::string strSearch = strPath+"*";
	std::string strTarget;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(strSearch.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return;
	} 
	do 
	{
		if( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			strTarget = strPath+FindFileData.cFileName;
			DeleteFile(strTarget.c_str());
		}
		else
		{
			if( 0 != strcmp(FindFileData.cFileName, ".") &&
				0 != strcmp(FindFileData.cFileName, "..") )
			{
				strTarget = strPath+FindFileData.cFileName;
				ClearDirectory(strTarget.c_str());
				RemoveDirectory(strTarget.c_str());
			}
		}
	} while( FindNextFile(hFind, &FindFileData) );
	FindClose(hFind);
}


void DeleteDirectoryWithFile(const char* szPath)
{
	ClearDirectory(szPath);
	RemoveDirectory(szPath);
}


void Time_tToFiletime(time_t t,FILETIME *ft)
{
	LONGLONG ll = Int32x32To64(t,10000000)+116444736000000000;
	ft->dwLowDateTime = (DWORD) ll;
	ft->dwHighDateTime = (DWORD)(ll >> 32);
}

BOOL SetFileTime_T(HANDLE hf,time_t createtime,time_t lastaccesstime,time_t lastmodifytime)
{	 
	FILETIME cft,mft,aft;
	if(createtime==0)
		createtime=time(0);
	Time_tToFiletime(createtime,&cft);
	if(lastmodifytime==0)
		lastmodifytime=time(0);
	Time_tToFiletime(lastmodifytime,&mft);
	if(lastaccesstime==0)
		lastaccesstime=time(0);
	Time_tToFiletime(lastaccesstime,&aft);
	return SetFileTime(hf,&cft,&aft,&mft);
}

std::string GetFileDirA(LPCSTR filePath)
{
	char sFilename[_MAX_PATH];
	char sDrive[_MAX_DIR];
	char sDir[_MAX_DIR];
	char sFname[_MAX_FNAME];
	char sExt[_MAX_EXT];
	_splitpath(filePath, sDrive, sDir, sFname, sExt);
	std::string dir=lstrcatA(sDrive,sDir);
	return dir;
}



BOOL ReadReg(TCHAR	*sKey, BYTE	*pBuffer,	DWORD	dwBufSize,HKEY	hkey, TCHAR	*sSubKey, DWORD	ulType)
{
	HKEY	hSubkey;
	if(RegOpenKeyEx(hkey, sSubKey, 0, KEY_READ, &hSubkey) != ERROR_SUCCESS)
		return FALSE;
	__try
	{
		DWORD	dwType;
		if (RegQueryValueEx(hSubkey, sKey, 0, &dwType, pBuffer, &dwBufSize) == ERROR_SUCCESS
			&& dwType == ulType)
			return TRUE;
		return FALSE;
	}
	__finally
	{
		RegCloseKey(hSubkey);
	}
	return FALSE;
}

BOOL RegistryGetValue(HKEY hk, const TCHAR * pszKey, const TCHAR * pszValue, DWORD dwType, LPBYTE data, DWORD dwSize)
{
    HKEY hkOpened;
    // Try to open the key.
    if (RegOpenKeyEx(hk, pszKey, 0, KEY_READ, &hkOpened) != ERROR_SUCCESS)
    {
        return FALSE;
    }
    // If the key was opened, try to retrieve the value.
    if (RegQueryValueEx(hkOpened, pszValue, 0, &dwType, (LPBYTE)data, &dwSize) != ERROR_SUCCESS)
    {
        RegCloseKey(hkOpened);
        return FALSE;
    }
    // Clean up.
    RegCloseKey(hkOpened);
    return TRUE
		;
}

DWORD SystemTimeToTime_t(const  SYSTEMTIME *st, time_t *pt )  
{
	FILETIME ft;
	SystemTimeToFileTime( st, &ft );

	LONGLONG ll;

	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;

	ll = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;
	*pt = (DWORD)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
	return *pt;
}
