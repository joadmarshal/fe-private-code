
#include "CMemMapFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapFile

CMapFile::CMapFile()
{
	DWORD dwlowFileSize=0;
	DWORD dwHighFileSize=0;
	m_pvFile=NULL;
	m_hFile=INVALID_HANDLE_VALUE;
}

CMapFile::~CMapFile()
{
	CloseMapFile();
	CloseFile();
}

/////////////////////////////////////////////////////////////////////////////
// CMapFile operation handlers
// 空文件会弹出 map could not be opened，在里面敲一个回车，就好了
BOOL CMapFile::OpenFile(std::string sFile)
{
	CloseFile();
	if(NULL!=m_pvFile)
	{
		UnmapViewOfFile(m_pvFile);//在当前应用程序的内存地址空间解除对一个文件映射对象的映射
		m_pvFile=NULL;
	}

	m_hFile = CreateFile(sFile.c_str(),GENERIC_READ|GENERIC_WRITE,
		0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if (m_hFile == INVALID_HANDLE_VALUE) 
	{ 
		return FALSE;
	}

	m_dwlowFileSize = GetFileSize(m_hFile, &m_dwHighFileSize);

	return TRUE;
}

PVOID CMapFile::MapView(DWORD dwoffsethigh,DWORD dwoffsetlow,DWORD dwLenth)
{
	if(m_hFile==INVALID_HANDLE_VALUE)
		return NULL;
	DWORD dwmh,dwml;
	if(dwoffsethigh>m_dwHighFileSize)
	{
		dwmh=dwoffsethigh;
		dwml=dwoffsetlow;
	}
	else if(dwoffsethigh==m_dwlowFileSize)
	{
		dwmh=dwoffsethigh;
		dwml=dwoffsetlow>m_dwlowFileSize?dwoffsetlow:m_dwlowFileSize;
	}
	else
	{
		dwmh=m_dwHighFileSize;
		dwml=m_dwlowFileSize;
	}
	m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, dwmh, dwml, NULL);
	if (m_hFileMap == NULL)
	{
		return FALSE;
	}
	m_pvFile = MapViewOfFile(m_hFileMap,FILE_MAP_WRITE|FILE_MAP_READ,dwmh,dwml,dwLenth);//获取映射文件在内存中的首地址

	//CloseHandle(hFileMap);// 关闭内存映射对象句柄
	if (m_pvFile == NULL) 
	{
		return m_pvFile;
	}
	//SetFilePointer(m_hFile,m_dwlowFileSize, m_dwHighFileSize, FILE_BEGIN);
	//SetEndOfFile(m_hFile); //设定当前文件指针所在处为文件结束处.该处后面的内容将被删除
	return m_pvFile;
}

void CMapFile::CloseMapFile()
{
	if(m_pvFile!=NULL)
		UnmapViewOfFile(m_pvFile);//取消内存视图映射
	if(m_hFileMap!=NULL)
		CloseHandle(m_hFileMap);// 关闭内存映射对象句柄
	m_pvFile=NULL;
	m_hFileMap=NULL;
}

void CMapFile::CloseFile()
{
	CloseMapFile();
	if(m_hFile!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);
	m_hFile=INVALID_HANDLE_VALUE;
}

